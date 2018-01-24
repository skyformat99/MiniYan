#ifndef MINI_VIEW_CORE_H
#define MINI_VIEW_CORE_H

#include <jni.h>
#include "WebView.h"
#include "GraphicsContextAndroid.h"
#include "GraphicsContextGL.h"
#include "AutoObject.h"
#include "StringUtils.h"
#include "LoaderAndroid.h"
#include "SalLog.h"
#include "LEvent.h"
#include "MiniThread.h"
#include "rapidjson.h"
#include "json/document.h"
#include "MiniThreadPool.h"
#include "MiniLoaderTask.h"

const char* kBrowserClass = "com/mini/app/core/MiniWebView";

enum TouchEventType
{
	ACTION_ARROW_UP = 19,
    ACTION_ARROW_DOWN = 20,
    ACTION_ENTER = 66,
    ACTION_CENTER = 23,
};
bool JNI_LOG_ON = true;

static void nativeInitWebView(
		JNIEnv* env,
		jobject obj,
		jint w,
		jint h,
		jobject graphicsObject,
		jobject networkObj,
		jboolean enableGL,
		jboolean isDebug)
{
	JNI_LOG_ON = isDebug;

	KLOG("MiniTaskThread::run nativeInitWebView");
//  test http engine
	//yanbo::MiniLoadRequest* request = new yanbo::MiniLoadRequest();
	//request->m_url = "http://m.hao123.com";
	//yanbo::MiniLoaderTask* task = new yanbo::MiniLoaderTask(request);
	//yanbo::MiniThreadPool::getInstance()->sendMiniTask(task);


	char json[100] = "{ \"hello\" : \"hello json world\" }";
	rapidjson::Document d;
	d.Parse<0>(json);

	KLOG((char *)d["hello"].GetString());

    util::LGraphicsContext* aGc = NULL;
    if (!enableGL)
    {
        util::GraphicsContextAndroid* gc = new util::GraphicsContextAndroid();
        gc->initGraphics(env, graphicsObject);
        aGc = gc;
    }
    else
    {
        util::GraphicsContextGL* gc = new util::GraphicsContextGL();
        gc->initGLEnvironment(w, h);
        aGc = gc;
    }

    yanbo::LoaderAndroid* loader = new yanbo::LoaderAndroid();
    loader->initLoader(env, networkObj);
    yanbo::WebView::getInstance()->setClientRange(LRect(0, 0, w, h));
    yanbo::WebView::getInstance()->setComponents(loader, aGc, NULL);
}

static void nativeDistroyWebView(JNIEnv* env, jobject obj)
{
	yanbo::WebView::getInstance()->distroy();
}

static void nativeLoadUrl(JNIEnv* env, jobject obj, jstring s)
{
	char* url = util::jstringTostr(env, s);
	KLOG(url);
	String wurl;

#if 1
	util::StringUtils::strTostrW(_CS(url), wurl);
	yanbo::WebView::getInstance()->loadPage(wurl);
#endif

#if 0
    StringA htmlSource = _CS("<html>"
    		                 "<head><style>"
    		                 "body{background-color:#ffffff;font-size:60px;color:#0000ff;}"
    		                 ".first{border: 1 solid #ff0000;background-color:#00ff00;color:#e32ede;}"
    		                 "</style></head>"
    		                 "<body>"
    		                 "<p>测试 my browser</p>"
    		                 "<p class=\"first\">测试CSS的CLASS</p>"
    		                 "</body></html>");
    yanbo::WebView::getInstance()->loadString(htmlSource);
#endif
}

static void nativeOnDataReceive(JNIEnv* env, jobject obj, jstring s)
{
	char* data = util::jstringTostr(env, s);
	yanbo::WebView::getInstance()->getLoader()->onDataReceived(_CS(data));
}

static void nativeOnDataFinished(JNIEnv* env, jobject obj)
{
	yanbo::WebView::getInstance()->getLoader()->onLoadFinished();
}

static void nativeWebViewDraw(JNIEnv* env, jobject obj)
{
	yanbo::WebView::getInstance()->getLoader()->repaint();
}

static void nativeHandleKeyEvent(JNIEnv* env, jobject obj, jint keyCode, jint isDown)
{

    util::LKeyEvent::KeyEventType mKeyCode = util::LKeyEvent::KEY_ARROW_DOWN;
    switch (keyCode)
    {
    case ACTION_ARROW_UP:
    	 {
    	    mKeyCode = util::LKeyEvent::KEY_ARROW_UP;
    	 }
    	 break;
    case ACTION_ARROW_DOWN:
    	 {
    	    mKeyCode = util::LKeyEvent::KEY_ARROW_DOWN;
    	 }
    	 break;
    }

    util::LKeyEvent evt(mKeyCode, isDown);
    yanbo::WebView::getInstance()->handleKeyEvent(evt);
}

static JNINativeMethod gBrowserMethods[] = {
	{"nativeInitWebView", "(IILcom/mini/app/core/MiniGraphicsContext;Lcom/mini/app/loader/Loader;ZZ)V", (void*)nativeInitWebView},
	{"nativeLoadUrl", "(Ljava/lang/String;)V", (void*)nativeLoadUrl},
	{"nativeOnDataReceive", "(Ljava/lang/String;)V", (void*)nativeOnDataReceive},
	{"nativeOnDataFinished", "()V", (void*)nativeOnDataFinished},
	{"nativeWebViewDraw", "()V", (void*)nativeWebViewDraw},
	{"nativeDistroyWebView", "()V", (void*)nativeDistroyWebView},
	{"nativeHandleKeyEvent", "(II)V", (void*)nativeHandleKeyEvent},
};

extern int registerNativeMethods(JNIEnv* env, const char* className,
        JNINativeMethod* gMethods, int numMethods);

/*
* Register native methods for all classes we know about.
*/
int registerBrowserNatives(JNIEnv* env)
{
	if (!registerNativeMethods(env, kBrowserClass, gBrowserMethods,
                                 sizeof(gBrowserMethods) / sizeof(gBrowserMethods[0])))
	{
		return JNI_FALSE;
	}

	return JNI_TRUE;
}

#endif
