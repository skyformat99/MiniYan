#ifndef BITMAP_ANDROID_H
#define BITMAP_ANDROID_H

#include "LGdi.h"
#include "AutoObject.h"

namespace util
{

struct JBitmapAndroid;
class BitmapAndroid : public LBitmap
{
public:
	BitmapAndroid();
	virtual ~BitmapAndroid();

public:
	void init(JNIEnv* env, jobject o);
	virtual LVoid load(const StringA& aPath, LInt32 aId);
    virtual LSize size();
    void drawText(
    		const StringA& text,
    		const LRect& rect,
    		LGraphicsContext::TextAlign align,
    		const LFont& font,
    		const LRgb& penColor,
    		const LRgb& brushColor);

    AutoJObject getJavaBitmap() const;
    AutoJObject getBitmapObject() const;

private:
    struct JBitmapAndroid* m_privateBitmap;
};
}

#endif
