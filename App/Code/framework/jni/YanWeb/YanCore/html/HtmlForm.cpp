/*
 * HtmlForm.cpp
 *
 *  Created on: 2011-7-26
 *      Author: Administrator
 */

#include "HtmlForm.h"
#include "InputItem.h"

namespace yanbo
{

HtmlForm::HtmlForm(IItemListener* itemListener, 
		const StringA& id, 
		const StringA& name,
		const StringA& action,
		const StringA& method,
		const StringA& encoding,
		LBool selectable)
    : BlockItem(itemListener, id, name, selectable)
    , m_action(action)
{
	if(method.GetLength() == 0)
	{
	    m_method = _CS("GET");
	}
	else
	{
	    m_method = method;
	    m_method.ToUpper();
	}
	
	if(encoding.GetLength() == 0)
	{
	    m_encoding = _CS("application/x-www-form-urlencoded");
	}
	else
	{
	    m_encoding = encoding;
	    if(m_encoding.CompareNoCase(_CS("multipart/form-data")))
	    {
	        m_boundaryString = _CS("---------------------------7d816b1e15035a");
	    }
	}
}

HtmlForm::~HtmlForm()
{
	
}

void HtmlForm::addFormItem(FormItem* aFormItem)
{
	m_formList.push_back(aFormItem);
}

const FormItemList& HtmlForm::getFormItems() const
{
	return m_formList;
}

void HtmlForm::layout(RenderContext& rc)
{
	BlockItem::layout();
}

void HtmlForm::reset()
{
	
}

StringA HtmlForm::getAction()
{
	return m_action;
}

StringA HtmlForm::getMethod()
{
    return m_method;	
}

LBool HtmlForm::hasPassword()
{
	FormItemList::Iterator iter = m_formList.begin();
	FormItemList::Iterator iterEnd = m_formList.end();
	for(; iter != iterEnd; ++iter)
	{
	    FormItem* fItem = *iter;
	    if(fItem->getTagType() == HtmlTags::INPUT)
	    {
	        InputItem* inpItem = (InputItem*)fItem;
	        if (inpItem->getInputType() == InputItem::PASSWORD)
	        {
	            return util::LTrue;
	        }
	    }
	}
	
	return util::LFalse;
}

StringA HtmlForm::getEncoding()
{
	if(m_boundaryString.GetLength() == 0)
	{
	    return m_encoding;
	}
	else
	{
	    return m_encoding + _CS("; boundary=") + m_boundaryString;
	}
}

StringA HtmlForm::getItemValue(const StringA& itemName)
{
	FormItem* fItem = getFormItem(itemName);
	if(NULL != fItem)
	{
	    return fItem->getValue();
	}
	else
	{
	    return _CS("");
	}
}

StringA HtmlForm::getItemMimeType(const StringA& itemName)
{
	FormItem* fItem = getFormItem(itemName);
	if(NULL != fItem)
	{
	    return fItem->getMimeType();
	}
	else
	{
	    return _CS("");
	}
}

void HtmlForm::setItemValue(const StringA& name, const StringA& value)
{
	FormItem* fItem = getFormItem(name);
	if(NULL != fItem)
	{
	    return fItem->setValue(value);
	}
}

FormItem* HtmlForm::getFormItem(const StringA& itemName)
{
	FormItemList::Iterator iter = m_formList.begin();
	FormItemList::Iterator iterEnd = m_formList.end();
	for(; iter != iterEnd; ++iter)
	{
	    FormItem* fItem = *iter;
	    if(fItem->getName().CompareNoCase(itemName))
	    {
	        return fItem;
	    }
	}
	
	return NULL;
}

void HtmlForm::UnCheckRadioButtons(const StringA& name)
{
	FormItemList::Iterator iter = m_formList.begin();
	FormItemList::Iterator iterEnd = m_formList.end();
	for(; iter != iterEnd; ++iter)
	{
		FormItem* fItem = *iter;
		if(fItem->getTagType() == HtmlTags::INPUT)
		{
		    InputItem* inpItem = (InputItem*)fItem;
		    if(inpItem->getInputType() == InputItem::RADIO
		    		&& inpItem->getName().CompareNoCase(name))
		    {
		        inpItem->setChecked(util::LFalse);
		    }
		}
	}
}

LInt HtmlForm::getLastSubmitTime()
{
	return m_lastSubmitTime;
}

void HtmlForm::postForm(StringA& outstream)
{
	LBool isMultiPart = m_encoding.CompareNoCase(_CS("multipart/form-data"));
	LBool firstParam = util::LTrue;
} 

StringA HtmlForm::encodeFormItems()
{
	return _CS("");
}

}
