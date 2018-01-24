#include "MiniLoaderTask.h"
#include "StringUtils.h"
#include "SalLog.h"
#include "StringBuffer.h"

namespace yanbo
{
#define HTTP_URL_HEAD "http://"
#define HTTP_GET "GET "
#define HTTP_POST "POST "
#define HTTP_VERSON " HTTP/1.1\r\n"
#define HTTP_HEADER_END "\r\n"
#define HTTP_HOST "HOST: "
#define HTTP_ROOT_DIR "/"
#define HTTP_DEFAULT_PORT 80
#define HTTP_HEADER_CONTENT_TYPE "Content-Type"
#define HTTP_FORM_HEAD_VALUE "application/x-www-form-urlencoded"

class HTTPUri
{
public:
	HTTPUri()
	{
	}
	~HTTPUri()
	{
	}

public:
	CString m_host;
	int m_port;
	CString m_file;
};

MiniLoaderTask::MiniLoaderTask(MiniLoadRequest* request)
    : m_request(request)
    , m_parser(NULL)
    , m_callback(NULL)
{
}

MiniLoaderTask::~MiniLoaderTask()
{
	// 检查自动析构过程
	if (m_parser != NULL)
	{
		delete m_parser;
		m_parser = NULL;
	}
	KLOG("MiniLoaderTask ~MiniLoaderTask()");
}

void MiniLoaderTask::execute()
{
	init();
}

void MiniLoaderTask::init()
{
	KLOG("MiniLoaderTask init");
	m_socket = new MiniClientSocket(this);
	// 解析url转成HTTPUri
	HTTPUri uri;
	parseUrl(m_request->m_url, uri);
	KLOG("MiniLoaderTask parseUrl END");
    // 处理HTTPUri
	m_socket->initClientSocket(uri.m_host, uri.m_port);
	KSTRFORMAT("MiniLoaderTask initClientSocket END host=%s", uri.m_host);
	KSTRFORMAT("MiniLoaderTask initClientSocket END file=%s", uri.m_file);
	switch (m_request->m_method)
	{
	case MiniLoadRequest::EGet:
		requestGet(uri);
		break;
	case MiniLoadRequest::EPost:
		requestPost(uri);
		break;
	}

	KLOG("MiniLoaderTask init END");
}

void MiniLoaderTask::parseUrl(CString& url, HTTPUri& uri)
{
	CString urlHead = HTTP_URL_HEAD;
	CString newUrl = url.Mid(urlHead.GetLength(), url.GetLength() - urlHead.GetLength());
    int hostEndPos = newUrl.Find("/");
    CString host = newUrl.Mid(0, hostEndPos); // 获取HOST
    int portPos = host.Find(":");

    // HOST赋值
    if (portPos != -1)
    {
    	uri.m_host = host.Mid(0, portPos);
        CString port = host.Mid(portPos+1, host.GetLength() - portPos - 1);
        uri.m_port = LStr2Int((const LUint8*)port.GetBuffer(), 10);
    }
    else
    {
    	uri.m_port = HTTP_DEFAULT_PORT;
    	uri.m_host = host;
    }

	//KSTRFORMAT("MiniLoaderTask, parseUrl HOST=%s", host);
	if (hostEndPos != -1 && hostEndPos < newUrl.GetLength() - 1)
	{
		uri.m_file = newUrl.Mid(hostEndPos + 1, newUrl.GetLength() - host.GetLength() - 1);
	}
	else
	{
		uri.m_file = HTTP_ROOT_DIR;
	}
}

// GET请求方式
void MiniLoaderTask::requestGet(const HTTPUri& uri)
{
	StringBuffer headerBuffer;
	headerBuffer.append(HTTP_GET); // GET请求方式
	headerBuffer.append(uri.m_file); // 请求的URL
	headerBuffer.append(HTTP_VERSON); // HTTP协议版本
	headerBuffer.append(HTTP_HOST);
	headerBuffer.append(uri.m_host);
	headerBuffer.append(HTTP_HEADER_END);

    //headerStr += "Accept: */*\r\nUser-Agent: Lowell-Agent\r\nConnection: Close\r\n";
	initHeaders(headerBuffer);
    KLOG("MiniLoaderTask requestGet 2");

	KRefPtr<CString> headerStr = headerBuffer.toString();
	//CString str = *(headerStr.get());
	KFORMATLOG("StringBuffer::append str sPtr1=%s", (const char *)headerStr->GetBuffer());
	//KSTRFORMAT("MiniLoaderTask requestGet headerStr=%s", str);
    m_socket->requestData(*headerStr.get());
}

void MiniLoaderTask::requestPost(const HTTPUri& uri)
{
	StringBuffer headerBuffer;
	headerBuffer.append(HTTP_POST); // 请求方式
	headerBuffer.append(uri.m_file); // 请求的URL
	headerBuffer.append(HTTP_VERSON); // HTTP协议版本
	headerBuffer.append(HTTP_HOST);
	headerBuffer.append(uri.m_host);
	headerBuffer.append(HTTP_HEADER_END);

    initHeaders(headerBuffer);
    initParams(headerBuffer);

    m_socket->requestData(*headerBuffer.toString().get());
}

void MiniLoaderTask::initHeaders(StringBuffer& headerBuffer)
{
	if (!m_request->m_headers.isEmpty())
	{
	    RequestMap::Iterator iter = m_request->m_headers.begin();
	    RequestMap::Iterator iterEnd = m_request->m_headers.end();
		for (; iter != iterEnd; ++iter)
		{
			headerBuffer.append((*iter)->getKey());
			headerBuffer.append(": ");
			headerBuffer.append((*iter)->getValue());
			headerBuffer.append(HTTP_HEADER_END);
		}
	}

	headerBuffer.append(HTTP_HEADER_END);
	//KSTRFORMAT("initParams str=%s", headerStr);
}

void MiniLoaderTask::initParams(StringBuffer& paramBuffer)
{
	if (m_request->m_params.isEmpty())
	{
		return;
	}

    RequestMap::Iterator iter = m_request->m_params.begin();
    RequestMap::Iterator iterEnd = m_request->m_params.end();
	for (; iter != iterEnd; ++iter)
	{
		paramBuffer.append((*iter)->getKey());
		paramBuffer.append("=");
		paramBuffer.append((*iter)->getValue());
		paramBuffer.append("&");
	}

	//KSTRFORMAT("initParams str=%s", postStr);
}

void MiniLoaderTask::onSocketDataReceive(const char* data, int length)
{
	KFORMATLOG("MiniLoaderTask onSocketDataReceive begin onSocketDataReceive=%d", length);

	// 如果m_headerEnd<4则，执行Header获取
	// 否则，则开始加载真实数据
	if (m_parser == NULL)
	{
		m_parser = new HTTPParser(m_callback);
	}

	if (m_parser->parseResponse(data, length))
	{
		m_socket->blockClose();
		//onSocketDataFinished();
	}

}

void MiniLoaderTask::onSocketDataFinished()
{

	KLOG("HTTPParser::parseResponse MiniLoaderTask::onSocketDataFinished()");
//	KRefPtr<CString> ptr = m_parser->getBody();
//	if (ptr != NULL)
//	{
//		KSTRFORMAT("MiniLoaderTask onSocketDataFinished begin str=%s", ptr->Left(100));
//		KSTRFORMAT("MiniLoaderTask onSocketDataFinished end str=%s", ptr->Right(100));
//	}

}

void MiniLoaderTask::onSocketDataError(int code)
{
    if (m_callback != NULL)
	{
		m_callback->onLoadError();
	}
}

void MiniLoaderTask::setCallback(MiniLoaderCallback* callback)
{
	m_callback = callback;
}

}
