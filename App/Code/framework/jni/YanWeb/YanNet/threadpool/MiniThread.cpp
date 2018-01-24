/*
 * MiniThreadAndroid.cpp
 *
 *  Created on: 2015-7-29
 *      Author: yanbo
 */
#include "AutoLock.h"
#include "MiniThread.h"
#include "SalLog.h"

#define TAG "MiniThread"

namespace yanbo
{

MiniMutex MiniThread::sLock;
pthread_cond_t MiniThread::sCondition;

MiniThread::MiniThread()
{
}

MiniThread::~MiniThread()
{
}

void MiniThread::start()
{
    pthread_create(&mThread, NULL, startThread, this);
}

int MiniThread::wait()
{
	if (!mRunning)
	{
		return 0;
	}

	// pthread_join function means waiting for the
	// thread end
    return pthread_join(mThread, NULL);
}

void MiniThread::stop()
{
}

void* MiniThread::startThread(void* ptr)
{
	MiniThread* thread = (MiniThread *) ptr;
	thread->mRunning = true;
    thread->run(ptr);
	thread->mRunning = false;
}

void MiniThread::waitOnNotify()
{
	AutoLock lock(&sLock);
	pthread_cond_wait(&sCondition, sLock.getMutex());
}

void MiniThread::notify()
{
	AutoLock lock(&sLock);
	pthread_cond_signal(&sCondition);
}

void MiniThread::run(void* ptr)
{
	KLOG("MiniThreadAndroid::run");
}

}
