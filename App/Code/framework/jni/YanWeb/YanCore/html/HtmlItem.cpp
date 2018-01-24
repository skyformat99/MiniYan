/*
 * HtmlItem.cpp
 *
 *  Created on: 2011-7-1
 *      Author: yanbo
 */

#include "HtmlItem.h"
#include "LColor.h"
#include "HtmlDocument.h"
#include "kvector.h"
#include "StringUtils.h"
#include "RenderContext.h"
#include "SalLog.h"
#include "BlockItem.h"
#include "KRefPtr.h"

namespace yanbo
{

#define kDefaultInputBorderWidth 2
/* 在HTML 中，比较麻烦的是在一个Inline flow 中，有Block 对象，如下面的例子：
*  <i>Italic only<b>Italic and bold
*  <div>
*  Wow, a block!
*  </div>
*  <div>
*  Wow, another block!
*  </div>
*  More italic and bold text</b>More italic text</i>
*  在上面的例子中，两个div 都违背了在Inline flow 中必须全部都是inline 类型的原则。
*  在Render 树有一个很复杂的步骤来完善这棵树。Render 树中会构造三个匿名的Block flow，
*  第一个Block flow 放在存有div 之前的所有的Inline 类型，第二个匿名的Block 保存div，第
*  三个匿名的Block 存有在div 之后的所有的inline 类型。
*  <anonymous pre block>
*  <i>Italic only<b>italic and bold</b></i>
*  </anonymous pre block>
*  <anonymous middle block>
*  <div>
*  Wow, a block
*  </div>
*  <div>
*  Wow, another, block!
*  </div>
*  </anonymous middle block>
*  <anonymous post block>
*  <i><b>More italic and bold text</b>More italic text</i>
*  </anonymous post block>
*  注意到， 这里的bold 和italic 节点已经结合成为两个Render 对象，因为他们既在
*  anonymous pre block 中，也在anonymous post block 中。Inline flow 通过这种递归的拆分并创
*  建连续链(continuation chain)，来联系这些对象。
*/

// The HashMap for storing continuation pointers.
// An inline can be split with blocks occuring in between the inline content.
// When this occurs we need a pointer to the next object. We can basically be
// split into a sequence of inlines and blocks. The continuation will either be
// an anonymous block (that houses other blocks) or it will be an inline flow.
// <b><i><p>Hello</p></i></b>. In this example the <i> will have a block as
// its continuation but the <b> will just have an inline as its continuation.

HtmlItem::HtmlItem(
		IItemListener* itemListener,
		const util::StringA& id,
		const util::StringA& name,
		util::LBool selectable)
	: m_itemListener(itemListener)
    , m_id(id)
    , m_name(name)
    , m_selectable(selectable)
    , m_parent(NULL)
    , m_maxWidth(0)
    , m_type(HtmlTags::TAGNONE)
    , m_selected(LFalse)
    , m_lineNumber(0)
    , m_doc(NULL)
	, m_isViewRoot(LFalse)
{
}

HtmlItem::~HtmlItem()
{
	HtmlItemList::Iterator iter = m_children.begin();
	HtmlItemList::Iterator endIter = m_children.end();

	for (; iter != endIter; ++iter)
	{
		delete *iter;
	}

	m_children.clear();
}

void HtmlItem::layout(RenderContext& rc)
{
    layoutInline(rc);	
}

void HtmlItem::layoutInline(RenderContext& rc)
{

	if (m_type == HtmlTags::BR)
	{
	    rc.newLine(this);
	}
	else
	{
		rc.addLineItem(this);
		handleXYPos(rc);

		HtmlItemList::Iterator iter = m_children.begin();
		HtmlItemList::Iterator iterEnd = m_children.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->layout(rc);
			m_width += (*iter)->getWidth();
			if (m_height < (*iter)->getHeight())
			{
				m_height = (*iter)->getHeight();
			}
		}

		KFORMATLOG("Inline::layout m_x=%d and width=%d", m_x, m_width);
		rc.addX(m_width);
		rc.setNextLineHeight(m_height);
	}

}

