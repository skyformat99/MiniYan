/*
 * ImageItem.h
 *
 *  Created on: 2011-6-30
 *      Author: yanbo
 */

#ifndef ImageItem_h
#define ImageItem_h

#include "InlineItem.h"

namespace yanbo
{

class ResourceLoader;
class ImageItem : public InlineItem
{
public:
	ImageItem(IItemListener* itemListener, 
			const StringA& id,
			const StringA& name,  
			LBool selectable,
			StringA src);
	
	~ImageItem();
	
public:
	void setImage(util::LBitmap* bitmap);
	virtual void execute();
	virtual LBool isImage();
	const StringA& getImageUrl() const;
	
	virtual void paint(util::LGraphicsContext& dc);
	virtual void layout(RenderContext& rc);
	
private:
	util::LBitmap* m_bitmap;
	StringA m_src;
};

}

#endif /* IMAGEITEM_H_ */
