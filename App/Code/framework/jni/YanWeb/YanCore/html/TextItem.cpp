/*
 * TextItem.cpp
 *
 *  Created on: 2011-7-12
 *      Author: yanbo
 */

#include "TextItem.h"
#include "StringUtils.h"
//#include "SalLog.h"
#include "RenderContext.h"
#include "LColor.h"
#include "SalLog.h"
#include "StringUtils.h"

namespace yanbo
{

TextItem::Line::Line(LInt length, const StringA& text)
{
	m_lineLength = length;
	m_text = text;
}

TextItem::Line::~Line()
{	
}

TextItem::TextItem(IItemListener* itemListener,
		const StringA& id,
		const StringA& name,
		const StringA& text,
		LBool selectable)
    : InlineItem(itemListener, id, name, LFalse)
    , m_text(text)
    , m_textLines(NULL)
    , m_newFont(NULL)
{
}

TextItem::~TextItem()
{
	if (NULL != m_newFont)
	{
	    delete m_newFont;
	    m_newFont = NULL;
	}
	
	if (NULL != m_textLines)
	{
	    delete m_textLines;
	    m_textLines = NULL;
	}
}

void TextItem::layout(RenderContext& rc)
{
	handleXYPos(rc);
	
	m_width = 0;
	m_height = 0;
	
	if (NULL == m_textLines)
	{
	    m_textLines = new KVector<Line*>();
	}
	
	m_maxWidth = rc.getMaxWidth() - m_x;
	
	KSTRFORMAT("TextItem::layout m_text=%s", m_text);

	m_newFont = util::LFont::createSalInstance(getStyle()->m_font); // platform reference font
	int longestLine = calcTextLine(m_text, m_maxWidth - getStyle()->marginLeft() - getStyle()->marginRight(), m_newFont);

	m_width = longestLine + getStyle()->marginLeft() + getStyle()->marginRight();
	KFORMATLOG("text=%s and text width=%d and lineWidth=%d", (const char*)m_text.GetBuffer(), m_width, longestLine);
	m_height = m_newFont->getFontHeight() * m_textLines->size() + getStyle()->marginBottom();
	
	KLOG("TextItem::layout begin");
	KDESLOG(m_height);
	KDESLOG(m_newFont->getFontHeight());
	KDESLOG(getStyle()->m_bottomMargin);
	KDESLOG(getBottomY());
	//rc.setX(getEndX());
	//rc.setY(getBottomY() - m_newFont->getFontHeight());
	//rc.setNextLineHeight(m_newFont->getFontHeight());
	//rc.addItemInterval();
	KLOG("TextItem::layout end");

	if (getParent()->isBlockItem())
	{
		rc.addLineItem(this);
		rc.addX(m_width);
		rc.setNextLineHeight(m_height);
	}
	else
	{
		m_x = m_x - getParent()->getXpos();
		m_y = m_y - getParent()->getYpos();
	}

}

void TextItem::setText(const StringA& text)
{
	m_text = text;
}

void TextItem::setAlignement(util::LGraphicsContext::TextAlign alignement)
{
	getStyle()->m_textAlignement = alignement;
}

// Calculate the text lines
LInt TextItem::calcTextLine(const StringA& text, int maxWidth, util::LFont* newFont)
{
	LInt currentMaxLineWidth  = maxWidth;
	
	if (getStyle()->m_positionType < 0)
	{
	    currentMaxLineWidth = maxWidth;
	}

	LInt longestLine = m_newFont->calcTextLine(text, currentMaxLineWidth);
	LInt len = m_newFont->getLineSize();
	for (LInt i = 0; i < len; i++)
	{
		m_textLines->addElement(new Line(
				m_newFont->getLineWidth(i),
				m_newFont->getLineText(i)));
	}

	return longestLine;
}

LBool TextItem::isMultiLine()
{
    return m_textLines->size() > 1 ? LTrue : LFalse;
}

void TextItem::paint(util::LGraphicsContext& gc)
{
	LayoutPoint topLeft = getAbsoluteContainerTopLeft();
	LayoutUnit x = (getParent()->isBlockItem() ? 0 : getParent()->getXpos()) + topLeft.iX + getXpos();
	LayoutUnit y = (getParent()->isBlockItem() ? 0 : getParent()->getYpos()) + topLeft.iY + getYpos();

	if (NULL != m_newFont)
	{
		x = x - m_scrollX;
		y = y - m_scrollY;
		
		LInt textHeight = m_newFont->getFontHeight();
		LInt len = m_textLines->size();
		for (LInt i = 0; i < len; i++)
		{
		    Line* line = m_textLines->elementAt(i);

			y = i > 0 ? y + m_newFont->getFontHeight() : y;
			LayoutUnit left = 0;
			if (getStyle()->m_textAlignement == util::LGraphicsContext::TextCenter)
			{   // 居中对齐
				left = x + (m_width - line->m_lineLength)/2;
			}
			else if (getStyle()->m_textAlignement == util::LGraphicsContext::TextRight)
			{   // 右对齐
				left = x + m_width - line->m_lineLength;
			}
		    
		    HtmlItem* link = getLinkItem();
		    if (link != NULL)
		    {
				KLOG("LINK COLOR");
				if (getStyle()->m_color == COLOR_BLACK)
				{
					gc.setPenColor(util::LColor::parseRgbInt(COLOR_LINKCOLOR));
				}
				else
				{
					gc.setPenColor(util::LColor::parseRgbInt(getStyle()->m_color));
				}
					
				gc.setPenStyle(util::LGraphicsContext::NullPen);
					//gc.drawRect(x, y, line->m_lineLength, textHeight);
					
				gc.setFont(getStyle()->m_font);
				gc.setPenStyle(util::LGraphicsContext::SolidPen);
					//dc.drawText(line->m_text, util::LPoint(x, y + textHeight));
				gc.drawText(line->m_text, util::LRect(left, y, line->m_lineLength, textHeight)
						, util::LGraphicsContext::TextLeft);
					//KLOG("link");
					//KSTRLOG(line->m_text);
				gc.setBrushColor(util::LColor::parseRgbInt(getStyle()->m_bgColor));
				gc.setPenColor(util::LColor::parseRgbInt(getStyle()->m_color));
//

				if (link->isSelected())
				{
					gc.setBrushStyle(util::LGraphicsContext::NullBrush);
					gc.setPenColor(util::LColor::parseRgbInt(COLOR_RED));

					gc.drawHollowRect(x, y, m_width, m_height);
				}
		    }
		    else
		    {

		    	KLOG("draw Text");

                gc.setBrushStyle(util::LGraphicsContext::SolidBrush);
		        gc.setPenColor(util::LColor::parseRgbInt(getStyle()->m_bgColor));
				gc.setPenStyle(util::LGraphicsContext::NullPen);

				
				gc.setFont(getStyle()->m_font);
				

				gc.setPenColor(util::LColor::parseRgbInt(getStyle()->m_color));
				KFORMATLOG("text color=%x", getStyle()->m_color);
				KFORMATLOG("text length=%d", line->m_lineLength);
			    KFORMATLOG("text x=%d, y=%d", x, y);
				//KDESLOG("text color=%x", getStyle()->m_color);
				gc.setPenStyle(util::LGraphicsContext::SolidPen);
				gc.drawText(line->m_text, LRect(left, y, line->m_lineLength, textHeight), util::LGraphicsContext::TextLeft);
				//KLOG("no link");
				//KSTRLOG(line->m_text);
				gc.setBrushColor(util::LColor::parseRgbInt(getStyle()->m_bgColor));
				gc.setPenColor(util::LColor::parseRgbInt(getStyle()->m_color));
		    }
		    

		}
	}
}

LBool TextItem::isText() const
{
    return LTrue;	
}

HtmlItem* TextItem::getLinkItem()
{
	HtmlItem* parent = getParent();
	KSTRFORMAT("TextItem parent tagname=%s", parent->getTagName());
	while (parent && !parent->isLink())
	{
	    parent = parent->getParent();
	}
	
	return parent;
}

}
