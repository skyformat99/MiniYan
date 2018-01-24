#include "FontAndroid.h"
#include "AutoObject.h"
#include "SalLog.h"
#include "StringUtils.h"
#include "LGdi.h"

namespace util
{
static FontAndroid* s_font = NULL;
struct JFontAndroid
{
	JNIEnv*   m_env;
	jweak     m_obj;
    jmethodID m_getFontWidth;
    jmethodID m_getFontHeight;
    jmethodID m_getTextWidth;
    jmethodID m_getLineSize;
    jmethodID m_getLineWidth;
    jmethodID m_getLineText;
    jmethodID m_calcTextLine;
    AutoJObject object(JNIEnv* env) {
        return getRealObject(env, m_obj);
    }
};

FontAndroid::FontAndroid()
    : m_privateFont(NULL)
{
	KLOG("FontAndroid::FontAndroid()");
	JNIEnv* env = getJNIEnv();
	jclass clazz = env->FindClass("com/mini/app/core/MiniFont");
    jmethodID constructMethod = env->GetMethodID(clazz, "<init>",
                                           "()V");
    jobject obj = env->NewObject(clazz, constructMethod);
    init(env, obj);
    env->DeleteLocalRef(clazz);
}

void FontAndroid::init(JNIEnv* env, jobject o)
{
	KLOG("FontAndroid::init");
    if (!m_privateFont)
    {
    	m_privateFont = new JFontAndroid;
    }

    KLOG("FontAndroid::init1");
	jclass clazz = env->GetObjectClass(o);
    //jclass clazz = env->FindClass("Mini/app/core/MiniFont");
	KLOG("FontAndroid::init2");
	m_privateFont->m_env = env;
	KLOG("FontAndroid::init3");
	m_privateFont->m_obj = env->NewGlobalRef(o);
	KLOG("FontAndroid::init4");
	m_privateFont->m_getFontWidth = GetJMethod(env, clazz, "getFontWidth", "(Ljava/lang/String;I)I");
	m_privateFont->m_getFontHeight = GetJMethod(env, clazz, "getFontHeight", "(I)I");
	m_privateFont->m_getTextWidth = GetJMethod(env, clazz, "getTextWidth", "(Ljava/lang/String;I)I");
    m_privateFont->m_getLineSize = GetJMethod(env, clazz, "getLineSize", "()I");
    m_privateFont->m_getLineWidth = GetJMethod(env, clazz, "getLineWidth", "(I)I");
    m_privateFont->m_getLineText = GetJMethod(env, clazz, "getLineText", "(I)Ljava/lang/String;");
    m_privateFont->m_calcTextLine = GetJMethod(env, clazz, "calcTextLine", "(Ljava/lang/String;II)I");
}

FontAndroid::~FontAndroid()
{
	if (m_privateFont != NULL)
	{
        m_privateFont->m_env->DeleteGlobalRef(m_privateFont->m_obj);
		delete m_privateFont;
	}

	m_privateFont = NULL;
}

LInt FontAndroid::getFontWidth(LUint8 ch, LInt size) const
{
	KLOG("FontAndroid::getFontWidth");;
    //StringW strW = chW;
    StringA astr = ch;
    //util::StringUtils::strWtoStr(chW, astr);

    JNIEnv* env = m_privateFont->m_env;
    AutoJObject javaObject = m_privateFont->object(env);
    if (!javaObject.get())
        return 0;

    return env->CallIntMethod(javaObject.get(), m_privateFont->m_getFontWidth,
    		strToJstring(env, (const char*)astr.GetBuffer()), size);
}

LInt FontAndroid::getTextWidth(const StringA& text, LInt size) const
{
	KLOG("FontAndroid::getTextWidth");
    //StringA astr;
    //util::StringUtils::strWtoStr(text, astr);
    KLOG("FontAndroid::getTextWidth1");
    JNIEnv* env = m_privateFont->m_env;
    AutoJObject javaObject = m_privateFont->object(env);
    if (!javaObject.get())
    	return 0;

    KLOG("FontAndroid::getTextWidth2");
    return env->CallIntMethod(javaObject.get(), m_privateFont->m_getTextWidth,
    		strToJstring(env, (const char*)text.GetBuffer()), size);
}

LInt FontAndroid::getFontHeight(LInt size) const
{
	KLOG("FontAndroid::getFontHeight");

	if (m_privateFont == NULL)
	{
		KLOG("FontAndroid::getFontHeight0");
        return 0;
	}
    JNIEnv* env = m_privateFont->m_env;
    KLOG("FontAndroid::getFontHeight1");
    AutoJObject javaObject = m_privateFont->object(env);
    KLOG("FontAndroid::getFontHeight2");
    if (!javaObject.get())
        return 0;

    KLOG("FontAndroid::getFontHeight1");
    return env->CallIntMethod(javaObject.get(), m_privateFont->m_getFontHeight, size);
}

LInt FontAndroid::getLineSize() const
{
    if (m_privateFont == NULL)
    {
        KLOG("FontAndroid::getFontHeight0");
        return 0;
    }
    JNIEnv* env = m_privateFont->m_env;
    KLOG("FontAndroid::getFontHeight1");
    AutoJObject javaObject = m_privateFont->object(env);
    KLOG("FontAndroid::getFontHeight2");
    if (!javaObject.get())
        return 0;

    KLOG("FontAndroid::getFontHeight1");
    return env->CallIntMethod(javaObject.get(), m_privateFont->m_getLineSize);
}

LInt FontAndroid::getLineWidth(LInt index) const
{
    if (m_privateFont == NULL)
    {
        KLOG("FontAndroid::getFontHeight0");
        return 0;
    }
    JNIEnv* env = m_privateFont->m_env;
    KLOG("FontAndroid::getFontHeight1");
    AutoJObject javaObject = m_privateFont->object(env);
    KLOG("FontAndroid::getFontHeight2");
    if (!javaObject.get())
        return 0;

    KLOG("FontAndroid::getFontHeight1");
    return env->CallIntMethod(javaObject.get(), m_privateFont->m_getLineWidth, index);
}

StringA FontAndroid::getLineText(LInt index) const
{
    if (m_privateFont == NULL)
    {
        KLOG("FontAndroid::getFontHeight0");
        return (const LUint8*)NULL;
    }
    JNIEnv* env = m_privateFont->m_env;
    KLOG("FontAndroid::getFontHeight1");
    AutoJObject javaObject = m_privateFont->object(env);
    KLOG("FontAndroid::getFontHeight2");
    if (!javaObject.get())
        return (const LUint8*)NULL;;

    KLOG("FontAndroid::getFontHeight1");
    jstring jstr = (jstring) env->CallObjectMethod(javaObject.get(), m_privateFont->m_getLineText, index);
    StringA result = (const LUint8*) jstringTostr(env, jstr);
    return result;
}

LInt FontAndroid::calcTextLine(const StringA& text, LInt maxWidth, LInt fontSize) const
{
    if (m_privateFont == NULL)
    {
        KLOG("FontAndroid::calcTextLine0");
        return 0;
    }
    JNIEnv* env = m_privateFont->m_env;
    KLOG("FontAndroid::calcTextLine1");
    AutoJObject javaObject = m_privateFont->object(env);
    KLOG("FontAndroid::calcTextLine2");
    if (!javaObject.get())
        return 0;

    KLOG("FontAndroid::calcTextLine3");
    return env->CallIntMethod(
            javaObject.get(),
            m_privateFont->m_calcTextLine,
            strToJstring(env, (const char*)text.GetBuffer()),
            maxWidth,
            fontSize
    );
}

class AndroidFont : public LFont
{
public:
    AndroidFont(const LFont& font);
    virtual ~AndroidFont();

