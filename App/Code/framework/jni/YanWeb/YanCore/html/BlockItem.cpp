/*
 * BlockItem.cpp
 *
 *  Created on: 2011-7-1
 *      Author: yanbo
 *   Modify on: 2012-8-03
 *  Description: All Copyright reserved
 */

#include "BlockItem.h"
#include "LColor.h"
#include "RenderContext.h"
#include "SalLog.h"
#include "HtmlDocument.h"

namespace yanbo
{

BlockItem::BlockItem(IItemListener* itemListener,
		const StringA& id,
		const StringA& name,
		LBool selectable)
    : HtmlItem(itemListener, id, name, LFalse)// block item can't be selected
    , m_isChildrenInline(LTrue)
    , m_isAnonymous(LFalse)
	, m_positionedObjects(NULL)
{
}

BlockItem::~BlockItem()
{
	if (m_positionedObjects != NULL)
	{
		PositionedObjectsList::Iterator iter = m_positionedObjects->begin();
		PositionedObjectsList::Iterator endIter = m_positionedObjects->end();

		for (; iter != endIter; ++iter)
		{
			delete *iter;
		}

		//m_positionedObjects->clear();
		delete m_positionedObjects;
	}
}

void BlockItem::setChildrenInline(LBool isInline)
{
	m_isChildrenInline = isInline;
}

LBool BlockItem::isChildrenInline()
{
	return m_isChildrenInline;
}


void BlockItem::paint(util::LGraphicsContext& gc)
{
	if (getStyle()->m_displayType == util::Style::DISPLAY_NONE)
	{
	    return;
	}
	
	if (m_type == HtmlTags::HR)
    {
		// 绘制时要取出绝对坐标进行绘制。
	    LayoutPoint topLeft = getAbsoluteContainerTopLeft();
		int x = topLeft.iX + getXpos();
		int y = topLeft.iY + getYpos();

		gc.setPenColor(util::LColor::parseRgbInt(COLOR_GRAY));
		gc.setPenStyle(util::LGraphicsContext::SolidPen);
		gc.drawLine(x, y + m_height/2 - m_scrollY - 1, x + m_width, y + m_height/2 - m_scrollY - 1);

		gc.setPenColor(util::LColor::parseRgbInt(COLOR_LIGHTGRAY));
		gc.setPenStyle(util::LGraphicsContext::SolidPen);
		gc.drawLine(x, y + m_height/2 - m_scrollY, x + m_width, y + m_height/2 - m_scrollY);
    }
    else
    {
        HtmlItem::paint(gc);
    }
}

LBool BlockItem::isBlockItem() const
{
    return getStyle()->m_displayType == util::Style::DISPLAY_INLINE ? LFalse : LTrue;
}

void BlockItem::layout()
{
	if (getStyle()->m_width)
	{
		m_width = getStyle()->m_width;
	}
	else
	{
		m_width = getParent() ? getParent()->getWidth() : m_doc->getViewPort().GetWidth();
	}

	layoutBlock(LFalse);
}

void BlockItem::layoutBlock(LBool relayoutChildren)
{
	LayoutUnit maxFloatLogicalBottom = 0;

	if (isChildrenInline())
	{
		layoutInlineChildren();
	}
	else
	{
	    layoutBlockChildren(relayoutChildren, maxFloatLogicalBottom);
	}
}

void BlockItem::layoutBlockChildren(LBool relayoutChildren, LayoutUnit& maxFloatLogicalBottom)
{
	HtmlItemList::Iterator iter = m_children.begin();
    HtmlItemList::Iterator iterEnd = m_children.end();

	LayoutUnit previousFloatLogicalBottom = 0;
	for (; iter != iterEnd; ++iter)
	{
		HtmlItem* o = *iter;
		if (layoutSpecialChild(o, previousFloatLogicalBottom))
		{
            continue;
		}

		layoutBlockChild(o, previousFloatLogicalBottom);
	}

	m_height = previousFloatLogicalBottom;
}

void BlockItem::layoutInlineChildren()
{
	int tmpX = 0;
	int tmpY = 0;
	
	RenderContext rc;

	rc.setMaxWidth(m_doc->getViewPort().GetWidth());
	rc.setNextLineHeight(0);
	
	LBool center = (getStyle()->m_positionType == util::Style::STATICPOSITION
				       && getStyle()->m_textAlignement == util::LGraphicsContext::TextCenter) ? LTrue
			           : LFalse;
	rc.setCenter(center);
	// add top padding
	rc.addX(getStyle()->paddingLeft());
	rc.addY(getStyle()->paddingTop());
		
	HtmlItemList::Iterator iter = m_children.begin();
	HtmlItemList::Iterator iterEnd = m_children.end();
	for (; iter != iterEnd; ++iter)
	{
	    (*iter)->layout(rc);
	}
	
	rc.newLine(this);// 换行
	rc.setNextLineHeight(0);

	rc.addY(getStyle()->m_bottomPadding);
	if (rc.getY() - tmpY > getStyle()->m_height)
	{
		m_height = rc.getY() - tmpY;
	}
	else
	{
		m_height = getStyle()->m_height;
	}
}

LBool BlockItem::layoutSpecialChild(HtmlItem* child, LayoutUnit& currentLogicBottom)
{
    return layoutPositionChild(child);
}

void BlockItem::layoutBlockChild(HtmlItem* child, LayoutUnit& currentLogicBottom)
{
	int y = currentLogicBottom + child->getStyle()->marginTop();
	int x = child->getStyle()->marginLeft();
	child->setXpos(x);
	child->setYpos(y);
	child->layout();
	currentLogicBottom += child->getHeight();
}

LBool BlockItem::layoutPositionChild(HtmlItem* child)
{
	if (child->isPositioned())
	{
        child->getContainingBlock()->insertPositionedObject(child);
        adjustPositionedBlock(child);
        return LTrue;
    }

    return LFalse;
}

void BlockItem::addChild(HtmlItem* child)
{
	addChild(child, LTrue);
}

void BlockItem::addChild(HtmlItem* child, LBool isNotAnonymousBlock)
{
	if (isNotAnonymousBlock)
	{
		if (isChildrenInline() && child->isBlockItem())
		{
			HtmlItem::addChild(child);
			setChildrenInline(LFalse);
			makeChildrenNonInline(child);
		}
		else if (!isChildrenInline() && child->isInline())
		{
			HtmlItemList::Iterator iter = m_children.end();
		    HtmlItem* lastChild = *(--iter);
			if (lastChild && lastChild->isBlockItem())
			{
				BlockItem* block = static_cast<BlockItem*>(lastChild);
				if (block->isAnonymousBlock())
				{
					block->addChild(child, LFalse);
					child->setParent(block);
					return;
				}
			}

			BlockItem* b = createAnonymousBlock();
			b->setDocument(getDocument());
			m_children.push_back(b);
			b->setParent(this);
			b->addChild(child, LFalse);
			child->setParent(b);
		}
		else
		{
			HtmlItem::addChild(child);
		}

	}
	else
	{
		HtmlItem::addChild(child);
	}
}

void BlockItem::makeChildrenNonInline(HtmlItem* block)
{
	BlockItem* b = createAnonymousBlock();
	b->setDocument(getDocument());
	HtmlItemList::Iterator iter = m_children.begin();
    HtmlItemList::Iterator iterEnd = m_children.end();
	HtmlItemList::Iterator tmpIter;
	for (; iter != iterEnd;)
	{
		tmpIter = iter;
		++iter;
		if (*tmpIter != block)
		{
			b->addChild(*tmpIter, LFalse);
			(*tmpIter)->setParent(b);
			m_children.erase(tmpIter);			
		}
		else
		{
			break;
		}	
	}

	b->setParent(this);
	m_children.insert(tmpIter, b);
}

BlockItem* BlockItem::createAnonymousBlock()
{
	BlockItem* b = new BlockItem(NULL, _CS(""), _CS(""), LFalse);
	b->setIsAnonymousBlock(LTrue);
	return b;
}

LBool BlockItem::isAnonymousBlock()
{
	return m_isAnonymous;
}

void BlockItem::setIsAnonymousBlock(LBool isAnonymous)
{
	m_isAnonymous = isAnonymous;
}

void BlockItem::insertPositionedObject(HtmlItem* o)
{
    //if (o->isRenderFlowThread())
    //    return;
    
    // Create the list of special objects if we don't aleady have one
    if (!m_positionedObjects)
    {
        m_positionedObjects = new PositionedObjectsList;
    }

	m_positionedObjects->push_back(o);
}

void BlockItem::adjustPositionedBlock(HtmlItem* child)
{
	child->setXpos(child->getStyle()->left());
	child->setYpos(child->getStyle()->top());
	child->layout();
	KFORMATLOG("adjustPositioned, x=%d, y=%d", child->getStyle()->left(), child->getStyle()->top());
}


}
