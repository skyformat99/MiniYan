/*
 * ImageItem.cpp
 *
 *  Created on: 2011-11-14
 *      Author: Administrator
 */

#include "ImageItem.h"
#include "LColor.h"
#include "StringUtils.h"
#include "ResourceLoader.h"
#include "RenderContext.h"
#include "SalLog.h"

namespace yanbo
{

ImageItem::ImageItem(
		IItemListener* itemListener,
		const StringA& id,
		const StringA& name,
		LBool selectable,
		StringA src)
    : InlineItem(itemListener, id, name, LFalse)
    , m_bitmap(NULL)  // img item can't be selected
    , m_src(src)
{
	
}

ImageItem::~ImageItem()
{
	if (m_bitmap != NULL)
	{
	    delete m_bitmap;
	    m_bitmap = NULL;
	}
}

void ImageItem::setImage(util::LBitmap* bitmap)
{
	m_bitmap = bitmap;
	m_width = getStyle()->width() > 0 ?
			getStyle()->width() : m_bitmap->size().iWidth;
	m_height = getStyle()->height() > 0 ?
			getStyle()->height() : m_bitmap->size().iHeight;
}

void ImageItem::execute()
{
	
}

void ImageItem::paint(util::LGraphicsContext& gc)
{
	LayoutPoint topLeft = getAbsoluteContainerTopLeft();
	int x = topLeft.iX + getXpos();
	int y = topLeft.iY + getYpos();
	KFORMATLOG("ImageItem::paint topLeft.iX=%d", topLeft.iY);
	KFORMATLOG("ImageItem::paint getXpos()=%d", getXpos());
	KFORMATLOG("ImageItem::paint x=%d", x);
	if (NULL != m_bitmap)
	{
		gc.setPenColor(util::LColor::parseRgbInt(COLOR_LINKCOLOR));
		//gc.drawRect(x, y - m_scrollY, m_width + 1, m_height + 1);
	    //gc.drawBitmap(util::LPoint(x, y), m_bitmap);
		KLOG("ImageItem::paint drawBitmap begin");
		gc.drawBitmap(util::LRect(x, y, m_width, m_height), m_bitmap);
		KLOG("ImageItem::paint drawBitmap end");
	}	
}

void ImageItem::layout(RenderContext& rc)
{
	KLOG("ImageItem::layout begin");
	if (NULL != m_bitmap)
	{
		rc.addLineItem(this);
		handleXYPos(rc);
		if (m_x + m_width > rc.getMaxWidth())
		{
			rc.addY(m_height);
			rc.newLine(this);	    
		}
		else
		{
			rc.addY(m_height);
		}
	    
		KFORMATLOG("Image width=%d", m_width);
		KFORMATLOG("Image height=%d", m_height);
	}
	else
	{
		KLOG("ImageItem::layout end0");
		util::LBitmap* bitmap = util::LBitmap::create();
		KLOG("ImageItem::layout end1");
		bitmap->load(m_src, 0);
		KLOG("ImageItem::layout end2");
		setImage(bitmap);
		KLOG("ImageItem::layout end3");

		layout(rc);

		KLOG("ImageItem::layout end");
	}
}

LBool ImageItem::isImage()
{
    return LTrue;	
}

const StringA& ImageItem::getImageUrl() const
{
	return m_src;
}

}
