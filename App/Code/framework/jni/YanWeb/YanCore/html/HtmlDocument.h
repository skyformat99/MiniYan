/*
 * HtmlDocument.h
 *
 *  Created on: 2011-6-22
 *      Author: yanbo
 */

#ifndef HtmlDocument_h
#define HtmlDocument_h

#include "String.h"
#include "HtmlItem.h"
#include "HtmlForm.h"
namespace yanbo
{

typedef KList<HtmlForm*> FormList;
class HtmlDocument
{
public:
	HtmlDocument();
	~HtmlDocument();

public:
	void setRenderTreeRoot(HtmlItem* root);
	HtmlItem* getRenderTreeRoot() const;
	
	void setPageTitle(const StringA& titleText);
	const StringA& getPageTitle() const;
	
	void setPageUrl(const StringA& url);
	const StringA& getPageUrl() const;
	
	HtmlItem* getPreItem();
	HtmlItem* getNextItem();
	
	HtmlItem* getCurrentItem();
	
	void addForm(HtmlForm* form);
	
	void addHtmlItem(HtmlItem* item);
	
	void resetHtmlFocus();
	void clearHtmlList();

	void setViewPort(const LayoutRect& viewRect);
	const LayoutRect& getViewPort() const;
	
private:
    /**+
     * The URL of this page.
     */
    StringA                       m_url;
	
    /**
     *  The title text of this page.
     */
	StringA                       m_title;
	
	HtmlItem*                     m_root;
	
//	HtmlItem*                     m_currentItem;
	
	FormList                      m_formList;   
	HtmlItemList                  m_itemList;
	HtmlItemList::Iterator        m_currentItemIter; // control up, down key event display
	LayoutRect                    m_viewRect;
};

}
#endif /* HTMLDOCUMENT_H_ */
