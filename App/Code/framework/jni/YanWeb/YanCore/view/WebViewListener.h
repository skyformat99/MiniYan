/*
 * WebViewListener.h
 *
 *  Created on: 2011-7-18
 *      Author: yanbo
 */

#ifndef WebViewListener_h
#define WebViewListener_h

#include "IItemListener.h"
#include "IHtmlForm.h"

namespace yanbo
{
// Event Action Listener
class WebView;
class WebViewListener : public IItemListener
{
public:
	WebViewListener(WebView* view);
	~WebViewListener();
	
public:
	virtual void onClick(const String& url, const StringA& script);
	virtual void onFormSubmit(const IHtmlForm& form);
	
private:
	WebView* m_view;
};

}
#endif /* WEBVIEWLISTENER_H_ */
