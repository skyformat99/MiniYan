/*
 * LinkItem.h
 *
 *  Created on: 2011-6-30
 *      Author: yanbo
 */

#ifndef LinkItem_h
#define LinkItem_h

#include "InlineItem.h"

namespace yanbo
{

class LinkItem : public InlineItem
{
public:
    LinkItem(IItemListener* itemListener, 
			const StringA& id,
			const StringA& name,
			const StringA& url);
    
    ~LinkItem();
    
public:
    virtual LBool isLink() const;
    
	virtual void execute();
	void setLinkFocus(HtmlItem* item, const LBool selected);
	
};

}

#endif /* LINKITEM_H_ */
