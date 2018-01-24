/*
 * ExpatDom.h
 *
 *  Created on: 2015-3-9
 *      Author: yanbo
 */

#ifndef ExpatDom_h
#define ExpatDom_h

#include "Document.h"
#include "HtmlItem.h"

namespace yanbo
{
class ExpatDom : public Document
{
public:
	ExpatDom();
	~ExpatDom();

	virtual void createDocument(
			const StringA& buffer,
			HtmlDocument* doc,
			util::CssParser* cssParser,
			IItemListener* itemListener);

	HtmlItem* createHtmlItem(
			int type,
			const char* name,
			const char **atts,
			HtmlItem* parent);

	HtmlItem* createTextItem(const char* text, int len);

	Stack<HtmlItem*>* getItemStack() const;
	void appendText(const CString& text);
	void clearText();
	const CString& getText() const;

private:
	void getItemAttribute(
			StringA& sAttrValue,
			const char* cAttrName,
			const char** cAttr);

private:
	HtmlDocument*      m_htmlDoc;
	util::CssParser*   m_cssParser;
	IItemListener*     m_itemListener;

	Stack<HtmlItem*>*  m_itemStack;
	CString            m_text;
};
}

#endif
