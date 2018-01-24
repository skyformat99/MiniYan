/*
 * LGraphicItem.cpp
 *
 *  Created on: 2011-6-23
 *      Author: yanbo
 */

#include "LBoxItem.h"


namespace util
{

LBoxItem::LBoxItem()
    : m_visible(LTrue)
    , m_x(0)
    , m_y(0)
    , m_width(0)
    , m_height(0)
    , m_scrollX(0)
    , m_scrollY(0)
    , m_leftPadding(0)
    , m_topPadding(0)
	, m_style(new Style())
{
}

LBoxItem::LBoxItem(const Style* style)
    : m_x(0)
    , m_y(0)
    , m_width(0)
    , m_height(0)
    , m_scrollX(0)
    , m_scrollY(0)
    , m_leftPadding(0)
    , m_topPadding(0)
{
	m_style   = (Style*)style;
	m_visible = LTrue;	
}

LBoxItem::~LBoxItem()
{
}

void LBoxItem::setTransparent(LBool transparent)
{
	m_style->m_transparent = transparent;
}

LBool LBoxItem::getTransparency()
{
	return m_style->m_transparent;
}

void LBoxItem::setFont(LFont font)
{
	m_style->m_font = font;
}

const LFont& LBoxItem::getFont() const
{
    return m_style->m_font;
}

void LBoxItem::setPos(LInt x, LInt y)
{
	m_x = x;
	m_y = y;
}

void LBoxItem::setSize(LInt width, LInt height)
{
	m_width = width;
	m_height = height;
}

void LBoxItem::translate(LInt x, LInt y)
{
	m_x += x;
	m_y += y;
}

void LBoxItem::setScrollPos(LInt x, LInt y)
{
	m_scrollX = x;
	m_scrollY = y;
}

LInt LBoxItem::getXpos() const
{
	return m_x;
}

LInt LBoxItem::getYpos() const
{
	return m_y;
}

LInt LBoxItem::getWidth() const
{
    return m_width;	
}

LInt LBoxItem::getHeight() const
{
    return m_height;	
}

void LBoxItem::setWidth(LInt width)
{
	m_width = width;
}

void LBoxItem::setHeight(LInt height)
{
	m_height = height;
}

void LBoxItem::setPadding(LInt leftPadding, LInt topPadding)
{
	m_leftPadding = leftPadding;
	m_topPadding = topPadding;
}

void LBoxItem::setVisible(LBool visible)
{
    m_visible = visible;	
}

int LBoxItem::getEndX()
{
    return m_x + m_width;	
}

int LBoxItem::getBottomY()
{
	return m_y + m_height;
}

LInt LBoxItem::getScrollYPos()
{
	return m_scrollY;
}

void LBoxItem::setXpos(LInt x)
{
	m_x = x;
}

void LBoxItem::setYpos(LInt y)
{
    m_y = y;
}

}