void HtmlItem::handleXYPos(RenderContext& rc)
{
    switch (getStyle()->m_positionType)
    {
    case util::Style::RELATIVEPOSITION:
        {
            m_x = rc.getX() + getStyle()->m_left;
            m_y = rc.getY() + getStyle()->m_top;
        }
        break;
    case util::Style::FIXEDPOSITION:
    case util::Style::ABSOLUTEPOSITION:
        {
            m_x = getStyle()->m_left;
            m_y = getStyle()->m_top;
        }
        break;
    default:
        {
            m_x = rc.getX();
            m_y = rc.getY();
        }
        break;
    }
    
    rc.setX(m_x);
    rc.setY(m_y);
}

void HtmlItem::paint(util::LGraphicsContext& gc)
{	
    KLOG("HtmlItem::paint");
	LayoutPoint topLeft = getAbsoluteContainerTopLeft();
	LayoutUnit x = getXpos();
	LayoutUnit y = getYpos();

	if (getStyle()->m_positionType != util::Style::FIXEDPOSITION)
	{
		x += topLeft.iX;
		y += topLeft.iY;
	}
	else
	{
		KFORMATLOG("paint, x=%d, y=%d", x, y);
	}

	if (m_type == HtmlTags::BR)
	{
        return;
	}

	KFORMATLOG("tagName=%s, htmlitem m_bgColor=%x", (const char*)m_tagName.GetBuffer(), getStyle()->m_bgColor);
	if (!getStyle()->m_transparent)
	{
		KLOG("not transparent");
		KFORMATLOG("x=%d, y=%d, w=%d, h=%d", x - m_scrollX, y - m_scrollY, m_width, m_height);
	    gc.setBrushStyle(util::LGraphicsContext::SolidBrush);
	    gc.setBrushColor(util::LColor::parseRgbInt(getStyle()->backgroundColor()));
	    gc.setPenStyle(util::LGraphicsContext::NullPen);
	    gc.drawRect(x - m_scrollX, y - m_scrollY, m_width, m_height);
	}
	
//	paintBorder(gc, getStyle()->m_border, x, y);
	
	HtmlItemList::Iterator iter    = m_children.begin();
	HtmlItemList::Iterator iterEnd = m_children.end();
	
	for (; iter != iterEnd; ++iter)
	{
        HtmlItem* childItem = *iter;
        childItem->paint(gc);
	}
}

