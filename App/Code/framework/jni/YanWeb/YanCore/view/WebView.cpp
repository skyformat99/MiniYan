/*
 * WebView.cpp
 *
 *  Created on: 2011-7-15
 *      Author: yanbo
 */

#include "WebView.h"
#include "HtmlItem.h"

namespace yanbo
{

WebView* WebView::s_instance = NULL;
WebView::WebView()
    : m_loader(NULL)
    , m_doc(NULL)
    , m_listener(NULL)
	, m_controller(NULL)
{
}

WebView::~WebView()
{
	if (NULL != m_listener)
	{
	    delete m_listener;
	    m_listener = NULL;
	}
	
	if (NULL != m_loader)
	{
	    delete m_loader;
	    m_loader = NULL;
	}
	
	if (NULL != m_doc)
	{
	    delete m_doc;
	    m_doc = NULL;
	}
}

WebView* WebView::getInstance()
{
	if (s_instance == NULL)
	{
		s_instance = new WebView();
	}

	return s_instance;
}

void WebView::distroy()
{
	if (s_instance != NULL)
	{
		delete s_instance;
		s_instance = NULL;
	}
}

void WebView::setClientRange(const LRect& clientRect)
{
	m_clientRect = clientRect;
}

const LRect& WebView::getClientRange() const
{
    return m_clientRect;	
}

void WebView::setComponents(NetWorkBase* httpEngine,  
		util::LGraphicsContext* gc,
		ResourceLoaderClient* client)
{
	if (NULL == m_loader)
	{
	    m_loader = new ResourceLoader(client);
	    m_loader->setGraphicContext(gc);
	    m_loader->setNetWorkInstance(httpEngine);
	    
	    m_listener = new WebViewListener(this);
	    m_doc = new HtmlDocument();
	    m_loader->setView(this);
	    
	    m_controller = new WebViewController(this);
	}

}

void WebView::loadPage(const util::String& url)
{
	if (NULL != m_loader)
	{
	    m_loader->load(url, ResourceLoader::HTMLDOC);
	}
}

HtmlDocument* WebView::getDocument() const
{
	return m_doc;
}

WebViewListener* WebView::getListener() const
{
	return m_listener;
}

void WebView::handleMouseEvent(const util::LMouseEvent& evt)
{
    switch (evt.getType())
    {
	case util::LMouseEvent::MOUSE_PRESS:
		{
			m_controller->onMousePress(evt.getPosition());
		}
		break;
	case util::LMouseEvent::MOUSE_UP:
		{
			m_controller->onMouseUp(evt.getPosition());
		}
		break;
	case util::LMouseEvent::MOUSE_LONG_PRESS:
		{
		}
		break;
	case util::LMouseEvent::MOUSE_SCROLL:
		{
		}
		break;
    }
}

void WebView::handleKeyEvent(const util::LKeyEvent& evt)
{
    switch (evt.getType())
    {
    case util::LKeyEvent::KEY_ARROW_DOWN:
    	{
    	    m_controller->onDownKey(LFalse);
    	}
        break;
    case util::LKeyEvent::KEY_ARROW_UP:
    	{
    	    m_controller->onUpKey(LFalse);
    	}
    	break;
    case util::LKeyEvent::KEY_ENTER:
    	{
    	    m_controller->onEnterKey(LFalse);
    	}
    	break;
    }
}

void WebView::handleTouchEvent(const util::LTouchEvent& evt)
{
	switch (evt.getType())
	{
	case util::LTouchEvent::TOUCH_DOWN:
		m_controller->onTouchDown(evt.getPosition());
		break;
	case util::LTouchEvent::TOUCH_UP:
		m_controller->onTouchUp(evt.getPosition());
		break;
	}
}

ResourceLoader* WebView::getLoader() const
{
    return m_loader;	
}

void WebView::loadString(const StringA& src)
{
	m_loader->loadString(src);
}

}
