#ifndef MiniLoader_h
#define MiniLoader_h

#include "UtilString.h"
#include "MiniLoaderTask.h"
#include "MiniLoadRequest.h"
#include "MiniLoaderCallback.h"

namespace yanbo
{
class MiniLoader
{
public:
	MiniLoader(MiniLoaderCallback* callback);
	~MiniLoader();

	void load(const CString& url);
	void reset();
	void setCallback(MiniLoaderCallback* callback);

private:
	MiniLoaderTask* m_task;
	MiniLoadRequest* m_request;
	MiniLoaderCallback* m_callback;
};
}

#endif
