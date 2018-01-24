#ifndef HtmlItem_h
#define HtmlItem_h

// all render base class
#include "LGdi.h"
#include "KList.h"
#include "LBoxItem.h"
#include "IItemListener.h"
#include "HtmlTags.h"
#include "CssManager.h"

namespace yanbo
{

class HtmlItem;
class RenderContext;
class HtmlDocument;
class BlockItem;
typedef KList<HtmlItem*> HtmlItemList;
typedef LRect LayoutRect;
typedef LInt  LayoutUnit;
typedef LPoint LayoutPoint;

class HtmlItem : public util::LBoxItem
{
public:
	HtmlItem(IItemListener* itemListener, 
			const StringA& id,
			const StringA& name,  
			LBool selectable);
	
	virtual ~HtmlItem();
	
public:
	virtual void layout(RenderContext& rc);
	
	// prepare to reconstruct the layout
	virtual void layout();

	virtual void paint(util::LGraphicsContext& dc);
	virtual void paintBorder(util::LGraphicsContext& gc, util::Border& border, LayoutUnit x, LayoutUnit y);
	virtual LBool isText() const;
	virtual LBool isLink() const;
	virtual LBool isBlockItem() const;
	virtual LBool isImage() const;
	virtual LBool isStyle() const;
	virtual LBool isJavaScript() const;
	virtual void execute();
	virtual void setStyle(util::CssManager* manager, util::CssRule* parentRule);
	virtual void setScrollPos(LInt x, LInt y);
	
	void setId(const StringA& id);
	const StringA& getId() const;
	
	void setTagName(const StringA& tagName);
	const StringA& getTagName() const;
	
	void setClassName(const StringA& className);
	const StringA& getClassName() const;
	
	void setName(const StringA& name);
	const StringA& getName() const;
	
	void setTagType(HtmlTags::HtmlType tagType);
	HtmlTags::HtmlType getTagType() const;
	
	void setSelected(const LBool selected);
	LBool isSelected();
	LBool isSelectable();
	
	void setOnClick(const StringA& onClick);
	const StringA& getOnClick() const;
	
	void itemCenter(RenderContext& rc);
	void layoutInline(RenderContext& rc);
	
	HtmlItem* getPreItem();
	HtmlItem* getNextItem();

	// get the near container,<div><p><a/></p></div>
	// such as above, get the p container
	//HtmlItem* getNearContainer();
	BlockItem* getContainingBlock() const;

	LBool isViewRoot() const;
	void setIsViewRoot(LBool isViewRoot);

	LBool hasTransform() const;

	virtual LBool isReplaced() const;

	virtual LBool isInline() const;

	virtual void addChild(HtmlItem* child);

	void setParent(HtmlItem* o);
	HtmlItem* getParent() const;

	LBool isFloatingOrPositioned() const;
	LBool isFloating() const;
	LBool isPositioned() const;
	LBool isRelPositioned() const;
	LBool isTableCell() const;
	LBool hasOverflowClip() const;
	virtual LBool isInlineBlockOrInlineTable() const;
	util::Style* getStyle() const;

	virtual LayoutUnit marginAfter() const;
	virtual LayoutUnit marginBefore() const;
	virtual LayoutUnit borderBefore() const;
	virtual LayoutUnit borderAfter() const;
	virtual LayoutUnit paddingBefore(bool includeIntrinsicPadding = true) const;
	virtual LayoutUnit paddingAfter(bool includeIntrinsicPadding = true) const;

	LBool isWritingModeRoot() const;
	void setLogicalHeight(LayoutUnit size);
	LayoutUnit logicalHeight() const;

	LayoutUnit borderAndPaddingStart() const;

	//void computeAndSetBlockDirectionMargins(const BlockItem* containingBlock);

	//void computeBlockDirectionMargins(const BlockItem* containingBlock, LayoutUnit& marginBefore, LayoutUnit& marginAfter) const;

	LayoutPoint getAbsoluteContainerTopLeft() const;
	void setDocument(HtmlDocument* doc);
	HtmlDocument* getDocument() const;

	virtual LBool hitTest(const LayoutPoint& point);

private:
	HtmlItem* getPreItem(HtmlItem* currentItem);
	HtmlItem* getNextItem(HtmlItem* currentItem);	
	
protected:
	void handleXYPos(RenderContext& rc);
	
public:	
	HtmlItemList     m_children;
	HtmlItem*        m_parent;
	
protected:
    /**
     * The listener who will receive event callbacks.
     */
	IItemListener*            m_itemListener;
    /**
     * Standard Html and XHTML attribute. A unique id for the element. Used with CSS or JavaScript
     */
	StringA                   m_id;
    
	/**
     * Many html items like input, links etc has a name attribute
     */
	StringA                   m_name;
	// html type
	HtmlTags::HtmlType        m_type;
	
    /** The maximum width (in pixels) this item can have */
    LInt                      m_maxWidth; //the maximum width this item is allowed to have.
    
    /** true if this htmlitem can be selected (highlighted) by the user */
    LBool                     m_selectable;
    
    /**  True if this item is selected */
    LBool                     m_selected;
    
    /** the url this htmlitem is linking to. (used for image items, link items etc */
    StringA                   m_url;
    
    /**Rendered line number, NOT source line number. */
    LInt                      m_lineNumber;
    
    HtmlDocument*             m_doc;
    
    StringA                   m_tagName;
    
    StringA                   m_className;
    
    StringA                   m_onClick;

	LBool                     m_isViewRoot;
	LBool                     m_hasOverflowClip;
};

}

#endif
