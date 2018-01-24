/*
 * LoaderAndroid.cpp
 *
 *  Created on: 2014-2-23
 *      Author: yanbo
 */

#include "LoaderAndroid.h"
#include "AutoObject.h"
#include "StringUtils.h"
#include "SalLog.h"

namespace yanbo
{

struct JLoader
{
	JNIEnv*   m_env;
	jweak     m_obj;
	jmethodID m_loadURL;
    util::AutoJObject object(JNIEnv* env) {
        return util::getRealObject(env, m_obj);
    }
};

LoaderAndroid::LoaderAndroid()
    : m_privateLoader(NULL)
{	
}

LoaderAndroid::~LoaderAndroid()
{
	if (m_privateLoader != NULL)
	{
		delete m_privateLoader;
		m_privateLoader = NULL;
	}
}

void LoaderAndroid::initLoader(JNIEnv* env, jobject o)
{
	m_privateLoader = new JLoader;
    jclass clazz = env->GetObjectClass(o);
    m_privateLoader->m_env = env;
    m_privateLoader->m_obj = env->NewWeakGlobalRef(o);
    m_privateLoader->m_loadURL = util::GetJMethod(env, clazz, "load", "(Ljava/lang/String;)V");
}

void LoaderAndroid::loadUrl(const String& url)
{
	StringA aurl;
	util::StringUtils::strWtoStr(url, aurl);

	KLOG((const char*)aurl.GetBuffer());
    JNIEnv* env = m_privateLoader->m_env;
    util::AutoJObject javaObject = m_privateLoader->object(env);
    //aurl = _CS("http://wap.baidu.com/");
    KFORMATLOG("url=%s", (const char*)aurl.GetBuffer());
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_privateLoader->m_loadURL,
    		util::strToJstring(env, (const char*)aurl.GetBuffer()));
}

void LoaderAndroid::postData(const ParamMap& params)
{

}

void LoaderAndroid::cancel()
{

}

}
