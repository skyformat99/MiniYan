/*
 * HtmlForm.h
 *
 *  Created on: 2011-7-1
 *      Author: yanbo
 */

#ifndef HtmlForm_h
#define HtmlForm_h


#include "BlockItem.h"
#include "klist.h"
#include "FormItem.h"
namespace yanbo
{

typedef KList<FormItem*> FormItemList;
class HtmlForm : public BlockItem,
        public IHtmlForm
{
public:
	HtmlForm(IItemListener* itemListener, 
			const StringA& id, 
			const StringA& name,
			const StringA& action,
			const StringA& method,
			const StringA& encoding,
			LBool selectable);
	
	virtual ~HtmlForm();
	
public:
	virtual void layout(RenderContext& rc);
	void addFormItem(FormItem* aFormItem);
	const FormItemList& getFormItems() const;
	void reset();
	FormItem* getFormItem(const StringA& itemName);
	void UnCheckRadioButtons(const StringA& name);
	LInt getLastSubmitTime();
	
public:
	/** Return url to post to. */
	virtual StringA getAction();
	
	/** Return GET or POST. */
	virtual StringA getMethod();
	
	/** Return URL encoded parameter string. */
	virtual StringA encodeFormItems();
	
	/** Return encoding for From. */
	virtual StringA getEncoding();
	
	virtual StringA getItemValue(const StringA& itemName);
	virtual StringA getItemMimeType(const StringA& itemName);
	virtual void setItemValue(const StringA& name, const StringA& value);
	virtual LBool hasPassword();
	
	/** Write the form data to the output stream. */
	virtual void postForm(StringA& outstream);
	
private:
	FormItemList               m_formList;
	StringA                    m_action;
	StringA                    m_method;
	StringA                    m_encoding;
	StringA                    m_boundaryString;
	LInt                       m_lastSubmitTime;
};

}
#endif /* HTMLFORM_H_ */
