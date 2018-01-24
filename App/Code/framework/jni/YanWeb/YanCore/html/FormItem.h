/*
 * FormItem.h
 *
 *  Created on: 2011-6-30
 *      Author: yanbo
 */

#ifndef FormItem_h
#define FormItem_h

#include "InlineItem.h"

namespace yanbo
{

class HtmlForm;
class FormItem : public InlineItem
{
public:
	FormItem(IItemListener* itemListener, 
			const StringA& id, 
			const StringA& name, 
			const StringA& inValue,
			const StringA& inTitle);
	
	virtual ~FormItem();
	
public:
	//virtual void layout(RenderContext& rc);
	void setValue(const StringA& inValue);
	const StringA& getValue() const;
	
	void setTitle(const StringA& title);
	const StringA& getTitle() const;
	void setParentForm(HtmlForm* form);
	const StringA& getMimeType() const;
	const StringA& getEncodingParams() const;
	
protected:
	StringA   m_value;
	StringA   m_title;
	StringA   m_mimeType;
	HtmlForm  *m_parentForm;
};
}
#endif /* FORMITEM_H_ */
