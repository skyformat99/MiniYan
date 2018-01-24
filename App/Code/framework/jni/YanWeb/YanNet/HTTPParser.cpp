//
// Created by yanbo on 2017/8/28.
//

#include "HTTPParser.h"
#include "SalLog.h"
#include "StringUtils.h"

#define CONTENT_LENGTH             _CS("Content-Length")
#define TRANSFER_ENCODING          _CS("Transfer-Encoding")
#define TRANSFER_ENCODING_CHUNKED  _CS("chunked")
#define LOCATION                   _CS("Location")

namespace yanbo
{
HTTPParser::HTTPParser(MiniLoaderCallback* callback)
    : m_headerEnd(0)
    , m_chunkedFlag(LFalse)
    , m_currentChunkLength(0)
    , m_currentChunkSize(0)
    , m_callback(callback)
{
    m_buffer = new StringBuffer();
}

HTTPParser::~HTTPParser()
{
    if (m_buffer != NULL)
    {
        delete m_buffer;
        m_buffer = NULL;
    }
}

LBool HTTPParser::parseResponse(const char* data, int length)
{

    KFORMATLOG("HTTPParser::parseResponse function begin length=%d", length);

    CString src(data, length);

    KFORMATLOG("HTTPParser::parseResponse src str=%s", src.GetBuffer());
    // 如果m_headerEnd<4则，执行Header获取
    // 否则，则开始加载真实数据
    int begin = 0;
    if (m_headerEnd < 4)
    {
        //testHeaders(data, length);
        for (int i = 0; i < length; i++)
        {
            execHeaderEndByte(*(data + i));
            // 如果是\r\n,则表示需要换行
            // \r\n前面数据为header数据
            // 一旦碰到\r, 则表示可以开始捕获
            // 前面的数据, 如果标记为4,则退出
            // 获取header的步骤, 所截取的字符串
            // 长度为i-begin
            if (m_headerEnd == 1) // '\r'
            {
                //KFORMATLOG("HTTPParser::parseResponse m_headerEnd=%d i=%d", m_headerEnd, i);
                // cannotDelete表示是否不需要删除
                // 当i == length - 1时表示需要删除
                if (i - begin > 0)
                {
                    LBool cannotDelete = i != length - 1 ? LTrue : LFalse;
                    m_buffer->append(data, begin, i - begin, cannotDelete);
                }

                // 解析header
                parseResponseHeader();
                m_buffer->clear();

                // 跳过\r\n
                begin = i + 2;
            }
            else if (m_headerEnd == 2) // '\n'
            {
                //KFORMATLOG("HTTPParser::parseResponse m_headerEnd=%d", m_headerEnd);
                begin = i + 1;
                if (i == length - 1)
                {
                    delete data;
                    return LFalse;
                }
            }
            else if (m_headerEnd == 3) // '\r'
            {
                //KFORMATLOG("HTTPParser::parseResponse m_headerEnd=%d", m_headerEnd);
                begin = i + 2;
                if (i == length - 1)
                {
                    delete data;
                    return LFalse;
                }
            }
            else if (m_headerEnd == 0 && i == length - 1 && length - begin > 0)
            {
                //KFORMATLOG("HTTPParser::parseResponse m_headerEnd=%d", m_headerEnd);
                // 该功能是用于添加尾部数据
                // 如果该数据没有遇到\r, 并且该数据的长度大于0
                // 最后一个参数为LFalse表示该item控制数据删除
                m_buffer->append(data, begin, length - begin, LFalse);
            }
            else if (m_headerEnd == 4) // '\n'
            {
                if (m_callback != NULL)
                {
                    m_callback->onLoadHeader(m_response);
                }

                //KFORMATLOG("HTTPParser::parseResponse m_headerEnd=%d", m_headerEnd);
                begin = i + 1;
                if (i == length - 1)
                {
                    delete data;
                    return LFalse;
                }
                else
                {
                    break;
                }
            }
        }
    }

    // 超过4后，开始解析正文
    if (m_headerEnd >= 4 && length - begin > 0)
    {
        LBool result = parseResponseBody(data, begin, length);
        // 卸磨杀驴中。。。
        delete data;
        return result;
    }

    return LFalse;
}

void HTTPParser::parseResponseHeader()
{
    if (m_response == NULL)
    {
        m_response = new MiniLoadResponse();
    }

    KRefPtr<CString> header = m_buffer->toString();

    KFORMATLOG("HTTPParser::parseResponse parseResponseHeader headerStr=%s", (const char*)header->GetBuffer());
    // header指针会自动delete，所以src必须设置为LFalse
    StringA src((const LUint8*)header->GetBuffer(), LFalse, header->GetLength());

    // status为0，则表示进入HTTP/1.1 200 OK
    if (m_response->getStatus() == 0)
    {
        KRefPtr<KVector<StringA> > array = StringUtils::split(src, _CS(" "));
        StringA statusStr = StringUtils::skipBlank(array->elementAt(1));
        m_response->setStatus(StringUtils::stringToInt(statusStr));
        //KFORMATLOG("parseResponseHeader status=%d", m_response->getStatus());
    }
    else
    {
        KRefPtr<KVector<StringA> > array = StringUtils::split(src, _CS(":"));
        if (array->elementAt(0).FindNoCase(CONTENT_LENGTH) >= 0)
        {
            StringA lengthStr = StringUtils::skipBlank(array->elementAt(1));
            m_response->setContentLength(StringUtils::stringToInt(lengthStr));
        }
        else if (array->elementAt(0).FindNoCase(TRANSFER_ENCODING) >= 0
                && array->elementAt(1).FindNoCase(TRANSFER_ENCODING_CHUNKED) >= 0)
        {
            m_response->setIsChunked(LTrue);
        }
        else if (array->elementAt(0).FindNoCase(LOCATION))
        {
            m_response->setRedirectUrl(StringUtils::skipBlank(array->elementAt(1)));
        }
    }

    //KFORMATLOG("parseResponseHeader isChunck=%d", (LInt)m_response->isChunked());
}

LBool HTTPParser::parseResponseBody(const char* data, int begin, int length)
{
    // 如果HTTP传输协议使用chunked
    if (m_response->isChunked())
    {
        if (m_chunkedFlag)
        {
            int leftLen = m_currentChunkSize - m_currentChunkLength;
            KFORMATLOG("HTTPParser::parseResponse leftLen=%d", leftLen);
            int minLength = leftLen > (length - begin) ? (length - begin) : leftLen;

            if (minLength > 0)
            {
                if (m_callback != NULL)
                {
                    m_callback->onLoadReceived(data + begin, minLength);
                }

                m_currentChunkLength += minLength;
            }

            // 表示该chunk已经收取完毕
            if (leftLen <= length - begin)
            {
                m_currentChunkSize = 0;
                m_chunkedFlag = LFalse;
                m_currentChunkLength = 0;

                if (leftLen != length - begin)
                {
                    return parseResponseBody(data, begin + leftLen, length);
                }
            }

        }
        else
        {
            // 如果标记为LFalse，则表示需要开始获取长度
            for (int i=begin; i<length; i++)
            {
                char ch = *(data + i);

                if (ch == '\n' && m_buffer->size() > 0)
                {
                    // 找到'\n'表示需要开始获取真实数据了
                    m_chunkedFlag = LTrue;
                    KRefPtr<CString> lengthStr = m_buffer->toString();
                    // 终于找到需要该知道的长度信息了
                    StringA src((const LUint8*)lengthStr->GetBuffer(), LFalse, lengthStr->GetLength());
                    //StringA statusStr = StringUtils::skipBlank(src);
                    // 数据堆长度必须减去2，因为一个chunk结束必带/r/n
                    m_currentChunkSize = StringUtils::stringToInt(src, 16);

                    KFORMATLOG("HTTPParser::parseResponse m_currentChunkSize=%d", m_currentChunkSize);
                    // 清除Buffer
                    m_buffer->clear();

                    if (m_currentChunkSize == 0)
                    {
                        return LTrue;
                    }
                    // 跳过/r/n
                    begin = i + 1;
                    // 找到/n，赶紧逃跑
                    break;
                }
                else
                {
                    if (!StringUtils::isspace(ch))
                    {
                        KFORMATLOG("HTTPParser::parseResponse size data=%c", ch);
                        m_buffer->append(ch);
                    }
                }
            }

            // 查找过程中发现data数据还没有被用完
            if (m_chunkedFlag)
            {
                int len = length - begin;
                if (len <= 0)
                {
                    return LFalse;
                }

                if (m_currentChunkSize >= len)
                {
                    if (m_callback != NULL)
                    {
                        m_callback->onLoadReceived(data + begin, len);
                    }

                    m_currentChunkLength += len;
                }
                else
                {
                    if (m_callback != NULL)
                    {
                        m_callback->onLoadReceived(data + begin, m_currentChunkSize);
                    }

                    len = m_currentChunkSize;
                    m_currentChunkSize = 0;
                    m_currentChunkLength = 0;
                    m_chunkedFlag = LFalse;

                    // data字串没有被利用完，继续解析
                    return parseResponseBody(data, begin + len, length);
                }
            }
        }
    }
    else
    {
        KFORMATLOG("HTTPParser::parseResponse left_size=%d", (length - begin));

        // 如果不是chunked， 则直接返回数据
        if (m_callback != NULL)
        {
            m_callback->onLoadReceived(data + begin, length - begin);
        }

        m_currentChunkLength += length - begin;
        if (m_currentChunkLength >= m_response->getContentLength())
        {
            return LTrue;
        }
    }

    return LFalse;
}

void HTTPParser::execHeaderEndByte(const char ch)
{
    if (((m_headerEnd == 0 || m_headerEnd == 2)  && ch == '\r')
            || ((m_headerEnd == 1 || m_headerEnd == 3) && ch == '\n'))
    {
        ++m_headerEnd;
    }
    else
    {
        m_headerEnd = 0;
    }
}

}