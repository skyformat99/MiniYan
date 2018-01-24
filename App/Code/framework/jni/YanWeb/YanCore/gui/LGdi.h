/*
============================================================================
 Name        : lgdi.h
 Author      : yanbo
 Version     :
 Copyright   : Your copyright notice
 Description : lgdi.h - header file
============================================================================
*/

#ifndef LGdi_h
#define LGdi_h

#include "LGraphic.h"
#include "UtilString.h"
#include "KRef.h"

// software abstraction layer API
// graphic abstract class
namespace util
{
	class LBitmap;
	class LFont;
	class LRgb;
	class LGraphicsContext
	{
	public:
		enum BrushStyle
		{
            NullBrush = 1,
		    SolidBrush
		};

		enum PenStyle
		{
            NullPen = 1,
		    SolidPen,
		    DotPen
		};
		
		enum TextAlign
		{
		    TextLeft = 1,
		    TextCenter,
		    TextRight
		};
		

	public:
		virtual ~LGraphicsContext(){}

	public:
		virtual LVoid drawLine(const LPoint& p1, const LPoint& p2) = 0;
		virtual LVoid drawLine(LInt x0, LInt y0, LInt x1, LInt y1) = 0;
		virtual LVoid drawRect(const LRect& rect) = 0;
		virtual LVoid drawRect(LInt x, LInt y, LInt w, LInt h) = 0;
		virtual LVoid drawHollowRect(const LRect& rect) = 0;
		virtual LVoid drawHollowRect(LInt x, LInt y, LInt w, LInt h) = 0;

		virtual LVoid drawEllipse(const LRect& rect)=0;

		// 弧形角的方框
		virtual LVoid drawRoundRect(const LRect& rect, const LSize& cornerSize) = 0;

		virtual LVoid drawText(const StringA& text, const LRect& rect) = 0;
		virtual LVoid drawText(const StringA& text, const LRect& rect, TextAlign align) = 0;
		virtual LVoid drawText(const StringA& text, const LPoint& p) = 0;

		virtual LVoid drawBitmap( const LPoint& tl, const LBitmap* b) = 0;
		virtual LVoid drawBitmap( const LRect& rect, const LBitmap* b) = 0;
		virtual LVoid drawBitmap( const LRect& rect, const LBitmap* s, const LRect& sRect) = 0;

		virtual LVoid drawBitmapMasked(const LRect& dRect, const LBitmap* b, const LRect& sRect, const LBitmap *mask, LBool shouldMask)=0;

		virtual LVoid setBrushStyle(BrushStyle style) = 0;
		virtual LVoid setPenStyle(PenStyle style) = 0;

		virtual LVoid setBrushColor(const LRgb& color) = 0;
		virtual LVoid setPenColor(const LRgb& color) = 0;
		virtual LVoid setFont(const LFont& font) = 0;
		
		virtual LVoid submit() = 0;
		virtual LVoid reset() = 0;
	};
	
	class LRgb
	{
	public:
		LInt m_red;
        LInt m_green;
		LInt m_blue;
		LInt m_alpha;
    
	public:
		LRgb()
		    : m_red(0)
		    , m_green(0)
		    , m_blue(0)
		    , m_alpha(255)
		{
		}

		LRgb(LInt aRed, LInt aGreen, LInt aBlue)
			: m_red(aRed)
			, m_green(aGreen)
			, m_blue(aBlue)
		    , m_alpha(255)
		{
		}

		LRgb(LInt aRed, LInt aGreen, LInt aBlue, LInt alpha)
			: m_red(aRed)
			, m_green(aGreen)
			, m_blue(aBlue)
		    , m_alpha(alpha)
		{
		}

		~LRgb(){}

	public:
		LVoid set(LInt aRed, LInt aGreen, LInt aBlue)
		{
			m_red = aRed;
			m_green = aGreen;
			m_blue = aBlue;
		}
	};

	class LBitmap : public KRef
	{		
	public:
		LBitmap() {}
		virtual ~LBitmap() {}

#if ENABLE(BROWSER_KERNEL)
		static LBitmap* create() { return NULL; }
#else
		static LBitmap* create();
#endif

	public:
		virtual LVoid load(const StringA& aPath, LInt32 aId) {}
		virtual LVoid create(const StringA& aFileName){}
		
		void setData(const StringA& data) { m_data = data; }
        virtual LSize size() { return LSize(0,0); }

    private:
        StringA m_data;
	};

	class LFont : public KRef
	{
	public:
		typedef enum FontStyle
		{
			FONT_STYLE_NORMAL = 0,
			FONT_STYLE_ITALIC,
			FONT_STYLE_BOLD,
			FONT_STYLE_UNDERLINE,
		} FontStyle;
		
	public:
#if ENABLE(BROWSER_KERNEL)
        static LFont* createSalInstance(const LFont& font){ return NULL; }
#else	
		static LFont* createSalInstance(const LFont& font);// instance a platform font
#endif
		LFont()
	        : m_fontStyle(FONT_STYLE_NORMAL)
	        , m_size(60)
	    {
        }
        LFont(const LFont& font)
			: m_fontStyle(font.m_fontStyle)
			, m_size(font.m_size)
		{
		}
		virtual ~LFont() {}

	public:
		virtual LInt getFontHeight() const
		{
			return m_size > 0 ? m_size : 12;
		}

		virtual LInt getFontWidth(LUint16 ch) const
		{
			return  12;
		}

		virtual LInt getTextWidth(const StringA& text) const
		{
			return text.GetLength() * getFontAdapter();
		}

		virtual void setFontSize(LInt size)
		{
			m_size = size;
		}

		LInt getFontSize() const
		{
			return m_size;
		}
		
		virtual void setFontStyle(FontStyle style)
		{
			m_fontStyle = style;
		}
		
		FontStyle getFontStyle() const
		{
			return m_fontStyle;
		}

		virtual LInt getLineSize() const
		{
			return 0;
		}

		virtual LInt getLineWidth(LInt index) const
		{
			return 0;
		}

		virtual StringA getLineText(LInt index) const
		{
			return (const LUint8*)NULL;
		}

		virtual LInt calcTextLine(const StringA& text, LInt maxWidth) const
		{
			return 0;
		}

	private:
		// 主要用来做字体宽高适配
		LInt getFontAdapter() const
		{
            return (m_size * 2 / 3);
		}

	protected:
		FontStyle m_fontStyle;
		LInt      m_size;
	};
	
	class MediaPlayer
	{
	public:
#if ENABLE(BROWSER_KERNEL)
        static MediaPlayer* create() { return NULL; }
#else
		static MediaPlayer* create();// instance a platform font
#endif
		virtual ~MediaPlayer() {}
		virtual void start() = 0;
		virtual void play() = 0;
		virtual void pause() = 0;
		virtual void stop()  = 0;
		virtual void release() = 0;
		virtual void reset()   = 0;
	};
}

#endif // LGDI_H

// End of File
