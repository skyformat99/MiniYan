#include "BitmapAndroid.h"
#include "AutoObject.h"
#include "SalLog.h"
#include "StringUtils.h"
#include "LColor.h"
#include "LGdi.h"

namespace util
{

struct JBitmapAndroid
{
	JNIEnv*   m_env;
	jweak     m_obj;
    jmethodID m_loadImage;
    jmethodID m_getWidth;
    jmethodID m_getHeight;
    jmethodID m_getBitmap;
    jmethodID m_drawText;
	jmethodID m_recycle;
    AutoJObject object(JNIEnv* env)
    {
        return getRealObject(env, m_obj);
    }
};

BitmapAndroid::BitmapAndroid()
    : m_privateBitmap(NULL)
{
	KLOG("BitmapAndroid::BitmapAndroid()");
	JNIEnv* env = getJNIEnv();
	KLOG("BitmapAndroid::BitmapAndroid()0");
	jclass clazz = env->FindClass("com/mini/app/core/MiniBitmap");
	KLOG("BitmapAndroid::BitmapAndroid()1");
    jmethodID constructMethod = env->GetMethodID(clazz, "<init>",
                                           "()V");

    KLOG("BitmapAndroid::BitmapAndroid()5");
    jobject obj = env->NewObject(clazz, constructMethod);

    init(env, obj);

	env->DeleteLocalRef(clazz);
}

void BitmapAndroid::init(JNIEnv* env, jobject o)
{
	KLOG("BitmapAndroid::init");
    if (!m_privateBitmap)
    {
    	m_privateBitmap = new JBitmapAndroid;
    }

    KLOG("BitmapAndroid::init1");
	jclass clazz = env->GetObjectClass(o);
	m_privateBitmap->m_env = env;
	m_privateBitmap->m_obj = env->NewGlobalRef(o);
	m_privateBitmap->m_loadImage = GetJMethod(env, clazz, "loadImage", "(Ljava/lang/String;)V");
	m_privateBitmap->m_getWidth = GetJMethod(env, clazz, "getWidth", "()I");
	m_privateBitmap->m_getHeight = GetJMethod(env, clazz, "getHeight", "()I");
	m_privateBitmap->m_getBitmap = GetJMethod(env, clazz, "getBitmap", "()Landroid/graphics/Bitmap;");
	m_privateBitmap->m_drawText  = GetJMethod(env, clazz, "drawText", "(Ljava/lang/String;IIIIIII)V");
	m_privateBitmap->m_recycle  = GetJMethod(env, clazz, "recycle", "()V");
}

BitmapAndroid::~BitmapAndroid()
{
	if (m_privateBitmap != NULL)
	{
		JNIEnv* env = m_privateBitmap->m_env;
//		AutoJObject javaObject = m_privateBitmap->object(env);
//		if (!javaObject.get())
//			return;

		env->CallVoidMethod(
				m_privateBitmap->m_obj,
				m_privateBitmap->m_recycle
		);

		env->DeleteGlobalRef(m_privateBitmap->m_obj);
		delete m_privateBitmap;
	}

	m_privateBitmap = NULL;
}



LVoid BitmapAndroid::load(const StringA& aPath, LInt32 aId)
{
	KLOG("BitmapAndroid::load");
    JNIEnv* env = m_privateBitmap->m_env;
    AutoJObject javaObject = m_privateBitmap->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_privateBitmap->m_loadImage,
    		strToJstring(env, (const char*)aPath.GetBuffer()));
}

LSize BitmapAndroid::size()
{
	KLOG("BitmapAndroid::size()");
    JNIEnv* env = m_privateBitmap->m_env;
    AutoJObject javaObject = m_privateBitmap->object(env);
    if (!javaObject.get())
    {
        return LSize(0, 0);
    }

    if (m_privateBitmap != NULL)
    {
    	int width = env->CallIntMethod(javaObject.get(), m_privateBitmap->m_getWidth);
    	int height = env->CallIntMethod(javaObject.get(), m_privateBitmap->m_getHeight);

    	return LSize(width, height);
    }

    return LSize(0, 0);
}

LBitmap* LBitmap::create()
{
	return new BitmapAndroid();
}

AutoJObject BitmapAndroid::getJavaBitmap() const
{
    JNIEnv* env = m_privateBitmap->m_env;
    return m_privateBitmap->object(env);
}

AutoJObject BitmapAndroid::getBitmapObject() const
{
	KLOG("BitmapAndroid::getBitmapObject begin");
    JNIEnv* env = m_privateBitmap->m_env;
    AutoJObject javaObject = m_privateBitmap->object(env);
    KLOG("BitmapAndroid::getBitmapObject end1");
    jobject jobj = env->CallObjectMethod(javaObject.get(), m_privateBitmap->m_getBitmap);
    KLOG("BitmapAndroid::getBitmapObject end");
    return getRealObject(env, jobj);
}

void BitmapAndroid::drawText(const StringA& text,
		const LRect& rect,
		LGraphicsContext::TextAlign align,
		const LFont& font,
		const LRgb& penColor,
		const LRgb& brushColor)
{
    JNIEnv* env = m_privateBitmap->m_env;
    AutoJObject javaObject = m_privateBitmap->object(env);
    if (!javaObject.get())
        return;

    //StringA astr;
    //util::StringUtils::strWtoStr(text, astr);

    env->CallVoidMethod(
    		javaObject.get(),
    		m_privateBitmap->m_drawText,
    		strToJstring(env, (const char*)text.GetBuffer()),
    		rect.GetWidth(),
    		rect.GetHeight(),
    		(jint)align,
    		font.getFontSize(),
    		(jint)LColor::rgb(penColor.m_red, penColor.m_green, penColor.m_blue),
    		(jint)font.getFontStyle(),
    		(jint)LColor::rgb(brushColor.m_red, brushColor.m_green, brushColor.m_blue)
    	);
}

}
