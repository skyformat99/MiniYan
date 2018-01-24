/*
 * InputItem.cpp
 *
 *  Created on: 2011-7-17
 *      Author: Administrator
 */

#include "InputItem.h"
//#include "SalGdi.h"
#include "LColor.h"
#include "StringUtils.h"
#include "RenderContext.h"
#include "SalLog.h"

namespace yanbo
{

InputItem::InputItem(IItemListener* itemListener, 
		const StringA& id,
		const StringA& name,
		const StringA& value,
		const StringA& title,
		const StringA& inputType,
		const StringA& imageUrl,
		LBool checked)
    : FormItem(itemListener, id, name, value, title)
{
    m_type = TEXT;
    m_value = value;
    m_title = title;
    m_checked = checked;
    
    if (inputType.GetLength() > 0)
    {
        if (inputType.CompareNoCase(_CS("text")))
        {
            m_type = TEXT;
        }
        else if (inputType.CompareNoCase(_CS("password")))
        {
            m_type = PASSWORD;
        }
        else if (inputType.CompareNoCase(_CS("reset")))
        {
            m_type = RESET;
        }
        else if (inputType.CompareNoCase(_CS("radio")))
        {
            m_type = RADIO;				
        }
        else if (inputType.CompareNoCase(_CS("checkbox")))
        {
            m_type = CHECKBOX;				
        }
        else if (inputType.CompareNoCase(_CS("file")))
        {
            m_type = FILE;
        }
        else if (inputType.CompareNoCase(_CS("hidden")))// fixed
        {
            m_type = HIDDEN;
            m_selectable = util::LFalse;
        }
        else if (inputType.CompareNoCase(_CS("button")))
        {
            m_type = BUTTON;
        }
        else if (inputType.CompareNoCase(_CS("submit")))
        {
            m_type = SUBMIT;
        }
        else if (inputType.CompareNoCase(_CS("image")))
        {
            m_type = IMAGE;
        }
        else
        {
            m_type = NOTSUPPORTED;
        }
    }
}

InputItem::~InputItem()
{
	
}

void InputItem::layout(RenderContext& rc)
{
	rc.addLineItem(this);
	int maxWidth = rc.getMaxWidth();
	util::LFont* m_newFont = util::LFont::createSalInstance(getStyle()->m_font);
	switch (m_type)
	{
	case BUTTON:
	case SUBMIT:
	case RESET:
		{
			//m_leftPadding = maxWidth / 80;
			m_leftPadding = 0;
			//m_width  = m_width + 6;
			m_width  = maxWidth / 5;
			m_height = m_newFont->getFontHeight() + 10;
		}
		break;
	case TEXT:
	case PASSWORD:
		{
		    m_leftPadding = 5;
		    m_width =  maxWidth / 3;
		    m_height = m_newFont->getFontHeight() + 6;
		}
		break;
	case IMAGE:
		{

		}
		break;
	case HIDDEN:
		{
		    m_selectable = util::LFalse;
		    m_width = 0;
		    m_height = 0;
		    m_leftPadding = 0;
		}
		break;
	case FILE:
		{
			m_width = maxWidth/2;
			m_leftPadding = maxWidth / 80;
			m_height = m_newFont->getFontHeight() + 6;
			m_mimeType = _CS("text/plain"); //test
			m_value = _CS("File upload not supported");
		}
		break;
	case CHECKBOX:
		{
			m_leftPadding    = maxWidth / 25;
			m_width = m_height = m_newFont->getFontHeight();
		}
		break;
	case RADIO:
		{
			m_leftPadding = maxWidth / 25;
			m_width = m_height = m_newFont->getFontHeight();
		}
		break;
	}
	
	if (HIDDEN != m_type && rc.getX() >= 0)
	{
	    if(rc.getX() + m_leftPadding + m_width > rc.getMaxWidth() + rc.getNewLineXStart())
	    {
	        rc.newLine(this);
	        rc.setNextLineHeight(m_height);
	    }
	    else
	    {
	        if (rc.getLineItemCount() >= 0)
	        {
	            if (m_height > rc.getNextLineHeight())
	            {
	                rc.setNextLineHeight(m_height);
	            } 
	        }
	    }
	}
	
	setPos(rc.getX() + rc.getMaxWidth()/80, rc.getY());
	
	//itemCenter(rc);
	
	KLOG("InputItem::layout");
	KDESLOG(m_y);
	KDESLOG(m_height);
	
	rc.addX(m_width);
	
	if (m_type != HIDDEN)
	{
	    rc.addItemInterval();
	    rc.addLineItemCount();
	}

}

void InputItem::paint(util::LGraphicsContext& gc)
{
	LayoutPoint topLeft = getAbsoluteContainerTopLeft();
	int x = topLeft.iX + getXpos();
	int y = topLeft.iY + getYpos();

	if (HIDDEN == m_type)
	{
	    return;
	}
	
	gc.setFont(getStyle()->m_font);

	switch (m_type)
	{
    case TEXT:
    case PASSWORD:
    case FILE:
    	{
    	    paintTextBox(gc, x, y);
    	}
    	break;
    case SUBMIT:
    case BUTTON:
    case RESET:
    	{
    	    paintButton(gc, x, y);
    	}
    	break;
    case CHECKBOX:
    	{
    	
    	}
    	break;
	}
	
    if (m_selected)
    {
		gc.setBrushStyle(util::LGraphicsContext::NullBrush);
        gc.setPenColor(util::LColor::parseRgbInt(COLOR_RED));
        
        gc.drawHollowRect(x - m_scrollX + m_leftPadding, y - m_scrollY, m_width, m_height);
//        gc.drawRect(x - m_scrollX + m_leftPadding+1, y - m_scrollY+1, m_width-2, m_height-2);
    }
}

void InputItem::paintTextBox(util::LGraphicsContext& gc, LayoutUnit x, LayoutUnit y)
{
    gc.setBrushStyle(util::LGraphicsContext::SolidBrush);
    gc.setBrushColor(util::LColor::parseRgbInt(getStyle()->m_bgColor));

    //gc.setBrushColor(util::LRgb(200, 10, 0));
   
    gc.setPenStyle(util::LGraphicsContext::NullPen);
    gc.drawRect(x - m_scrollX + m_leftPadding, y - m_scrollY, m_width, m_height);
    
    paintBorder(gc, getStyle()->m_border, x - m_scrollX + m_leftPadding, y - m_scrollY);
	
	if (m_value.GetLength() > 0)
	{
		if (m_type == PASSWORD)
		{
			//gc.drawText(_WS("********"), util::LPoint(m_x, m_y));
			gc.drawText(_CS("********"), LRect(x - m_scrollX + m_leftPadding, y - m_scrollY + 6, m_width-m_leftPadding, m_height-6), util::LGraphicsContext::TextLeft);
		}
		else
		{
		    //util::String strW;
		    //util::StringUtils::strTostrW(m_value, strW);
		    // dc.drawText(strW, util::LPoint(m_x, m_y));
			gc.drawText(m_value, LRect(x - m_scrollX + m_leftPadding, y - m_scrollY + 6, m_width-m_leftPadding, m_height-6), util::LGraphicsContext::TextLeft);
		}
	}	

	
}

void InputItem::paintButton(util::LGraphicsContext& gc, LayoutUnit x, LayoutUnit y)
{
	if (getStyle()->m_bgColor == COLOR_WHITE)
	{
	    gc.setBrushStyle(util::LGraphicsContext::SolidBrush);
        gc.setBrushColor(util::LColor::parseRgbInt(COLOR_LIGHTGRAY));
        //dc.drawRect(m_x + m_leftPadding, m_y - m_scrollY, m_width, m_height);

        gc.setPenStyle(util::LGraphicsContext::SolidPen);
        gc.setPenColor(util::LColor::parseRgbInt(COLOR_DARKGRAY));
        gc.drawRect(x - m_scrollX + m_leftPadding, y - m_scrollY, m_width, m_height);
	}
	else
	{
	    gc.setBrushStyle(util::LGraphicsContext::SolidBrush);
	    gc.setBrushColor(util::LColor::parseRgbInt(getStyle()->m_bgColor));
       
        gc.setPenStyle(util::LGraphicsContext::NullPen);
        gc.drawRect(x - m_scrollX + m_leftPadding, y - m_scrollY, m_width, m_height);
        
        gc.setPenStyle(util::LGraphicsContext::SolidPen);
        paintBorder(gc, getStyle()->m_border, x, y);
        

	}
	
    if (m_value.GetLength() >0) {
        gc.setPenStyle(util::LGraphicsContext::SolidPen);
        gc.setPenColor(util::LColor::parseRgbInt(getStyle()->m_color));
	    //util::String strW;
	    //util::StringUtils::strTostrW(m_value, strW);
        //dc.drawText(strW, util::LPoint(m_x + m_leftPadding, m_y - m_scrollY + m_height));
		gc.drawText(m_value, LRect(x - m_scrollX + m_leftPadding,
				y - m_scrollY + 6, m_width-m_leftPadding,
				m_height-6),
				util::LGraphicsContext::TextCenter);
    }
}

void InputItem::paintRadioButton(util::LGraphicsContext& dc, LayoutUnit x, LayoutUnit y)
{
	
}

void InputItem::paintCheckBox(util::LGraphicsContext& dc, LayoutUnit x, LayoutUnit y)
{
	
}

void InputItem::execute()
{
	
}

int InputItem::getInputType()
{
    return 	m_type;
}

void InputItem::setChecked(LBool isChecked)
{
	m_checked = isChecked;
}

const util::LBool InputItem::getChecked() const
{
	return m_checked;
}

}
