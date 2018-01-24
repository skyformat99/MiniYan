#ifndef HtmlRenderer_h
#define HtmlRenderer_h

#include "HtmlDocument.h"
#include "HtmlForm.h"
#include "CssParser.h"
#include "Document.h"

namespace yanbo
{
class ResourceLoader;
class HtmlRenderer
{
public:
	HtmlRenderer();
	~HtmlRenderer();
	
public:
	// layout all html items
	void renderHTML(HtmlDocument* doc,
			const StringA& buffer,
			IItemListener* itemListener,
			const LayoutRect& rect,
			ResourceLoader* loader);

	void layoutRender();
	void relayoutRender();
	void paintRenderer(util::LGraphicsContext& dc);
	
private:
	util::CssParser*                m_cssParser;
	HtmlForm*                       m_form;
	IItemListener*                  m_itemListener;
	HtmlDocument*                   m_htmlDoc;
    util::LRect                     m_rect;
    ResourceLoader*                 m_loader;
    Document*                       m_document;
};

}
#endif
