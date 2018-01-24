/*
 * GraphicsContextAndroid.cpp
 *
 *  Created on: 2012-5-10
 *      Author: yanbo
 *  2d graphic api instance by java
 */


#include "GraphicsContextAndroid.h"
#include "LColor.h"
#include "AutoObject.h"
#include "StringUtils.h"
#include "SalLog.h"
#include "BitmapAndroid.h"
#include "FontAndroid.h"
#include <android/log.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

namespace util
{

struct JGraphicsContext
{
	JNIEnv*   m_env;
	jweak     m_obj;
	jmethodID m_drawLineI4;
	jmethodID m_drawLineP2;
	jmethodID m_drawRectI4;
	jmethodID m_drawRectR;
	jmethodID m_drawTextSP;
	jmethodID m_drawTextSR;
	jmethodID m_setBrushColorI;
	jmethodID m_setPenColorI;
	jmethodID m_setBrushStyleI;
	jmethodID m_setPenStyleI;
    jmethodID m_setFontI;
    jmethodID m_drawBitmap;
    jmethodID m_drawJavaBitmap;
    AutoJObject object(JNIEnv* env)
    {
        return getRealObject(env, m_obj);
    }
};

void GraphicsContextAndroid::initGraphics(JNIEnv* env, jobject o)
{
	m_privateGc = new JGraphicsContext;
    jclass clazz = env->GetObjectClass(o);
    m_privateGc->m_env = env;
    m_privateGc->m_obj = env->NewGlobalRef(o);
    m_privateGc->m_drawLineI4 = GetJMethod(env, clazz, "drawLine", "(IIII)V");
    m_privateGc->m_drawRectI4 = GetJMethod(env, clazz, "drawLine", "(IIII)V");
    m_privateGc->m_drawTextSP = GetJMethod(env, clazz, "drawText", "(Ljava/lang/String;II)V");
    m_privateGc->m_drawTextSR = GetJMethod(env, clazz, "drawText", "(Ljava/lang/String;IIIII)V");
    m_privateGc->m_drawRectI4 = GetJMethod(env, clazz, "drawRect", "(IIII)V");
    m_privateGc->m_setBrushColorI = GetJMethod(env, clazz, "setBrushColor", "(I)V");
    m_privateGc->m_setPenColorI = GetJMethod(env, clazz, "setPenColor", "(I)V");
    m_privateGc->m_setPenStyleI = GetJMethod(env, clazz, "setPenStyle", "(I)V");
    m_privateGc->m_setBrushStyleI = GetJMethod(env, clazz, "setBrushStyle", "(I)V");
    m_privateGc->m_setFontI = GetJMethod(env, clazz, "setFont", "(II)V");
    m_privateGc->m_drawBitmap = GetJMethod(env, clazz, "drawBitmap", "(Lcom/mini/app/core/MiniBitmap;IIII)V");
    m_privateGc->m_drawJavaBitmap = GetJMethod(env, clazz, "drawBitmap", "(Landroid/graphics/Bitmap;IIIII)V");
}


GraphicsContextAndroid::GraphicsContextAndroid()
    : m_privateGc(NULL)
{
}

GraphicsContextAndroid::~GraphicsContextAndroid()
{
	if (m_privateGc != NULL)
	{
		m_privateGc->m_env->DeleteGlobalRef(m_privateGc->m_obj);
		delete m_privateGc;
		m_privateGc = NULL;
	}
}

void GraphicsContextAndroid::reset()
{

}

LVoid GraphicsContextAndroid::drawLine(const LPoint& p1, const LPoint& p2)
{
    JNIEnv* env = m_privateGc->m_env;
    AutoJObject javaObject = m_privateGc->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_privateGc->m_drawLineI4,
            p1.iX, p1.iY, p2.iX, p2.iY);
}

LVoid GraphicsContextAndroid::drawLine(LInt x0, LInt y0, LInt x1, LInt y1)
{
    JNIEnv* env = m_privateGc->m_env;
    AutoJObject javaObject = m_privateGc->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_privateGc->m_drawLineI4,
            x0, y0, x1, y1);
}

LVoid GraphicsContextAndroid::drawRect(const LRect& aRect)
{
    JNIEnv* env = m_privateGc->m_env;
    AutoJObject javaObject = m_privateGc->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_privateGc->m_drawRectI4,
            aRect.iTopLeft.iX, aRect.iTopLeft.iY, aRect.GetWidth(), aRect.GetHeight());
}

LVoid GraphicsContextAndroid::drawRect(LInt x, LInt y, LInt w, LInt h)
{
    JNIEnv* env = m_privateGc->m_env;
    AutoJObject javaObject = m_privateGc->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_privateGc->m_drawRectI4,
            x, y, w, h);
}

LVoid GraphicsContextAndroid::drawEllipse(const LRect& aRect)
{
}

LVoid GraphicsContextAndroid::drawRoundRect(const LRect& aRect, const LSize& aCornerSize)
{
}

LVoid GraphicsContextAndroid::drawText(const StringA& aText, const LRect& aRect)
{
    JNIEnv* env = m_privateGc->m_env;
    AutoJObject javaObject = m_privateGc->object(env);
    if (!javaObject.get())
        return;

    //StringA astr;
    //util::StringUtils::strWtoStr(aText, astr);
    env->CallVoidMethod(javaObject.get(), m_privateGc->m_drawTextSP,
    		strToJstring(env, (const char*)aText.GetBuffer()), aRect.iTopLeft.iX, aRect.iTopLeft.iY);
}

