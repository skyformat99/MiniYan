/*
 * TextItem.h
 *
 *  Created on: 2011-6-30
 *      Author: yanbo
 */

#ifndef TextItem_h
#define TextItem_h

#include "InlineItem.h"

namespace yanbo
{

class TextItem : public InlineItem
{
public:
	TextItem(IItemListener* itemListener,
			const util::StringA& id,
			const util::StringA& name,
			const util::StringA& text,
			LBool selectable);
	
	~TextItem();
	
public:
	virtual void layout(RenderContext& rc);
	virtual void paint(util::LGraphicsContext& dc);
	virtual LBool isText() const;
	
	void setAlignement(util::LGraphicsContext::TextAlign alignement);
	void setText(const StringA& text);
	int calcTextLine(const StringA& text, int maxWidth, util::LFont* newFont);
	LBool isMultiLine();
	HtmlItem* getLinkItem();
	
private:
	class Line
	{
	public:
		Line(LInt length, const StringA& text);
		~Line();
		
	public:
		LInt m_lineLength;
		StringA m_text;
	};
	
protected:
	util::StringA m_text;
	util::KVector<Line*> *m_textLines;
	util::LFont* m_newFont;
};

}
#endif /* TEXTITEM_H_ */
