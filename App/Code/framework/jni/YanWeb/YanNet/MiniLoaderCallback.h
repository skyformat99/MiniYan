#ifndef MiniLoaderCallback_h
#define MiniLoaderCallback_h

namespace yanbo
{
class MiniLoaderCallback
{
public:
	virtual void onLoadHeader(KRefPtr<MiniLoadResponse> response);
	virtual void onLoadReceived(const char* bytes, int length);
	virtual void onLoadError();
};
}

#endif

