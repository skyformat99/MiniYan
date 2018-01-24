/*
 * ResourceLoader.h
 *
 *  Created on: 2011-6-23
 *      Author: yanbo
 */

#ifndef ResourceLoader_h
#define ResourceLoader_h

#include "NetWorkBase.h"
#include "HtmlRenderer.h"
#include "ResourceLoaderClient.h"
namespace yanbo
{
typedef util::KList<HtmlItem*> ItemWaitList;
class WebView;
class ResourceLoader : public NetWorkClient
{
public:
	enum LoadType
	{
	    HTMLDOC,
	    CACHEJS,
	    CACHECSS,
	    CACHEIMAGE,
	    CACHEOTHER
	};
	
public:
	ResourceLoader(ResourceLoaderClient* client);
	virtual ~ResourceLoader();
	
public:
	virtual void onDataReceived(const StringA& data);
	virtual void onStatusCode(LInt statusCode);
	virtual void onRedirectUrl(const StringA& redirectUrl);
	virtual void onLoadError(int error);
	virtual void onLoadFinished();
	virtual void onFileLen(int len);
	
public:
	void setNetWorkInstance(NetWorkBase* network);
	void setGraphicContext(util::LGraphicsContext* gc);
	void load(const String& url, LoadType type);
	void loadString(const StringA& src);
	void setView(WebView* view);
	void repaint();
	void addWaitList(HtmlItem* item);
	
	void executeDocument();
	void executeCss();
	void executeJavaScript();
	void executeImage();
	void executeImageByLocal(const String& url);
	void executeOtherResource();
	
private:
	void loadHtmlItem(HtmlItem* item);
	
protected:
	NetWorkBase*                      m_network;
	HtmlRenderer*                     m_render;
	util::StringA                     m_data;
	WebView*                          m_view;
	LoadType                          m_type;
	ResourceLoaderClient*             m_client;
	util::LGraphicsContext*           m_gc;
	ItemWaitList                      m_waitList;
	HtmlItem*                         m_currentItem;
};

}
#endif /* RESOURCELOADER_H_ */
