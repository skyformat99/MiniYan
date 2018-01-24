#ifndef MiniClientListener_h
#define MiniClientListener_h

namespace yanbo
{
class MiniClientListener
{
public:
	virtual void onSocketDataReceive(const char* data, int length) = 0;
	virtual void onSocketDataFinished() = 0;
	virtual void onSocketDataError(int code) = 0;
};
}

#endif