void HtmlItem::paintBorder(util::LGraphicsContext& gc, util::Border& border, LayoutUnit x, LayoutUnit y)
{
	x = x - m_scrollX;
	y = y - m_scrollY;
	
	if (border.m_topWidth > 0)
	{
	    gc.setPenStyle((util::LGraphicsContext::PenStyle)border.m_topStyle);
	    gc.setPenColor(util::LColor::parseRgbInt(border.m_topColor));
		gc.setBrushStyle(util::LGraphicsContext::SolidBrush);
	    gc.setBrushColor(util::LColor::parseRgbInt(border.m_topColor));
		gc.drawRect(x, y, m_width, border.m_topWidth);
	}
	else 
	{
		if (m_type == HtmlTags::INPUT)
		{
		    gc.setPenStyle((util::LGraphicsContext::PenStyle)border.m_topStyle);
		    gc.setPenColor(util::LColor::parseRgbInt(COLOR_DARKGRAY));
			gc.setBrushStyle(util::LGraphicsContext::SolidBrush);
	        gc.setBrushColor(util::LColor::parseRgbInt(COLOR_DARKGRAY));
		    gc.drawRect(x, y, m_width, kDefaultInputBorderWidth);
		}

	}
	
	if (border.m_leftWidth > 0)
	{
	    gc.setPenStyle((util::LGraphicsContext::PenStyle)border.m_leftStyle);
	    gc.setPenColor(util::LColor::parseRgbInt(border.m_leftColor));
		gc.setBrushStyle(util::LGraphicsContext::SolidBrush);
	    gc.setBrushColor(util::LColor::parseRgbInt(border.m_leftColor));
	    gc.drawRect(x, y, border.m_leftWidth, m_height);
	}
	else
	{
		if (m_type == HtmlTags::INPUT)
		{
			gc.setPenStyle((util::LGraphicsContext::PenStyle)border.m_topStyle);
			gc.setPenColor(util::LColor::parseRgbInt(COLOR_DARKGRAY));
			gc.setBrushStyle(util::LGraphicsContext::SolidBrush);
	        gc.setBrushColor(util::LColor::parseRgbInt(COLOR_DARKGRAY));
			gc.drawRect(x, y, kDefaultInputBorderWidth, m_height);
		}
	}
	
	if (border.m_bottomWidth > 0)
	{
	    gc.setPenStyle((util::LGraphicsContext::PenStyle)border.m_bottomStyle);
	    gc.setPenColor(util::LColor::parseRgbInt(border.m_bottomColor));
		gc.setBrushStyle(util::LGraphicsContext::SolidBrush);
	    gc.setBrushColor(util::LColor::parseRgbInt(border.m_bottomColor));
	    gc.drawRect(x, y + m_height - border.m_bottomWidth, m_width, border.m_bottomWidth);
	}
	else
	{
		if (m_type == HtmlTags::INPUT)
		{
			gc.setPenStyle((util::LGraphicsContext::PenStyle)border.m_topStyle);
			gc.setPenColor(util::LColor::parseRgbInt(COLOR_DARKGRAY));
			gc.setBrushStyle(util::LGraphicsContext::SolidBrush);
	        gc.setBrushColor(util::LColor::parseRgbInt(COLOR_DARKGRAY));
			gc.drawRect(x, y + m_height - border.m_bottomWidth, m_width, kDefaultInputBorderWidth);
		}
	}
	
	if (border.m_rightWidth > 0)
	{
	    gc.setPenStyle((util::LGraphicsContext::PenStyle)border.m_rightStyle);
	    gc.setPenColor(util::LColor::parseRgbInt(border.m_rightColor));
		gc.setBrushStyle(util::LGraphicsContext::SolidBrush);
	    gc.setBrushColor(util::LColor::parseRgbInt(border.m_rightColor));
	    gc.drawRect(x + m_width - border.m_rightWidth, y, border.m_rightWidth, m_height);
	}
	else
	{
		if (m_type == HtmlTags::INPUT)
		{
			gc.setPenStyle((util::LGraphicsContext::PenStyle)border.m_topStyle);
			gc.setPenColor(util::LColor::parseRgbInt(COLOR_DARKGRAY));
			gc.setBrushStyle(util::LGraphicsContext::SolidBrush);
	        gc.setBrushColor(util::LColor::parseRgbInt(COLOR_DARKGRAY));
			gc.drawRect(x + m_width - border.m_rightWidth, y, kDefaultInputBorderWidth, m_height);
		}
	}
	
}

LBool HtmlItem::isText() const
{
	return LFalse;
}

LBool HtmlItem::isLink() const
{
	return LFalse;	
}

void HtmlItem::execute()
{
}

void HtmlItem::setId(const StringA& id)
{
    m_id = id;	
}

const StringA& HtmlItem::getId() const
{
	return m_id;
}

void HtmlItem::setName(const StringA& name)
{
    m_name = name;	
}

const StringA& HtmlItem::getName() const
{
	return m_name;
}

void HtmlItem::setTagName(const StringA& tagName)
{
    m_tagName = tagName;	
}

const StringA& HtmlItem::getTagName() const
{
	return m_tagName;
}

void HtmlItem::setClassName(const StringA& className)
{
    m_className = className;	
}

const StringA& HtmlItem::getClassName() const
{
	return m_className;
}

void HtmlItem::setTagType(HtmlTags::HtmlType tagType)
{
	m_type = tagType;
}

HtmlTags::HtmlType HtmlItem::getTagType() const
{
	return m_type;
}

