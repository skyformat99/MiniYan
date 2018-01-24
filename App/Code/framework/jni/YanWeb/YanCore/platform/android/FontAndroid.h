#ifndef FontAndroid_h
#define FontAndroid_h

#include "UtilString.h"
#include "AutoObject.h"

namespace util
{

struct JFontAndroid;
class FontAndroid
{
public:
	FontAndroid();
	virtual ~FontAndroid();
	void init(JNIEnv* env, jobject o);
	LInt getFontHeight(LInt size) const;
	LInt getFontWidth(LUint8 ch,  LInt size) const;
	LInt getTextWidth(const StringA& text, LInt size) const;
	LInt getLineSize() const;
	LInt getLineWidth(LInt index) const;
	StringA getLineText(LInt index) const;
	LInt calcTextLine(const StringA& text, LInt maxWidth, LInt fontSize) const;

private:
	struct JFontAndroid* m_privateFont;
};

}

#endif
