/*
 * HtmlRenderer.cpp
 *
 *  Created on: 2011-6-30
 *      Author: yanbo
 */

#include "HtmlRenderer.h"
#include "ResourceLoader.h"
#include "RenderContext.h"
#include "SalLog.h"

namespace yanbo
{

HtmlRenderer::HtmlRenderer()
    : m_cssParser(NULL)
    , m_form(NULL)
    , m_itemListener(NULL)
    , m_htmlDoc(NULL)
    , m_document(NULL)
{	
}

HtmlRenderer::~HtmlRenderer()
{
	if (NULL != m_document)
	{
		delete m_document;
		m_document = NULL;
	}

	if (NULL != m_cssParser)
	{
	    delete m_cssParser;
	    m_cssParser = NULL;
	}
}

void HtmlRenderer::renderHTML(HtmlDocument* doc,
		const StringA& buffer,
		IItemListener* itemListener,
		const LayoutRect& rect,
		ResourceLoader* loader)
{
	m_htmlDoc = doc;
	m_htmlDoc->clearHtmlList();
	m_itemListener = itemListener;
	m_loader = loader;
	m_rect = rect;

	if (NULL == m_cssParser)
	{
	    m_cssParser = new util::CssParser();
	}

	m_document = Document::create(Document::ETiny2Doc);
	m_document->createDocument(buffer, m_htmlDoc, m_cssParser, itemListener);
	m_htmlDoc->resetHtmlFocus();
}

void HtmlRenderer::layoutRender()
{
	m_htmlDoc->setViewPort(m_rect);
	util::CssManager* cssManager = m_cssParser->getCssManager();
	if (NULL != cssManager)
	{
	    m_htmlDoc->getRenderTreeRoot()->setStyle(cssManager, NULL);
	}

	m_htmlDoc->getRenderTreeRoot()->layout();
}

void HtmlRenderer::relayoutRender()
{
	RenderContext rc;
	rc.setX(m_rect.iTopLeft.iX);
	rc.setY(0);
	//rc.setY(m_rect.iTopLeft.iY);
	rc.setNewLineXStart(m_rect.iTopLeft.iX);
	//rc.setNewLineXStart(100);
	rc.setMaxWidth(m_rect.GetWidth());
	rc.setMaxHeight(m_rect.GetHeight());
	m_htmlDoc->setViewPort(m_rect);
	m_htmlDoc->getRenderTreeRoot()->layout(rc);
}

void HtmlRenderer::paintRenderer(util::LGraphicsContext& gc)
{
	m_htmlDoc->getRenderTreeRoot()->paint(gc);
}

}
