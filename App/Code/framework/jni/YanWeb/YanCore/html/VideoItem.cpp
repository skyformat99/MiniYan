#include "VideoItem.h"

namespace yanbo
{
VideoItem::VideoItem(
		IItemListener* itemListener,
		const StringA& id,
		const StringA& name,
		LBool selectable,
		StringA src)
    : InlineItem(itemListener, id, name, selectable)
    , m_mediaPlayer(NULL)
    , m_src(src)
{
}

VideoItem::~VideoItem()
{
}

void VideoItem::loadVideo()
{

}

void VideoItem::layout(RenderContext& rc)
{

}


}
