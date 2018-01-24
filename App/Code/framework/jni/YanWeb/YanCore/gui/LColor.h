#ifndef LColor_h
#define LColor_h

#include "LGdi.h"

namespace util
{

#define COLOR_WHITE                     (LInt)0xffffff
#define COLOR_GRAY                      (LInt)0x808080
#define COLOR_DARKGRAY                  (LInt)0x404040
#define COLOR_BLACK                     (LInt)0x000000
#define COLOR_RED                       (LInt)0xff0000
#define COLOR_PINK                      (LInt)0xffafaf
#define COLOR_ORANGE                    (LInt)0xffc800
#define COLOR_YELLOW                    (LInt)0xffff00
#define COLOR_GREEN                     (LInt)0x00ff00
#define COLOR_MAGENTA                   (LInt)0xff00ff
#define COLOR_CYAN                      (LInt)0x00ffff
#define COLOR_BLUE                      (LInt)0x0000ff
#define COLOR_LINKCOLOR                 COLOR_BLUE // (LInt)0x005bae
#define COLOR_RADIOGREEN                (LInt)0x169133
#define COLOR_LIGHTGRAY                 (LInt)0xc0c0c0

class LColor
{
public:
	static LInt rgb(LInt red, LInt green, LInt blue, LInt alpha = 255);
	static LInt parseRgbString(const StringA& rgbString);
	static LRgb parseRgbInt(LInt rgbValue);
	static LRgb parseArgbInt(LInt argbValue);
};

}
#endif