// apply style property for render tree
void HtmlItem::setStyle(util::CssManager* manager, util::CssRule* parentRule)
{
	KLOG("HtmlItem::setStyle");
	KRefPtr<KVector<StringA> > classNames = util::StringUtils::split(m_className, _CS(" "));
	StringA idCss = _CS("#") + m_id;	
	int size = classNames->size();
	for (int i=0; i<size; i++)
	{
	    (*classNames.get())[i] = _CS(".") + (*classNames.get())[i];
	}
	KLOG("HtmlItem::setStyle1");
	manager->pushDoctreeNode(idCss, *(classNames.get()), m_tagName);
	KLOG("HtmlItem::setStyle2");
	KRefPtr<util::CssRule> newRule = manager->createNewCssRule(parentRule, manager->getCssRule());
	KLOG("HtmlItem::setStyle3");
	HtmlItemList::Iterator iter = m_children.begin();
	HtmlItemList::Iterator iterEnd = m_children.end();
	for (; iter != iterEnd; ++iter)
	{  
		(*iter)->setStyle(manager, newRule.get());
	}
    
	manager->popDoctreeNode();
    util::LFont::FontStyle fontStyle = util::LFont::FONT_STYLE_NORMAL;
    if (NULL != m_parent)
    {
        fontStyle = m_parent->getFont().getFontStyle();
    }
    
    newRule->createStyle(*m_style.get(), fontStyle);
	KLOG("HtmlItem::setStyle end");
}

void HtmlItem::setSelected(const util::LBool selected)
{
	m_selected = selected;	
}

util::LBool HtmlItem::isSelected()
{
	return m_selected;
}

void HtmlItem::setOnClick(const util::StringA& onClick)
{
    m_onClick = onClick;	
}

const util::StringA& HtmlItem::getOnClick() const
{
    return m_onClick;	
}

HtmlItem* HtmlItem::getPreItem()
{
	HtmlItem* item = NULL;
	if(NULL != m_parent)
	{
        HtmlItemList::Iterator iter = m_parent->m_children.begin();
        HtmlItemList::Iterator iterEnd = m_parent->m_children.end();
		for (; iter != iterEnd; ++iter)
		{
		    // because klist is a two-way circular linked list, so only iterEnd
		    // can judge whether list is end.
		    HtmlItemList::Iterator tmpIter = iter;
			if (*tmpIter == this && (--tmpIter) != iterEnd)
			{
				if((*iter)->m_children.count())
				{
				    item = getPreItem(*tmpIter);
				}
				else
				{
				    item = *tmpIter;
				}
				
				break;
			}
		}

		if (NULL == item)
		{
		    item = m_parent;
		}
	}

	if(NULL == item)
	{
	    item = this;
	}
	
	return item;
}

HtmlItem* HtmlItem::getNextItem()
{
	HtmlItem* item = NULL;

	if (m_children.count())
	{
	    HtmlItemList::Iterator iter = m_children.begin();
	    item = *iter;
	}
	else
	{
	    item = getNextItem(this);
	}
	
	if(NULL == item)
	{
	    item = this;
	}

	return item;
}

HtmlItem* HtmlItem::getPreItem(HtmlItem* currentItem)
{
	HtmlItem* item = currentItem;
    if (item->m_children.count())
    {
        HtmlItemList::Iterator iter = item->m_children.end();
        item = *(--iter);
    }

    return item;
}

HtmlItem* HtmlItem::getNextItem(HtmlItem* currentItem)
{
	HtmlItem* item = NULL;
	if (NULL != currentItem->m_parent)
	{
		// 找自己所在的下一个兄弟节点
	    HtmlItemList::Iterator iter = currentItem->m_parent->m_children.begin();
	    HtmlItemList::Iterator iterEnd = currentItem->m_parent->m_children.end();
		for (; iter != iterEnd; ++iter)
		{
			if (*iter == currentItem)
			{
			    if((++iter) != iterEnd)
			    {
					item = *iter;					
			    }
			    
			    break;
			}
		}

		if (NULL == item)
		{
		    item = getNextItem(currentItem->m_parent);
	    }
	}

	return item;
}

