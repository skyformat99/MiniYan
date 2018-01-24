/*
 * InputItem.h
 *
 *  Created on: 2011-6-30
 *      Author: yanbo
 */

#ifndef InputItem_h
#define InputItem_h

#include "FormItem.h"

namespace yanbo
{

class InputItem : public FormItem
{
public:
    enum InputType
    {
    	NOTSUPPORTED,
    	TEXT,
    	PASSWORD,
    	CHECKBOX,
    	RADIO,
    	SUBMIT,
    	RESET,
    	FILE,
    	HIDDEN,
    	IMAGE,
    	BUTTON
    };
    
private:
    LInt                 m_type;
    LBool                m_checked;
    LBool                m_activated;
    util::LBitmap*       m_bitmap;
    
public:
    InputItem(IItemListener* itemListener, 
			const StringA& id,
			const StringA& name,
			const StringA& value,
			const StringA& title,
			const StringA& inputType,
			const StringA& imageUrl,
			LBool checked);
    
    ~InputItem();
    
public:
	virtual void layout(RenderContext& rc);
	virtual void paint(util::LGraphicsContext& dc);
	virtual void execute();
	
	int getInputType();
	void setChecked(LBool isChecked);
	const LBool getChecked() const;

private:
	void paintTextBox(util::LGraphicsContext& gc, LayoutUnit x, LayoutUnit y);
	void paintButton(util::LGraphicsContext& gc, LayoutUnit x, LayoutUnit );
	void paintRadioButton(util::LGraphicsContext& gc, LayoutUnit x, LayoutUnit );
	void paintCheckBox(util::LGraphicsContext& gc, LayoutUnit x, LayoutUnit );
};

}
#endif /* INPUTITEM_H_ */
