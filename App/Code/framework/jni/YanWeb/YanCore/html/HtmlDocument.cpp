/*
 * HtmlDocument.cpp
 *
 *  Created on: 2011-7-9
 *      Author: yanbo
 */
#include "HtmlDocument.h"

namespace yanbo
{

HtmlDocument::HtmlDocument()
    : m_title(_CS("yanbo"))
    , m_url(_CS(""))
    , m_root(NULL)
{
}

HtmlDocument::~HtmlDocument()
{	
}

void HtmlDocument::setRenderTreeRoot(HtmlItem* root)
{
    m_root = root;
}

HtmlItem* HtmlDocument::getRenderTreeRoot() const
{
	return m_root;
}

void HtmlDocument::setPageTitle(const StringA& titleText)
{
	m_title = titleText;
}

const StringA& HtmlDocument::getPageTitle() const
{
	return m_title;
}

void HtmlDocument::setPageUrl(const StringA& url)
{
    m_url = url;	
}

const StringA& HtmlDocument::getPageUrl() const
{
	return m_url;
}

// use linear structure to deal with the up, down event draw can enhance efficiency
HtmlItem* HtmlDocument::getPreItem()
{
	HtmlItemList::Iterator iter = m_currentItemIter;
	
	if (--m_currentItemIter == m_itemList.end())
	{
	    m_currentItemIter = iter;
	}
	
	return *m_currentItemIter;
}

HtmlItem* HtmlDocument::getNextItem()
{
	HtmlItemList::Iterator iter = m_currentItemIter;
	
	if (++m_currentItemIter == m_itemList.end())
	{
	    m_currentItemIter = iter;
	}
	
	return *m_currentItemIter;
}

void HtmlDocument::addForm(HtmlForm* form)
{
	m_formList.push_back(form);
}

HtmlItem* HtmlDocument::getCurrentItem()
{
    return *m_currentItemIter;
}

void HtmlDocument::addHtmlItem(HtmlItem* item)
{
	m_itemList.push_back(item);	
}

void HtmlDocument::resetHtmlFocus()
{
	m_currentItemIter = m_itemList.begin();
}
void HtmlDocument::clearHtmlList()
{
	m_itemList.clear();
}

void HtmlDocument::setViewPort(const LayoutRect& viewRect)
{
	m_viewRect = viewRect;
}

const LayoutRect& HtmlDocument::getViewPort() const
{
	return m_viewRect;
}

}

