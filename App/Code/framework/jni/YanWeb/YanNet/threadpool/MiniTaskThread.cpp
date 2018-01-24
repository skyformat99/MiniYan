/*
 * MiniMutex.cpp
 *
 *  Created on: 2015-11-20
 *      Author: yanbo
 */
#include "MiniTaskThread.h"
#include "SalLog.h"

namespace yanbo
{
MiniTaskThread::MiniTaskThread(MiniBlockQueue* queue)
    : m_queue(queue)
    , m_continue(LTrue)
{
}

MiniTaskThread::~MiniTaskThread()
{
}

void MiniTaskThread::run(void* ptr)
{
    while (m_continue)
    {
    	if (m_queue == NULL)
    	{
    		return;
    	}

    	KRefPtr<MiniTaskBase> task = m_queue->pollTask();
        if (task.get() != NULL)
        {
        	KFORMATLOG("MiniTaskThread::run listsize=%d", m_queue->size());

        	// 智能指针会自动释放内存
        	task->execute();
        	KFORMATLOG("MiniTaskThread::run listsize=%d end", m_queue->size());
        }
        else
        {
        	MiniThread::waitOnNotify();
        }
    }
}

void MiniTaskThread::stop()
{
	m_continue = LFalse;
}

}
