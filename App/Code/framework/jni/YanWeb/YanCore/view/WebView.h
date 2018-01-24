/*
 * WebView.h
 *
 *  Created on: 2011-7-15
 *      Author: yanbo
 */

#ifndef WebView_h
#define WebView_h

#include "LGdi.h"
#include "ResourceLoader.h"
#include "HtmlDocument.h"
#include "WebViewListener.h"
#include "ResourceLoaderClient.h"
#include "LEvent.h"
#include "WebViewController.h"

namespace yanbo
{

class WebView
{
public:
	static WebView* getInstance();
	void distroy();
	virtual ~WebView();
	
public:
	void setClientRange(const LRect& clientRect);// set client rect
	const LRect& getClientRange() const;
	// setting network and graphic components int webview
	void setComponents(NetWorkBase* httpEngine, 
			util::LGraphicsContext* gc, 
			ResourceLoaderClient* client);

	void loadPage(const String& url);
	void loadString(const StringA& src);
	HtmlDocument* getDocument() const;
	ResourceLoader* getLoader() const;
	WebViewListener* getListener() const;
	
	void handleMouseEvent(const util::LMouseEvent& evt);
	void handleKeyEvent(const util::LKeyEvent& evt);
	void handleTouchEvent(const util::LTouchEvent& evt);
	
private:
	WebView();

private:
	ResourceLoader*            m_loader;
	HtmlDocument*              m_doc;
	LRect                      m_clientRect;
	WebViewListener*           m_listener;
	WebViewController*         m_controller;

	static WebView*            s_instance;
};

}
#endif
