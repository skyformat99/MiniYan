#include "InlineItem.h"

namespace yanbo
{

InlineItem::InlineItem(IItemListener* itemListener, const StringA& id, 
		const StringA& name, LBool selectable)
    : HtmlItem(itemListener, id, name, selectable)
{
}

InlineItem::~InlineItem()
{	
}

void InlineItem::addChild(HtmlItem* child)
{
	addChildIgnoringContinuation(child);
}

void InlineItem::addChildToContinuation(HtmlItem* child)
{
}

void InlineItem::addChildIgnoringContinuation(HtmlItem* child)
{
	HtmlItem::addChild(child);
}

LBool InlineItem::isInline() const
{
	return LTrue;
}

}
