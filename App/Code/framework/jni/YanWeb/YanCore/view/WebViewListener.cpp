/*
 * WebViewListener.cpp
 *
 *  Created on: 2011-7-18
 *      Author: yanbo
 */

#include "WebViewListener.h"
#include "WebView.h"
#include "SalLog.h"

namespace yanbo
{

WebViewListener::WebViewListener(WebView* view)
	: m_view(view)
{
}

WebViewListener::~WebViewListener()
{
	
}

void WebViewListener::onClick(const String& url, const StringA& script)
{
	KLOG("WebViewListener::onClick");
	KSTRLOG(url);
	m_view->loadPage(url);
}

void WebViewListener::onFormSubmit(const IHtmlForm& form)
{
	
}

}
