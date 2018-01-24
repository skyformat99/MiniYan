#ifndef MiniTaskThread_h
#define MiniTaskThread_h

#include "MiniThread.h"
#include "MiniBlockQueue.h"

namespace yanbo
{
class MiniTaskThread : public MiniThread, public KRef
{
public:
	MiniTaskThread(MiniBlockQueue* queue);
	~MiniTaskThread();

	virtual void stop();
protected:
	virtual void run(void* ptr);

private:
	MiniBlockQueue* m_queue;
	LBool m_continue;
};

}
#endif
