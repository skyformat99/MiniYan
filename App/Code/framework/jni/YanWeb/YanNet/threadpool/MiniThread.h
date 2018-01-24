/*
 * MiniThreadAndroid.h
 *
 *  Created on: 2015-7-29
 *      Author: yanbo
 */
#ifndef MiniThreadAndroid_h
#define MiniThreadAndroid_h

#include "MiniMutex.h"
#include <pthread.h>

namespace yanbo
{

class MiniThread
{
public:
	MiniThread();
	virtual ~MiniThread();

	void						start();

    int				            wait();
    static void 			    waitOnNotify();
    static void				    notify();

    virtual void				stop();

protected:
    bool						mRunning;
    static MiniMutex     	    sLock;

    virtual void                run(void* ptr);

private:
    pthread_t                   mThread;
    static pthread_cond_t		sCondition;

	static void*				startThread(void* ptr);
};
}
#endif
