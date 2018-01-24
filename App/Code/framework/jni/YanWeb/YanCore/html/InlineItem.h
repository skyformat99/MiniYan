/*
 * InlineItem.h
 *
 *  Created on: 2012-8-06
 *      Author: yanbo
 */

#ifndef InlineItem_h
#define InlineItem_h

#include "HtmlItem.h"

namespace yanbo
{

class InlineItem : public HtmlItem
{
public:
	InlineItem(IItemListener* itemListener, const StringA& id, 
		const StringA& name, LBool selectable);
	virtual ~InlineItem();

	virtual void addChild(HtmlItem* child);

public:
	virtual LBool isInline() const;

private:
	void addChildToContinuation(HtmlItem* child);
	void addChildIgnoringContinuation(HtmlItem* child);
};

}
#endif