    virtual LInt getFontHeight() const;
    virtual LInt getFontWidth(LUint8 ch) const;
    virtual LInt getTextWidth(const StringA& text) const;
    virtual LInt calcTextLine(const StringA& text, LInt maxWidth) const;
    virtual LInt getLineSize() const;
    virtual LInt getLineWidth(LInt index) const;
    virtual StringA getLineText(LInt index) const;
};

AndroidFont::AndroidFont(const LFont& font)
    : LFont(font)
{
    if (!s_font)
    {
        s_font = new FontAndroid();
    }
}

AndroidFont::~AndroidFont()
{
}

LInt AndroidFont::getFontHeight() const
{
    return s_font->getFontHeight(m_size > 0 ? m_size : 12);
}

LInt AndroidFont::getFontWidth(LUint8 ch) const
{
    if (ch == ' ')
    {
        return 10;
    }

    return s_font->getFontWidth(ch, m_size > 0 ? m_size : 12);
}

LInt AndroidFont::getTextWidth(const StringA& text) const
{
    return s_font->getTextWidth(text, m_size > 0 ? m_size : 12);
}

LInt AndroidFont::getLineSize() const
{
    return s_font->getLineSize();
}

LInt AndroidFont::getLineWidth(LInt index) const
{
    return s_font->getLineWidth(index);
}

StringA AndroidFont::getLineText(LInt index) const
{
    return s_font->getLineText(index);
}

LInt AndroidFont::calcTextLine(const StringA& text, LInt maxWidth) const
{
    return s_font->calcTextLine(text, maxWidth, m_size);
}

LFont* LFont::createSalInstance(const LFont& font)
{
    return new AndroidFont(font);
}

}