void HtmlItem::itemCenter(RenderContext& rc)
{
	if (getStyle()->m_textAlignement == util::LGraphicsContext::TextCenter)
	{
	    if(m_x < rc.getNewLineXStart() + rc.getMaxWidth()/2)
	    {
	        m_x = rc.getNewLineXStart() + rc.getMaxWidth()/2;
	    }
	}
}

LBool HtmlItem::isSelectable()
{
    return m_selectable;	
}

void HtmlItem::setScrollPos(util::LInt x, util::LInt y)
{
	util::LBoxItem::setScrollPos(x, y);
	
    HtmlItemList::Iterator iter = m_children.begin();
    HtmlItemList::Iterator iterEnd = m_children.end();
	for (; iter != iterEnd; ++iter)
	{
	    (*iter)->setScrollPos(x, y);
	}
}

LBool HtmlItem::isImage() const
{
    return LFalse;	
}

LBool HtmlItem::isStyle() const
{
    return LFalse;	
}

LBool HtmlItem::isJavaScript() const
{
    return LFalse;	
}

LBool HtmlItem::isBlockItem() const
{
    return LFalse;	
}

BlockItem* HtmlItem::getContainingBlock() const
{
	// 根据position的不同，所取的containing block的方式也不一样
	HtmlItem* o = getParent();
	if (!isText() && getStyle()->m_positionType == util::Style::FIXEDPOSITION)
	{
		while (o && !o->isViewRoot() && !(o->hasTransform() && o->isBlockItem()))
			o = o->getParent();
	}
	else if (!isText() && getStyle()->m_positionType == util::Style::ABSOLUTEPOSITION)
	{
		while (o && (o->getStyle()->m_positionType == util::Style::STATICPOSITION || (o->isInline() && !o->isReplaced())) && !o->isViewRoot() && !(o->hasTransform() && o->isBlockItem()))
		{
			if (o->getStyle()->m_positionType == util::Style::RELATIVEPOSITION && o->isInline() && !o->isReplaced())
			{
				BlockItem* relPositionedInlineContainingBlock = o->getContainingBlock();
				while (relPositionedInlineContainingBlock->isAnonymousBlock())
                    relPositionedInlineContainingBlock = relPositionedInlineContainingBlock->getContainingBlock();
                return relPositionedInlineContainingBlock;
			}

			o = o->getParent();
		}
	}
	else 
	{
        while (o && ((o->isInline() && !o->isReplaced()) || !o->isBlockItem()))
            o = o->getParent();
    }

    if (!o || !o->isBlockItem())
        return NULL; // This can still happen in case of an orphaned tree

	return static_cast<BlockItem*>(o);
}

HtmlItem* HtmlItem::getParent() const
{
	return m_parent;
}

void HtmlItem::setParent(HtmlItem* o)
{
	m_parent = o;
}

LBool HtmlItem::isViewRoot() const
{
	return m_isViewRoot;
}

void HtmlItem::setIsViewRoot(LBool isViewRoot)
{
	m_isViewRoot = isViewRoot;
}

void HtmlItem::layout()
{
}

void HtmlItem::addChild(HtmlItem* child)
{
	m_children.push_back(child);
}

LBool HtmlItem::hasTransform() const
{
	return LTrue;
}

LBool HtmlItem::isReplaced() const
{
	return LFalse;
}

LBool HtmlItem::isInline() const
{
    return LFalse;
}

LBool HtmlItem::isFloatingOrPositioned() const
{
	return isFloating() || isPositioned();
}

LBool HtmlItem::isFloating() const
{
	return (getStyle()->m_floatType != util::Style::FLOAT_NONE) && !isPositioned();
}

LBool HtmlItem::isPositioned() const
{
	return getStyle()->m_positionType == util::Style::ABSOLUTEPOSITION || getStyle()->m_positionType == util::Style::FIXEDPOSITION;
}

