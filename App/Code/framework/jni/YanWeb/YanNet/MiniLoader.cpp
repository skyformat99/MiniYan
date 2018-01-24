#include "MiniLoader.h"
#include "MiniThreadPool.h"

namespace yanbo
{
MiniLoader::MiniLoader(MiniLoaderCallback* callback)
    : m_task(NULL)
    , m_callback(callback)
{
	m_request = new MiniLoadRequest();
}

MiniLoader::~MiniLoader()
{
	if (m_request != NULL)
	{
        delete m_request;
        m_request = NULL;
	}

//	if (m_task != NULL)
//	{
//		delete m_task;
//		m_task = NULL;
//	}
}

void MiniLoader::reset()
{
	m_request->resetRequest();
}

void MiniLoader::setCallback(MiniLoaderCallback* callback)
{
	m_callback = callback;
}

void MiniLoader::load(const CString& url)
{
	if (m_task != NULL)
	{
		delete m_task;
		m_task = NULL;
	}

	m_request->m_url = url;
	m_task = new MiniLoaderTask(m_request);

	MiniThreadPool::getInstance()->sendMiniTask(m_task);
}

}
