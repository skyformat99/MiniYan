/*
 * Color.cpp
 *
 *  Created on: 2011-6-22
 *      Author: yanbo
 */

#include "LColor.h"
#include "StringUtils.h"

namespace util
{

LInt LColor::rgb(LInt red, LInt green, LInt blue, LInt alpha)
{
	if (red < 0 || red > 255 ||
			green < 0 || green > 255 ||
			blue < 0 || blue > 255 ||
			alpha < 0 || alpha > 255)
	{
	    return 0;
	}

	//return ((alpha << 24) | (red << 16) | (green << 8) | blue);

	return (red << 16) | (green << 8) | blue;
}

LRgb LColor::parseRgbInt(LInt rgbValue)
{
	LInt r = LGetRValue(rgbValue);
	LInt g = LGetGValue(rgbValue);
	LInt b = LGetBValue(rgbValue);
	return LRgb(r, g, b);
}

LRgb LColor::parseArgbInt(LInt argbValue)
{
	LInt a = (argbValue >> 24) & 255;
	LInt r = (argbValue >> 16) & 255;
	LInt g = (argbValue >> 8) & 255;
	LInt b = argbValue & 255;
	return LRgb(r, g, b, a);
}

LInt LColor::parseRgbString(const StringA& rgbString)
{
	LInt r = StringUtils::stringToInt(rgbString.Mid(1, 2), 16);
	LInt g = StringUtils::stringToInt(rgbString.Mid(3, 2), 16);
	LInt b = StringUtils::stringToInt(rgbString.Mid(5, 2), 16);
	return rgb(r,g,b);
}

}