LBool HtmlItem::isRelPositioned() const
{
	return getStyle()->m_positionType == util::Style::RELATIVEPOSITION;
}

LBool HtmlItem::isTableCell() const
{
	return LFalse;
}

LBool HtmlItem::hasOverflowClip() const 
{ 
	return m_hasOverflowClip; 
}

LBool HtmlItem::isInlineBlockOrInlineTable() const
{
	return LFalse;
}

util::Style* HtmlItem::getStyle() const
{
	return m_style.get();
}

LBool HtmlItem::isWritingModeRoot() const
{ 
	return !getParent() || getParent()->getStyle()->writingMode() != getStyle()->writingMode(); 
}

LayoutUnit HtmlItem::marginAfter() const
{
	switch (getStyle()->writingMode()) 
	{
	case util::Style::TopToBottomWritingMode:
        return getStyle()->marginBottom();
    case util::Style::BottomToTopWritingMode:
        return getStyle()->marginTop();
    case util::Style::LeftToRightWritingMode:
        return getStyle()->marginRight();
    case util::Style::RightToLeftWritingMode:
        return getStyle()->marginLeft();
    }

    return getStyle()->marginBottom();
}

LayoutUnit HtmlItem::marginBefore() const
{
	switch (getStyle()->writingMode()) 
	{
    case util::Style::TopToBottomWritingMode:
        return getStyle()->marginTop();
    case util::Style::BottomToTopWritingMode:
        return getStyle()->marginBottom();
    case util::Style::LeftToRightWritingMode:
        return getStyle()->marginLeft();
    case util::Style::RightToLeftWritingMode:
        return getStyle()->marginRight();
    }

    return getStyle()->marginTop();
}

LayoutUnit HtmlItem::borderBefore() const 
{ 
	return getStyle()->borderBeforeWidth(); 
}

LayoutUnit HtmlItem::borderAfter() const 
{ 
	return getStyle()->borderAfterWidth(); 
}

LayoutUnit HtmlItem::paddingBefore(bool includeIntrinsicPadding) const
{
	/*LayoutUnit w = 0;
    Length padding = style()->paddingBefore();
    if (padding.isPercent())
        w = containingBlock()->availableLogicalWidth();
    return padding.calcMinValue(w);*/

	return getStyle()->paddingBefore();
}

LayoutUnit HtmlItem::paddingAfter(bool includeIntrinsicPadding) const
{
	return getStyle()->paddingAfter();
}

void HtmlItem::setLogicalHeight(LayoutUnit size)
{
    if (getStyle()->isHorizontalWritingMode())
        setHeight(size);
    else
        setWidth(size);
}

LayoutUnit HtmlItem::logicalHeight() const 
{ 
	return getStyle()->isHorizontalWritingMode() ? getHeight() : getWidth(); 
}

LayoutUnit HtmlItem::borderAndPaddingStart() const 
{ 
	return getStyle()->borderStartWidth() + getStyle()->paddingStart(); 
}

LayoutPoint HtmlItem::getAbsoluteContainerTopLeft() const
{
	// 使用getContainingBlock来寻找绝对的topleft坐标有个好处，
	// 就是当position处于不同状态时, 能够取到正确的Container
	BlockItem* container = getContainingBlock();
	LayoutUnit top = 0;
	LayoutUnit left = 0;
	while (container != NULL && container != this)
	{
		top  += container->getXpos();
		left += container->getYpos();
		container = container->getContainingBlock();
	}

	return LayoutPoint(top, left);
}

void HtmlItem::setDocument(HtmlDocument* doc)
{
    m_doc = doc;
}

HtmlDocument* HtmlItem::getDocument() const
{
    return m_doc;
}

LBool HtmlItem::hitTest(const LayoutPoint& point)
{
	HtmlItemList::Iterator iter = m_children.begin();
    HtmlItemList::Iterator iterEnd = m_children.end();
	for (; iter != iterEnd; ++iter)
	{
		if((*iter)->hitTest(point))
			return LTrue;
	}

	return LFalse;
}

}

