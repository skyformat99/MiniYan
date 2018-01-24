#include "GraphicsContextGL.h"
#include "ShaderUtil.h"
#include "MatrixState.h"
#include "BitmapAndroid.h"
#include <stdlib.h>
#include <android/bitmap.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace util
{

GraphicsContextGL::GraphicsContextGL()
{
}

GraphicsContextGL::~GraphicsContextGL()
{
}

LVoid GraphicsContextGL::initGLEnvironment(LInt width, LInt height)
{
    //m_textDraw = new BitmapAndroid();
	yanbo::MatrixState::init();
	yanbo::ShaderUtil::setScreenSize(width, height);
    glViewport(0, 0, width, height);
    //LReal32 ratio = (LReal32) width / height;
    //LReal32 ratio = (LReal32) height / width;
    LReal32 ratio = 1.0f;
    // 设置透视投影
	yanbo::MatrixState::setProjectFrustum(-1.0f * ratio, 1.0f * ratio, -1.0f, 1.0f, 1.0f, 50);

	yanbo::MatrixState::setCamera(0,0,1,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
	yanbo::MatrixState::copyMVMatrix();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

LVoid GraphicsContextGL::drawLine(const LPoint& p1, const LPoint& p2)
{
	KRefPtr<yanbo::BaseShape> line = new yanbo::BaseShape();
    line->setLine(p1, p2);
    line->setColor(m_penColor);
    m_shapes.push_back(line);
}

LVoid GraphicsContextGL::drawLine(LInt x0, LInt y0, LInt x1, LInt y1)
{
	drawLine(LPoint(x0, y0), LPoint(x1, y1));
}

LVoid GraphicsContextGL::drawRect(const LRect& aRect)
{
    KRefPtr<yanbo::BaseShape> shapeRect = new yanbo::BaseShape();
	shapeRect->setRect(aRect);
	shapeRect->setColor(m_brushColor);
    m_shapes.push_back(shapeRect);
}

LVoid GraphicsContextGL::drawRect(LInt x, LInt y, LInt w, LInt h)
{
	drawRect(LRect(x, y, w, h));
}

LVoid GraphicsContextGL::drawHollowRect(const LRect& rect)
{

}

LVoid GraphicsContextGL::drawHollowRect(LInt x, LInt y, LInt w, LInt h)
{
	drawLine(x, y, x+w, y);
	drawLine(x, y, x, y+h);
	drawLine(x+w, y, x+w, y+h);
	drawLine(x, y+h, x+w, y+h);
}

LVoid GraphicsContextGL::drawEllipse(const LRect& aRect)
{

}

LVoid GraphicsContextGL::drawRoundRect(const LRect& aRect, const LSize& aCornerSize)
{

}

LVoid GraphicsContextGL::drawText(const StringA& aText, const LRect& aRect)
{

}

LVoid GraphicsContextGL::drawText(const StringA& aText, const LPoint& aPoint)
{

}

LVoid GraphicsContextGL::drawBitmap(const LPoint& aTopLeft, const LBitmap* aBitmap)
{

}

LVoid GraphicsContextGL::drawBitmap(const LRect& aRect, const LBitmap* aBitmap)
{
    const BitmapAndroid* bitmap = static_cast<const BitmapAndroid*>(aBitmap);

    if (bitmap == NULL)
    {
    	return;
    }

    AutoJObject javaBitmap = bitmap->getBitmapObject();
    AndroidBitmapInfo info;
    memset(&info, 0, sizeof(info));
    AndroidBitmap_getInfo(javaBitmap.env(), javaBitmap.get(), &info);

    if (info.width <= 0 || info.height <= 0 ||
            (info.format != ANDROID_BITMAP_FORMAT_RGB_565
            		&& info.format != ANDROID_BITMAP_FORMAT_RGBA_8888))
    {
        return;
    }

    void* pixels = NULL;
    int res = AndroidBitmap_lockPixels(javaBitmap.env(), javaBitmap.get(), &pixels);
    if (pixels == NULL)
    {
        return;
    }

    KLOG("GraphicsContextGL::drawBitmap");

    KRefPtr<yanbo::BaseShape> tex = new yanbo::BaseShape();
    tex->setImage(pixels, aRect, LSize(info.width, info.height));
    m_shapes.push_back(tex);
    AndroidBitmap_unlockPixels(javaBitmap.env(), javaBitmap.get());
    KLOG("GraphicsContextGL::drawBitmap end");
}

LVoid GraphicsContextGL::drawBitmap(const LRect& aDestRect, const LBitmap* aSource, const LRect& aSourceRect)
{

}

LVoid GraphicsContextGL::drawBitmapMasked(const LRect& aDestRect, const LBitmap* aBitmap, const LRect& aSourceRect, const LBitmap *aMaskBitmap, LBool aInvertMask)
{

}

LVoid GraphicsContextGL::drawText(const StringA& text, const LRect& rect, TextAlign align)
{
	KRefPtr<BitmapAndroid> bitmap = new BitmapAndroid();
    bitmap->drawText(text, rect, align, m_font, m_penColor, m_brushColor);
	drawBitmap(rect, bitmap.get());
}

LVoid GraphicsContextGL::setBrushStyle(BrushStyle aBrushStyle)
{

}

LVoid GraphicsContextGL::setPenStyle(PenStyle aPenStyle)
{

}

LVoid GraphicsContextGL::setBrushColor(const LRgb& aColor)
{
    m_brushColor = aColor;
}

LVoid GraphicsContextGL::setPenColor(const LRgb& aColor)
{
	m_penColor = aColor;
}

LVoid GraphicsContextGL::setFont(const LFont& font)
{
	m_font = font;
}

LVoid GraphicsContextGL::reset()
{
	m_shapes.clear();
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

LVoid GraphicsContextGL::submit()
{
    //reset();
	KLOG("GraphicsContextGL::submit()");
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	KList<KRefPtr<yanbo::BaseShape> >::Iterator iter = m_shapes.begin();
	KList<KRefPtr<yanbo::BaseShape> >::Iterator iterEnd = m_shapes.end();
	for (; iter != iterEnd; ++iter)
	{
        (*iter)->drawSelf();
	}
}

}
