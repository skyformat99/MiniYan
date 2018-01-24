/*
 * ExpatDom.cpp
 *
 *  Created on: 2015-3-9
 *      Author: yanbo
 */

#include "ExpatDom.h"
#include "HtmlTags.h"
#include "expat.h"
#include "TextItem.h"
#include "InputItem.h"
#include "LinkItem.h"
#include "ImageItem.h"
#include "SalLog.h"
#include "StringUtils.h"
#include "MiniCore.h"

namespace yanbo
{
static void XMLCALL startElement(void *userData, const char *name, const char **atts)
{
	ExpatDom* expatDom = static_cast<ExpatDom*>(userData);
	HtmlTags* htmlTags = HtmlTags::getInstance();
	StringA tagType = (const LUint8*)name;
	int type = htmlTags->symbolAsInt(tagType);
	Stack<HtmlItem*>* stack = expatDom->getItemStack();
	HtmlItem* parent = stack->size() > 0 ? stack->elementAt(stack->size()-1) : NULL;
	HtmlItem* item = expatDom->createHtmlItem(type, name, atts, parent);
	stack->push(item);
	KLOG("startElement NAME=%s", name);
}

static void XMLCALL endElement(void *userData, const char *name)
{
	ExpatDom* expatDom = static_cast<ExpatDom*>(userData);
	if (expatDom->getText().GetLength() > 0)
	{
		expatDom->createTextItem(expatDom->getText().GetBuffer(), expatDom->getText().GetLength());
		expatDom->clearText();
	}

	HtmlItem* item = expatDom->getItemStack()->pop();

//	if (item->getTagType() == HtmlTags::STYLE
//			|| item->getTagType() == HtmlTags::SCRIPT
//			|| item->getTagType() == HtmlTags::TITLE)
//	{
//        delete item;
//        item = NULL;
//	}
}

static void XMLCALL characters(void *userData, const char *text, int len)
{
	if (len == 0)
	{
		return;
	}

	CString cText(text, len);

	KLOG("characters text=%s", cText.GetBuffer());
	ExpatDom* expatDom = static_cast<ExpatDom*>(userData);
	expatDom->appendText(cText);
	//expatDom->createTextItem(text, len);
	KLOG("characters text end");
}

ExpatDom::ExpatDom()
    : m_htmlDoc(NULL)
    , m_cssParser(NULL)
    , m_itemListener(NULL)
    , m_itemStack(NULL)
{
}

ExpatDom::~ExpatDom()
{
	if (m_itemStack != NULL)
	{
		delete m_itemStack;
		m_itemStack = NULL;
	}
}

void ExpatDom::appendText(const CString& text)
{
	m_text += text;
}

void ExpatDom::clearText()
{
	m_text = (const char*)NULL;
}

const CString& ExpatDom::getText() const
{
	return m_text;
}

Stack<HtmlItem*>* ExpatDom::getItemStack() const
{
	return m_itemStack;
}

HtmlItem* ExpatDom::createTextItem(const char* aText, int len)
{
	Stack<HtmlItem*>* stack = getItemStack();
	HtmlItem* parentElem = stack->size() > 0 ? stack->elementAt(stack->size()-1) : NULL;
	HtmlItem* item = NULL;
	if (NULL != parentElem)
	{
		//StringA tagType = (const LUint8*)parentElem->Value();
		//int type = htmlTags->symbolAsInt(tagType);
		StringA text((const LUint8*)aText, len);
		if (parentElem->getTagType() == HtmlTags::STYLE)
		{
			util::InputStream is(text);
			m_cssParser->parseCss(is);
		}
		else if (parentElem->getTagType() == HtmlTags::TITLE)
		{
		    m_htmlDoc->setPageTitle(text);
		}
		else if (parentElem->getTagType() == HtmlTags::SCRIPT)
		{
			if (text.GetLength() > 0)
			{
				compileCode((char*)(text.GetBuffer()));
				KLOG("compile finished");
				//KSTRFORMAT("elem->Value()=%s", text);
				int ret = 0;
				callFunction(const_cast<char*>("main()"), &ret);
			}
		}
		else // textitem
		{
			util::StringUtils::replaceSpecialChars(text);
		    if (text.GetLength() > 0)
		    {
				item = new TextItem(m_itemListener, _CS(""), _CS(""), text, LFalse);
				item->m_parent = parentElem;
				if (NULL != parentElem)
				{
					parentElem->addChild(item);
				}
		    }
		}
	}

	if (NULL != item)
	{
	    m_htmlDoc->addHtmlItem(item);
		item->setDocument(m_htmlDoc);
	}

	return item;
}

void ExpatDom::createDocument(
			const StringA& buffer,
			HtmlDocument* doc,
			util::CssParser* cssParser,
			IItemListener* itemListener)
{
	m_htmlDoc = doc;
	m_itemListener = itemListener;
	m_cssParser = cssParser;
	m_itemStack = new Stack<HtmlItem*>();
	XML_Parser parser = XML_ParserCreate(NULL);

	XML_SetUserData(parser, this);
	XML_SetElementHandler(parser, &startElement, &endElement);
	{
	    XML_SetCharacterDataHandler(parser, &characters);
	}

	KLOG("XML_Parse");
	XML_Parse(parser, (const LCharA*)buffer.GetBuffer(), buffer.GetLength(), 0);
	KLOG("XML_Parse End");
}

void ExpatDom::getItemAttribute(
		StringA& sAttrValue,
		const char* cAttrName,
		const char** cAttr)
{
	for (int i = 0; cAttr[i] != 0; i += 2)
	{
	    const char* name = cAttr[i];
	    const char* value = cAttr[i+1];
	    if (0 == util::LStrcmp(cAttrName, name))
	    {
	    	sAttrValue = (const LUint8*)value;
	    	break;
	    }
	}
}

HtmlItem* ExpatDom::createHtmlItem(
		int type,
		const char* name,
		const char **atts,
		HtmlItem* parent)
{
    StringA tagType = (const LUint8*)name;
    StringA tagId = _CS("");
    StringA tagName = _CS("");
    StringA tagClass = _CS("");

    getItemAttribute(tagId, "id", atts);
    getItemAttribute(tagName, "name", atts);
    getItemAttribute(tagClass, "class", atts);

	HtmlItem* item = NULL;
	switch (type)
	{
	case HtmlTags::HTML:
		{
		}
		break;
	case HtmlTags::TITLE:
		{
			item = new HtmlItem(m_itemListener, tagId, tagName, LFalse);
			item->setTagType((HtmlTags::HtmlType)type);
		}
		break;
	case HtmlTags::HEAD:
		{

		}
		break;
	case HtmlTags::LINK:
		{

		}
		break;
	case HtmlTags::META:
		{

		}
		break;
	case HtmlTags::BASE:
		{
		}
		break;
	case HtmlTags::STYLE:
		{
			item = new HtmlItem(m_itemListener, tagId, tagName, LFalse);
			item->setTagType((HtmlTags::HtmlType)type);
		}
		break;
	case HtmlTags::SCRIPT:
		{
			item = new HtmlItem(m_itemListener, tagId, tagName, LFalse);
			item->setTagType((HtmlTags::HtmlType)type);
		}
		break;
	// only body and its child can create render, render tree root is body item
	case HtmlTags::BODY:
		{
		    item = new BlockItem(m_itemListener, tagId, tagName, LFalse);
			item->m_parent = parent;
			item->setClassName(tagClass);
			item->setTagName(tagType);
			item->setIsViewRoot(LTrue);
			item->setTagType((HtmlTags::HtmlType)type);
			if (NULL != parent)
			{
			    parent->addChild(item);
			}

			m_htmlDoc->setRenderTreeRoot(item);
		}
		break;
	case HtmlTags::FORM:
		{
			StringA tagAction = _CS("");
			getItemAttribute(tagAction, "action", atts);

			StringA tagMethod = _CS("");
			getItemAttribute(tagMethod, "method", atts);

			util::StringA tagEncoding = _CS("");
			getItemAttribute(tagEncoding, "enctype", atts);

			item = new HtmlForm(m_itemListener,
					            tagId,
					            tagName,
					            tagAction,
					            tagMethod,
					            tagEncoding,
					            LFalse);

			item->m_parent = parent;
			item->setClassName(tagClass);
			item->setTagName(tagType);
			item->setTagType((HtmlTags::HtmlType)type);
			if (NULL != parent)
			{
				parent->addChild(item);
			}
		}
		break;
    case HtmlTags::HR:
	case HtmlTags::DIV:
	case HtmlTags::P:
		{
			item = new BlockItem(m_itemListener, tagId, tagName, util::LFalse);
			item->m_parent = parent;
			item->setClassName(tagClass);
			item->setTagName(tagType);
			item->setTagType((HtmlTags::HtmlType)type);
			if (NULL != parent)
			{
			    parent->addChild(item);
			}
		}
		break;
	case HtmlTags::IMG:
		{
		    StringA imageSrc = _CS("");
		    getItemAttribute(imageSrc, "src", atts);

			item = new ImageItem(m_itemListener, tagId, tagName, util::LFalse, imageSrc);
			item->m_parent = parent;
			item->setClassName(tagClass);
			item->setTagName(tagType);
			item->setTagType((HtmlTags::HtmlType)type);
			if (NULL != parent)
			{
			    parent->addChild(item);
			}

		}
		break;
	case HtmlTags::A:
		{
			StringA tagUrl = _CS("");
			getItemAttribute(tagUrl, "href", atts);

			item = new LinkItem(m_itemListener, tagId, tagName, tagUrl);
			item->m_parent = parent;
			item->setClassName(tagClass);
			item->setTagName(tagType);
			item->setTagType((HtmlTags::HtmlType)type);
			if (NULL != parent)
			{
			    parent->addChild(item);
			}
		}
		break;
	case HtmlTags::INPUT:
		{
		    StringA tagValue = _CS("");
		    getItemAttribute(tagValue, "value", atts);

		    StringA tagTitle = _CS("");
		    getItemAttribute(tagValue, "title", atts);

		    StringA tagInputType = _CS("");
		    getItemAttribute(tagInputType, "type", atts);

		    StringA tagImageUrl = _CS("");
		    getItemAttribute(tagImageUrl, "src", atts);

			item = new InputItem(m_itemListener,
					tagId,
					tagName,
					tagValue,
					tagTitle,
					tagInputType,
					tagImageUrl,
					LFalse);

			item->m_parent = parent;
			item->setClassName(tagClass);
			item->setTagName(tagType);
			item->setTagType((HtmlTags::HtmlType)type);
			if (NULL != parent)
			{
				parent->addChild(item);
			}
		}
		break;
	default:
		{
			item = new InlineItem(m_itemListener, tagId, tagName, util::LFalse);
			item->m_parent = parent;
			item->setClassName(tagClass);
			item->setTagName(tagType);
			item->setTagType((HtmlTags::HtmlType)type);
			if (NULL != parent)
			{
			    parent->addChild(item);
			}
		}
		break;
	}

	if (NULL != item
			&& type != HtmlTags::TITLE
			&& type != HtmlTags::STYLE
			&& type != HtmlTags::SCRIPT)
	{
	    m_htmlDoc->addHtmlItem(item);
		item->setDocument(m_htmlDoc);
	}

	return item;
}

}
