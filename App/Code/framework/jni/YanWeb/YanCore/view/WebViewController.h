/*
 * WebViewController.h
 *
 *  Created on: 2011-7-15
 *      Author: yanbo
 */

#ifndef WebViewController_h
#define WebViewController_h

#include "PlatformLib.h"
#include "LGraphic.h"

namespace yanbo
{

class WebView;
class WebViewController
{
public:
	WebViewController(WebView* view);
	~WebViewController();
	
public:
	// Key Event
	void onUpKey(LBool repeated);
	void onLeftKey(LBool repeated);
	void onDownKey(LBool repeated);
	void onRightKey(LBool repeated);
	void onEnterKey(LBool repeated);
	
	// Mouse Event
	void onMousePress(const LPoint& pt);
	void onMouseUp(const LPoint& pt);
	
	// Touch Event
	void onTouchDown(const LPoint& pt);
	void onTouchUp(const LPoint& pt);

protected:
	WebView* m_view;
};

}
#endif /* WEBVIEWCONTROLLER_H_ */
