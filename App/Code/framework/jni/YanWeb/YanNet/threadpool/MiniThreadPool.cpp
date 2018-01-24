/*
 * MiniMutex.cpp
 *
 *  Created on: 2015-11-20
 *      Author: yanbo
 */
#include "MiniThreadPool.h"

namespace yanbo
{
const int KMiniThreadPoolSize = 3;
MiniThreadPool* MiniThreadPool::s_pool = NULL;
MiniThreadPool::MiniThreadPool()
    : m_poolSize(0)
{
	m_queue = new MiniBlockQueue();
}

MiniThreadPool::~MiniThreadPool()
{
	KList<KRefPtr<MiniTaskThread> >::Iterator iter = m_threadList.begin();
	KList<KRefPtr<MiniTaskThread> >::Iterator iterEnd = m_threadList.end();
	for (; iter != iterEnd; ++iter)
	{
		(*iter)->stop();
	}

	m_threadList.clear();
}

MiniThreadPool* MiniThreadPool::getInstance()
{
    if (s_pool == NULL)
    {
    	s_pool = new MiniThreadPool();
    }

    return s_pool;
}

void MiniThreadPool::destroy()
{
	if (s_pool != NULL)
	{
		delete s_pool;
		s_pool = NULL;
	}
}

void MiniThreadPool::sendMiniTask(MiniTaskBase* task)
{
	KFORMATLOG("sendMiniTask MiniTaskThread::run listsize=%d", m_queue->size());
	// 添加task
	{
		m_queue->addTask(task);
	}

	if (m_threadList.empty() || m_poolSize < 3)
	{
		KRefPtr<MiniTaskThread> thread = new MiniTaskThread(m_queue.get());
        m_threadList.push_back(thread);
        thread->start();
        m_poolSize++;
	}
	else
	{
		MiniThread::notify();
	}
}



}
