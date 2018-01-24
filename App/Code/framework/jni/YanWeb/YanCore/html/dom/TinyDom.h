#ifndef TinyDom_h
#define TinyDom_h

#include "Document.h"
#include "tinyxml.h"
#include "HtmlItem.h"

namespace yanbo
{
class TinyDom : public Document
{
public:
	TinyDom();
	~TinyDom();

	virtual void createDocument(
			const StringA& buffer,
			HtmlDocument* doc,
			util::CssParser* cssParser,
			IItemListener* itemListener);

private:
	HtmlItem* createHtmlItem(TiXmlElement* elem,  TiXmlElement* parentElem, HtmlItem* parent);
	void createRenderTree(TiXmlElement* elem,  TiXmlElement* parentElem, HtmlItem* parent);

private:
	HtmlDocument*      m_htmlDoc;
	util::CssParser*   m_cssParser;
	IItemListener*     m_itemListener;
	TiXmlDocument*     m_tinyDoc;
};
}

#endif
