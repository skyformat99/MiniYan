/*
 * WebViewController.cpp
 *
 *  Created on: 2011-11-11
 *      Author: yanbo
 */

#include "WebViewController.h"
#include "WebView.h"

namespace yanbo
{

WebViewController::WebViewController(WebView* view)
    : m_view(view)
{
	
}

void WebViewController::onUpKey(LBool repeated)
{
	HtmlItem* item = m_view->getDocument()->getCurrentItem();
	item->setSelected(LFalse);
	
	item = m_view->getDocument()->getPreItem();
	while (!item->isSelectable())
	{    
	    HtmlItem* preItem = m_view->getDocument()->getPreItem();
	    
	    if (item == preItem)
	    {
	        break;
	    }
	    
	    item = preItem;
	}
	
	if (item->isSelectable())
	{
	    item->setSelected(LTrue);
	    
	    if (item->getYpos() - item->getScrollYPos() < 0)
	    {
	        int scrollYPos = item->getScrollYPos();
	        scrollYPos -= scrollYPos - item->getYpos();
	        m_view->getDocument()->getRenderTreeRoot()->setScrollPos(0, scrollYPos);
	    }
	}
		
	m_view->getLoader()->repaint();
}

void WebViewController::onLeftKey(LBool repeated)
{
	
}

void WebViewController::onDownKey(LBool repeated)
{
	HtmlItem* item = m_view->getDocument()->getCurrentItem();
	item->setSelected(LFalse);
	
	item = m_view->getDocument()->getNextItem();
	while (!item->isSelectable())
	{    
	    HtmlItem* nextItem = m_view->getDocument()->getNextItem();
	    
	    if (item == nextItem)
	    {
	        break;
	    }
	    
	    item = nextItem;
	}
	
	if (item->isSelectable())
	{
	    item->setSelected(LTrue);
	    if (item->getYpos() - item->getScrollYPos() > m_view->getClientRange().GetHeight())
	    {
	        int scrollYPos = item->getScrollYPos();
	        scrollYPos += item->getYpos() - scrollYPos - m_view->getClientRange().GetHeight() + m_view->getClientRange().GetHeight()/8;
	        m_view->getDocument()->getRenderTreeRoot()->setScrollPos(0, scrollYPos);
	    }
	}
		
	m_view->getLoader()->repaint();
}

void WebViewController::onRightKey(LBool repeated)
{
	
}

void WebViewController::onEnterKey(LBool repeated)
{
    m_view->getDocument()->getCurrentItem()->execute();	
}

void WebViewController::onMousePress(const LPoint& pt)
{
	
}

void WebViewController::onMouseUp(const LPoint& pt)
{
	
}

void WebViewController::onTouchDown(const LPoint& pt)
{

}
void WebViewController::onTouchUp(const LPoint& pt)
{

}

}
