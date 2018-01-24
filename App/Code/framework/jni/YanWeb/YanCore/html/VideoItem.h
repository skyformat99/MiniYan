#ifndef VideoItem_h
#define VideoItem_h

#include "InlineItem.h"

namespace yanbo
{

class VideoItem: public InlineItem
{
public:
	VideoItem(
			IItemListener* itemListener,
			const StringA& id,
			const StringA& name,
			LBool selectable,
			StringA src);

	~VideoItem();

	virtual void layout(RenderContext& rc);

private:
	void loadVideo();

private:
	StringA m_src;
	util::MediaPlayer* m_mediaPlayer;
};
}
#endif
