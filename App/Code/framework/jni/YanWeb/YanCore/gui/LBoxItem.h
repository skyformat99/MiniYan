/*
 * LBoxItem.h
 *
 *  Created on: 2011-6-22
 *      Author: yanbo
 */

#ifndef LBoxItem_h
#define LBoxItem_h

#include "PlatformLib.h"
#include "Style.h"
#include "KRef.h"
#include "KRefPtr.h"

namespace util
{

/**
 * LBoxItem. Visible items (like text, images, drawings etc through
 * subclassing) that knows how to draw themselves unto a Graphics.
 * To achieve this LBoxItem has a position, width height,
 * fore- and backgroundcolor and related properties. Each subclass of 
 * this abstract class, must implement a paint function, in order to specify
 * how the item is to be painted.
 */

class LBoxItem : public KRef
{
public:
	LBoxItem();
	LBoxItem(const Style* style);
	virtual ~LBoxItem();
	
public:
	void setTransparent(LBool transparent);
	LBool getTransparency();
	
	void setFont(LFont font);
	const LFont& getFont() const;
	
	void setPos(LInt x, LInt y);
	void setSize(LInt width, LInt height);
	
	void translate(LInt x, LInt y);
	virtual void setScrollPos(LInt x, LInt y);
	LInt getScrollYPos();
	
	void setXpos(LInt x);
	void setYpos(LInt y);

	LInt getXpos() const;
	LInt getYpos() const;
	
	LInt getWidth() const;
	LInt getHeight() const;
	
	void setWidth(LInt width);
	void setHeight(LInt height);
	
	void setPadding(LInt leftPadding, LInt topPadding);
	
	void setVisible(LBool visible);
	
	virtual int getEndX();
	virtual int getBottomY();

public:
	KRefPtr<Style>   m_style;

protected:
	LBool           m_visible;

	/** The x position of this item */
	LInt            m_x;

	/** The y position of this item */
	LInt            m_y;

	/** The vertical scroll position of this item. */
	LInt            m_scrollY;

	/** The horisontal scroll position of this item */
	LInt            m_scrollX;

	/** The width in pixels of this item */
	LInt            m_width;

	/** The height in pixels of this item */
	LInt            m_height;

	/** The number of padding pixels from the leftmost border of this item */
	LInt            m_leftPadding;

	/** The number of padding pixels from the topmost border of this item */
	LInt            m_topPadding;
};
}
#endif /* LGRAPHICITEM_H_ */
