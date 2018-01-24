/*
 * LinkItem.cpp
 *
 *  Created on: 2011-7-21
 *      Author: Administrator
 */

#include "LinkItem.h"
#include "TextItem.h"
#include "StringUtils.h"

namespace yanbo
{

LinkItem::LinkItem(IItemListener* itemListener, 
		const StringA& id,
		const StringA& name,
		const StringA& url)
    : InlineItem(itemListener, id, name, LTrue)		
{
	m_url = url;
}

LinkItem::~LinkItem()
{
	
}

LBool LinkItem::isLink() const
{
	KLOG("LinkItem::isLink()");
	return LTrue;
}

void LinkItem::execute()
{
	if (m_url.GetLength() > 0 && NULL != m_itemListener)
	{
	    String url;
	    util::StringUtils::strTostrW(m_url, url);
	    m_itemListener->onClick(url, m_onClick);
	}
}

}
