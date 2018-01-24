/*
 * BlockItem.h
 *
 *  Created on: 2011-6-30
 *   Modify on: 2012-12-02 All Copyright reserved
 *      Author: yanbo
 */

#ifndef BlockItem_h
#define BlockItem_h

#include "HtmlItem.h"
namespace yanbo
{

// block item
class BlockItem : public HtmlItem
{
public:
	typedef KList<HtmlItem*> PositionedObjectsList;

public:
	BlockItem(IItemListener* itemListener, const StringA& id, 
			const StringA& name, LBool selectable);
	
	virtual ~BlockItem();
	
public:
	// layout method inline or block child
	void setChildrenInline(LBool isInline);
	LBool isChildrenInline();

	// add create anonymous or normal child
    virtual void addChild(HtmlItem* child);
	void addChild(HtmlItem* child, LBool isNotAnonymousBlock);

	// layer layout
	virtual void layout();
	void layoutBlock(LBool relayoutChildren);
	void layoutBlockChildren(LBool relayoutChildren, LayoutUnit& maxFloatLogicalBottom);
	void layoutInlineChildren();
	// layout block child
	void layoutBlockChild(HtmlItem* child, LayoutUnit& currentLogicBottom);
	//void layoutInlineChild(HtmlItem* child);
	LBool layoutSpecialChild(HtmlItem* child, LayoutUnit& currentLogicBottom);
	void makeChildrenNonInline(HtmlItem* block);
	BlockItem* createAnonymousBlock();

	// paint child
	virtual void paint(util::LGraphicsContext& dc);
	virtual LBool isBlockItem() const;

	void setIsAnonymousBlock(LBool isAnonymous);
	virtual LBool isAnonymousBlock();
	virtual LBool isInlineBlockOrInlineTable() const { return isInline() && isReplaced(); }

	void insertPositionedObject(HtmlItem* o);
	void adjustPositionedBlock(HtmlItem* child);

private:
	// for layoutSpecialChild use.
	LBool layoutPositionChild(HtmlItem* child);

private:
	LBool                       m_isChildrenInline;
	LBool                       m_isAnonymous;
	PositionedObjectsList*      m_positionedObjects;
};

}
#endif /* BLOCKITEM_H_ */
