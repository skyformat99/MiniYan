/*
 * ResourceLoader.cpp
 *
 *  Created on: 2011-7-15
 *      Author: yanbo
 */
#include "ResourceLoader.h"
#include "WebView.h"
#include "SalLog.h"
#include "ImageItem.h"
#include "StringUtils.h"

namespace yanbo
{

enum CacheImageType
{
    CACHEBMP,
    CACHEJPEG,
    CACHEPNG
};

ResourceLoader::ResourceLoader(ResourceLoaderClient* client)
    : m_network(NULL)
    , m_render(NULL)
    , m_client(client)
{	
}

ResourceLoader::~ResourceLoader()
{
	if (NULL != m_network)
	{
	    delete m_network;
	    m_network = NULL;
	}
	
	if (NULL != m_render)
	{
	    delete m_render;
	    m_render = NULL;
	}

	if (NULL != m_gc)
	{
	    delete m_gc;
	    m_gc = NULL;
	}

	if (NULL != m_network)
	{
	    delete m_network;
	    m_network = NULL;
	}
}

void ResourceLoader::onFileLen(int len)
{
    KLOG("ResourceLoader::onFileLen");	
    KDESLOG(len);
    YanLog("dataLen=%d", len);
}


void ResourceLoader::onDataReceived(const util::StringA& data)
{
	KLOG("ResourceLoader::onDataReceived");
	m_data += data;
	KSTRLOG8(data);
	KLOG("onDataReceived end");
}

void ResourceLoader::onStatusCode(int statusCode)
{
	
}

void ResourceLoader::onRedirectUrl(const util::StringA& redirectUrl)
{
}

void ResourceLoader::onLoadError(int error)
{
	
}

void ResourceLoader::onLoadFinished()
{
	KLOG("ResourceLoader::onLoadFinished()");
	switch (m_type)
	{
	case HTMLDOC:
		{
		    executeDocument();
		}
		break;
	case CACHEJS:
		{
		
		}
		break;
	case CACHECSS:
		{
	
		}
	    break;
	case CACHEIMAGE:
		{
		    //executeImage();
		}
		break;
	case CACHEOTHER:
		{
	
		}
	    break;
	default:
		break;
	}
	
	if (!m_waitList.empty())
	{
	    KLOG("!m_waitList.empty()");
	    loadHtmlItem(*m_waitList.begin());
	}
	else
	{
	    m_currentItem = NULL;
	}
	
	//m_render->renderHTML();
}

void ResourceLoader::setGraphicContext(util::LGraphicsContext* gc)
{
	m_gc = gc;
}

void ResourceLoader::setNetWorkInstance(NetWorkBase* network)
{
	m_network = network;
	if (NULL != m_network)
	{
	    m_network->setNetWorkClient(this);
	}
}

void ResourceLoader::load(const util::String& url,  LoadType type)
{
	m_type = type;

	if (NULL != m_network)
	{
		m_data = (const util::LUint8*)NULL;
		m_network->loadUrl(url);
	}
}


void ResourceLoader::setView(WebView* view)
{
	m_view = view;
}

void ResourceLoader::executeDocument()
{
	KLOG("ResourceLoader::executeDocument()");
//	if(NULL != m_render)
//	{
//	    delete m_render;
//	    m_render = NULL;
//	}
	
	KLOG((const char*)m_data.GetBuffer());
	//KSTRLOG8(m_data);
	if (!m_render)
	{
		m_render = new HtmlRenderer();
	}

	m_render->renderHTML(m_view->getDocument(),
			m_data,
			m_view->getListener(),
			m_view->getClientRange(),
			this);
	
	KLOG("CREATE HtmlRenderer");
	m_render->layoutRender();
	KLOG("CREATE HtmlRenderer1");
	if (NULL != m_client)
	{
		m_client->onHtmlRenderFinished();
	}

	if (!m_gc)
	{
		return;
	}

	m_gc->reset();

	KLOG("paint");
	m_render->paintRenderer(*m_gc);
	m_gc->submit();
	if (NULL != m_client)
	{
	    KLOG("NULL != m_client");
	    m_client->onViewNeedToDraw();
	}
	
}

// Execute external css
void ResourceLoader::executeCss()
{
	
}

// // Execute external JS
void ResourceLoader::executeJavaScript()
{
	
}

void ResourceLoader::executeImage()
{
	KLOG("executeImage begin");
	m_waitList.erase(m_waitList.begin());
	KLOG("erase");
	//util::LBitmap* lb = util::LBitmap::create(m_data);
	KLOG("create");
	//((ImageItem*)m_currentItem)->setImage(lb);
	KLOG("executeImage end");
	
	if (m_render)
	{
		m_gc->reset();
		m_render->layoutRender();
		m_render->paintRenderer(*m_gc);
		
		if (NULL != m_client)
		{
		    m_client->onViewNeedToDraw();
		}
	}
}

void ResourceLoader::executeImageByLocal(const String& url)
{
	m_waitList.erase(m_waitList.begin());
	//util::LBitmap* lb = util::LBitmap::createLocalBitmap(url);
	//((ImageItem*)m_currentItem)->setImage(lb);
	
	if (m_render)
	{
		m_gc->reset();
		m_render->relayoutRender();
		m_render->paintRenderer(*m_gc);

		if (NULL != m_client)
		{
		    m_client->onViewNeedToDraw();
		}
	}
}

void ResourceLoader::executeOtherResource()
{
	
}

void ResourceLoader::repaint()
{
	KLOG("ResourceLoader::repaint()");
	if (NULL != m_gc)
	{
		m_gc->reset();
		m_render->paintRenderer(*m_gc);
		m_gc->submit();
	}

	if (NULL != m_client)
	{
	    m_client->onViewNeedToDraw();
	}
}

void ResourceLoader::addWaitList(HtmlItem* item)
{
	m_waitList.push_back(item);
}

void ResourceLoader::loadHtmlItem(HtmlItem* item)
{
#if 0
	m_currentItem = item;
	if(item->isImage())
	{
	    
	    const util::StringA url = ((ImageItem*)item)->getImageUrl();
	    
	    util::String urlW;
	    util::StringUtils::strTostrW(url, urlW);
	    //executeImageByLocal(_WS("c:\\Data\\SMILEY.BMP"));
	    /*if(urlW.FindNoCase(_WS("file")) == 0)
		{
		    util::String path = urlW;
		    path.ReplaceNoCase(_WS("file:///"), _WS(""));
		    path.ReplaceNoCase(_WS("/"), _WS("\\"));
		    
		    executeImageByLocal(path);
		}
		else
		{
			load(urlW, CACHEIMAGE);
		}	*/	
	    
	}
#endif	


}

void ResourceLoader::loadString(const StringA& src)
{
    m_data = src;
    m_type = HTMLDOC;
    onLoadFinished();
}

}
