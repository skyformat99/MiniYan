#ifndef MiniLoaderTask_h
#define MiniLoaderTask_h

#include "MiniTaskBase.h"
#include "UtilString.h"
#include "MiniLoadRequest.h"
#include "MiniClientSocket.h"
#include "MiniClientListener.h"
#include "KRefPtr.h"
#include "StringBuffer.h"
#include "HTTPParser.h"
#include "MiniLoaderCallback.h"

namespace yanbo
{
class HTTPUri;
class MiniLoaderTask : public MiniTaskBase, MiniClientListener
{
public:
	MiniLoaderTask(MiniLoadRequest* request);
	virtual ~MiniLoaderTask();

	void setCallback(MiniLoaderCallback* callback);

public:
	virtual void execute();

private:
	void init();
	void requestGet(const HTTPUri& uri);
	void requestPost(const HTTPUri& uri);
	void initHeaders(StringBuffer& headerStr);
	void initParams(StringBuffer& postStr);
	void parseUrl(CString& url, HTTPUri& uri);

	virtual void onSocketDataReceive(const char* data, int length);
	virtual void onSocketDataFinished();
	virtual void onSocketDataError(int code);

private:
	KRefPtr<MiniClientSocket> m_socket;
	KRefPtr<MiniLoadRequest> m_request;
	MiniLoaderCallback* m_callback;
	//StringBuffer m_buffer;
	// m_headerEnd作用如下：
	// 1, /r; 2, /n; 3, /r; 4, /n
	//LInt         m_headerEnd;
	HTTPParser* m_parser;
};

}
#endif
