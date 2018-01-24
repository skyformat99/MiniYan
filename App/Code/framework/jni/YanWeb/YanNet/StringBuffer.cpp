#include "StringBuffer.h"
#include "SalLog.h"

namespace yanbo
{
// const字串和CString不能执行delete
// 故需要加以判断，防止出错
class BufferItem : public KRef
{
public:
	BufferItem(LBool isConst = LFalse)
	    : buffer(NULL)
	    , length(0)
		, pos(0)
	    , bConst(isConst)
	{
	}

	~BufferItem()
	{
		if (buffer != NULL && !bConst)
		{
			KFORMATLOG("HTTPParser::parseResponse delete item address=%d and length=%d", (int) buffer, length);
            delete[] buffer;
            buffer = NULL;
		}
	}

public:
    char* buffer;
    int length;
	int pos;
	int bConst;
};

StringBuffer::StringBuffer()
    : m_length(0)
{
}

StringBuffer::~StringBuffer()
{
}

void StringBuffer::append(const char ch)
{
	char* chPtr = new char[1];
	*chPtr = ch;
	append(chPtr, 0, 1, LFalse);
}

void StringBuffer::append(const CString& str)
{
	append(str.GetBuffer(), 0, str.GetLength(), LTrue);
}

void StringBuffer::append(const char* buffer)
{
	//LInt len = util::LStrlen((const LUint8*)buffer);
	//KFORMATLOG("StringBuffer::append str len=%d", len);
	append(buffer, 0, util::LStrlen((const LUint8*)buffer), LTrue);
}

void StringBuffer::append(const char* buffer, int pos, int len, LBool isConst)
{
	if (len <= 0)
	{
		return;
	}

	KRefPtr<BufferItem> item = new BufferItem(isConst);
	item->buffer = (char*)(buffer);
	item->length = len;
	item->pos = pos;
	m_length += len;
	m_buffer.push_back(item);
}

KRefPtr<CString> StringBuffer::toString() const
{
	KFORMATLOG("StringBuffer::append str m_length=%d", m_length);
	if (m_length == 0)
	{
		return NULL;
	}

	//KFORMATLOG("StringBuffer::append str m_length1=%d", m_length);
    char* buffer = new char[m_length+1];
    util::LMemset(buffer, 0, m_length+1);
    KList<KRefPtr<BufferItem> >::Iterator iter = m_buffer.begin();
    KList<KRefPtr<BufferItem> >::Iterator iterEnd = m_buffer.end();

    int pos = 0;
    for (; iter != iterEnd; ++iter)
    {
        util::LMemcpy(buffer+pos, (*iter)->buffer + (*iter)->pos, (*iter)->length);
        pos += (*iter)->length;
    }

	// 这里使用字符串浅拷贝，因此不用释放buffer
	// 最后的释放过程由sptr代理完成
    //KRefPtr<CString> sPtr = new CString(buffer, LTrue, m_length);
	//KFORMATLOG("StringBuffer::append str sPtr=%s", (const char *)sPtr->GetBuffer());
    //return sPtr;
	return new CString(buffer, LTrue, m_length);
}

void StringBuffer::clear()
{
	m_length = 0;
	m_buffer.clear();
}

LInt StringBuffer::size() const
{
	return m_buffer.count();
}

}