LVoid GraphicsContextAndroid::drawText(const StringA& aText, const LPoint& aPoint)
{
    JNIEnv* env = m_privateGc->m_env;
    AutoJObject javaObject = m_privateGc->object(env);
    if (!javaObject.get())
        return;

    //StringA astr;
    //util::StringUtils::strWtoStr(aText, astr);
    env->CallVoidMethod(javaObject.get(), m_privateGc->m_drawTextSR,
    		strToJstring(env, (const char*)aText.GetBuffer()), aPoint.iX, aPoint.iY);
}

LVoid GraphicsContextAndroid::drawBitmap(const LPoint& aTopLeft, const LBitmap* aBitmap)
{
}

LVoid GraphicsContextAndroid::drawBitmap(const LRect& aRect, const LBitmap* aBitmap)
{
    JNIEnv* env = m_privateGc->m_env;
    //JNIEnv* env = getJNIEnv();
    AutoJObject javaObject = m_privateGc->object(env);
    if (!javaObject.get())
        return;

    KLOG("GraphicsContextAndroid::drawBitmap");
    const BitmapAndroid* bitmap = static_cast<const BitmapAndroid*>(aBitmap);
    AutoJObject javaBitmap = bitmap->getJavaBitmap();
    KLOG("GraphicsContextAndroid::drawBitmap1");

    ////// test ////////
//	jclass clazz = env->FindClass("Mini/app/core/MiniBitmap");
//    jmethodID constructMethod = env->GetMethodID(clazz, "<init>",
//                                           "()V");
//
//    jobject obj = env->NewObject(clazz, constructMethod);
    ///// test end //////

    env->CallVoidMethod(javaObject.get(), m_privateGc->m_drawBitmap,
    		javaBitmap.get(),
    		aRect.iTopLeft.iX,
    		aRect.iTopLeft.iY,
    		aRect.GetWidth(),
    		aRect.GetHeight()
    		);

    KLOG("GraphicsContextAndroid::drawBitmap2");
}

LVoid GraphicsContextAndroid::drawBitmap(const LRect& aDestRect, const LBitmap* aSource, const LRect& aSourceRect)
{

}

LVoid GraphicsContextAndroid::drawBitmapMasked(const LRect& aDestRect, const LBitmap* aBitmap, const LRect& aSourceRect, const LBitmap *aMaskBitmap, LBool aInvertMask)
{
}

LVoid GraphicsContextAndroid::setBrushStyle(BrushStyle aBrushStyle)
{
    JNIEnv* env = m_privateGc->m_env;
    AutoJObject javaObject = m_privateGc->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_privateGc->m_setBrushStyleI,
            (int)aBrushStyle);

}

LVoid GraphicsContextAndroid::setPenStyle(PenStyle aPenStyle)
{
    JNIEnv* env = m_privateGc->m_env;
    AutoJObject javaObject = m_privateGc->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_privateGc->m_setPenStyleI,
            (int)aPenStyle);

}

LVoid GraphicsContextAndroid::setBrushColor(const LRgb& aColor)
{
    JNIEnv* env = m_privateGc->m_env;
    AutoJObject javaObject = m_privateGc->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_privateGc->m_setBrushColorI,
            LColor::rgb(aColor.m_red, aColor.m_green, aColor.m_blue));
}

LVoid GraphicsContextAndroid::setPenColor(const LRgb& aColor)
{
    JNIEnv* env = m_privateGc->m_env;
    AutoJObject javaObject = m_privateGc->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_privateGc->m_setPenColorI,
            LColor::rgb(aColor.m_red, aColor.m_green, aColor.m_blue));
}

LVoid GraphicsContextAndroid::setFont(const LFont& font)
{
    JNIEnv* env = m_privateGc->m_env;
    AutoJObject javaObject = m_privateGc->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_privateGc->m_setFontI,
            font.getFontSize(), font.getFontStyle());
}

LVoid GraphicsContextAndroid::drawText(const StringA& text, const LRect& rect, TextAlign align)
{
    JNIEnv* env = m_privateGc->m_env;
    AutoJObject javaObject = m_privateGc->object(env);
    if (!javaObject.get())
        return;

    //StringA astr;
    //util::StringUtils::strWtoStr(text, astr);
    KFORMATLOG("GraphicsContextAndroid_drawText string=%s", (const char*)text.GetBuffer());
    env->CallVoidMethod(javaObject.get(), m_privateGc->m_drawTextSR,
    		strToJstring(env, (const char*)text.GetBuffer()),
    		rect.iTopLeft.iX,
    		rect.iTopLeft.iY,
    		rect.iBottomRight.iX,
    		rect.iBottomRight.iY,
    		(jint)align);

}

LVoid GraphicsContextAndroid::drawHollowRect(const LRect& rect)
{

}

LVoid GraphicsContextAndroid::drawHollowRect(LInt x, LInt y, LInt w, LInt h)
{
	drawLine(x, y, x+w, y);
	drawLine(x, y, x, y+h);
	drawLine(x+w, y, x+w, y+h);
	drawLine(x, y+h, x+w, y+h);
}

LVoid GraphicsContextAndroid::submit()
{
}

}
