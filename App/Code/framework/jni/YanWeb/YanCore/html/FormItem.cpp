/*
 * FormItem.cpp
 *
 *  Created on: 2011-7-8
 *      Author: yanbo
 */

#include "FormItem.h"

//#define MAX_PASSWORD 32
//#define MAX_TEXTAREA 1024

namespace yanbo
{

FormItem::FormItem(IItemListener* itemListener, 
			const StringA& id, 
			const StringA& name, 
			const StringA& inValue,
			const StringA& inTitle)
    : InlineItem(itemListener, id, name, util::LTrue)
{
	m_value = inValue;
	m_title = inTitle;
	m_mimeType = _CS("application/x-www-form-urlencoded");
	getStyle()->m_transparent = LFalse;
}

FormItem::~FormItem()
{
}

void FormItem::setValue(const StringA& inValue)
{
	m_value = inValue;
}

const StringA& FormItem::getValue() const
{
    return m_value;
}

void FormItem::setTitle(const StringA& title)
{
    m_title = title;	
}

const StringA& FormItem::getTitle() const
{
    return m_title;
}

void FormItem::setParentForm(HtmlForm* form)
{
	m_parentForm = form;
}

const StringA& FormItem::getMimeType() const
{
	return m_mimeType;
}

StringA getEncodingParams()
{
    return _CS("");	
}

}
