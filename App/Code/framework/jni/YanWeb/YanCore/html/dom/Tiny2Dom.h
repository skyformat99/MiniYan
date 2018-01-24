#ifndef Tiny2Dom_h
#define Tiny2Dom_h

#include "Document.h"
#include "tinyxml2.h"
#include "HtmlItem.h"

using namespace tinyxml2;

namespace yanbo
{
class Tiny2Dom : public Document
{
public:
	Tiny2Dom();
	~Tiny2Dom();

	virtual void createDocument(
			const StringA& buffer,
			HtmlDocument* doc,
			util::CssParser* cssParser,
			IItemListener* itemListener);

private:
	HtmlItem* createHtmlItem(XMLNode* node, XMLNode* parentElem, HtmlItem* parent);
	void createRenderTree(XMLNode* elem, XMLNode* parentElem, HtmlItem* parent);

private:
	HtmlDocument*      m_htmlDoc;
	util::CssParser*   m_cssParser;
	IItemListener*     m_itemListener;
};
}

#endif
