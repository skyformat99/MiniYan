#ifndef MediaPlayerAndroid_h
#define MediaPlayerAndroid_h

#include "LGdi.h"

namespace util
{
struct JMediaPlayer;
class MediaPlayerAndroid : public MediaPlayer
{
public:
	MediaPlayerAndroid();
	~MediaPlayerAndroid();

public:
	virtual void start();
	virtual void play();
	virtual void pause();
	virtual void stop();
	virtual void release();
	virtual void reset();

private:
	struct JMediaPlayer* m_playerPrivate;
};
}

#endif
