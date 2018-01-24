#ifndef GraphicsContextGL_h
#define GraphicsContextGL_h

#include "LGdi.h"
#include "KList.h"
#include "BaseShape.h"
#include "KRefPtr.h"

namespace util
{
class GraphicsContextGL : public LGraphicsContext
{
public:
	GraphicsContextGL();
    virtual ~GraphicsContextGL();

public:
    LVoid initGLEnvironment(LInt width, LInt height);

	virtual LVoid drawLine(const LPoint& p1, const LPoint& p2);
	virtual LVoid drawLine(LInt x0, LInt y0, LInt x1, LInt y1);
	virtual LVoid drawRect(const LRect& aRect);
	virtual LVoid drawRect(LInt x, LInt y, LInt w, LInt h);
	virtual LVoid drawHollowRect(const LRect& rect);
	virtual LVoid drawHollowRect(LInt x, LInt y, LInt w, LInt h);

	virtual LVoid drawEllipse(const LRect& aRect);

	virtual LVoid drawRoundRect(const LRect& aRect, const LSize& aCornerSize);

	virtual LVoid drawText(const StringA& aText, const LRect& aRect);
	virtual LVoid drawText(const StringA& aText, const LPoint& aPoint);

	virtual LVoid drawBitmap(const LPoint& aTopLeft, const LBitmap* aBitmap);
	virtual LVoid drawBitmap(const LRect& aRect, const LBitmap* aBitmap);
	virtual LVoid drawBitmap(const LRect& aDestRect, const LBitmap* aSource, const LRect& aSourceRect);

	virtual LVoid drawBitmapMasked(const LRect& aDestRect, const LBitmap* aBitmap, const LRect& aSourceRect, const LBitmap *aMaskBitmap, LBool aInvertMask);
	virtual LVoid drawText(const StringA& text, const LRect& rect, TextAlign align);
	virtual LVoid setBrushStyle(BrushStyle aBrushStyle);
	virtual LVoid setPenStyle(PenStyle aPenStyle);

	virtual LVoid setBrushColor(const LRgb& aColor);
	virtual LVoid setPenColor(const LRgb& aColor);
	virtual LVoid setFont(const LFont& font);
	virtual LVoid reset();

	virtual LVoid submit();

private:
	KList<KRefPtr<yanbo::BaseShape> > m_shapes;
	LRgb m_brushColor;
	LRgb m_penColor;
	//KRefPtr<BitmapAndroid> m_textDraw;

	LFont m_font;
};

}
#endif
