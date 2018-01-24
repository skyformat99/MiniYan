#include "Tiny2Dom.h"
#include "TextItem.h"
#include "InputItem.h"
#include "LinkItem.h"
#include "ImageItem.h"
#include "SalLog.h"
#include "StringUtils.h"
#include "MiniCore.h"

namespace yanbo
{
Tiny2Dom::Tiny2Dom()
    : m_htmlDoc(NULL)
    , m_cssParser(NULL)
    , m_itemListener(NULL)
{
}

Tiny2Dom::~Tiny2Dom()
{
}

void Tiny2Dom::createDocument(
		const StringA& buffer,
		HtmlDocument* doc,
		util::CssParser* cssParser,
		IItemListener* itemListener)
{
	m_htmlDoc = doc;
	m_itemListener = itemListener;
	m_cssParser = cssParser;

	XMLDocument xmlDoc;

	xmlDoc.Parse((const LCharA*)buffer.GetBuffer());
	XMLElement *root = xmlDoc.RootElement();

	if (root != NULL)
	{
	    createRenderTree(root, NULL, NULL);
	}
}

void Tiny2Dom::createRenderTree(XMLNode* elem, XMLNode* parentElem, HtmlItem* parent)
{
	//KLOG("createRenderTree");
	HtmlItem* item = createHtmlItem(elem, parentElem, parent);
	XMLNode* child = NULL;

	for (child = elem->FirstChild(); child != NULL; child = child->NextSibling())
	{
	    createRenderTree(child, elem, item);
	}
}

HtmlItem* Tiny2Dom::createHtmlItem(XMLNode* node, XMLNode* parentElem, HtmlItem* parent)
{
	HtmlItem* item = NULL;
	HtmlTags* htmlTags = HtmlTags::getInstance();

	if (node->ToText() == NULL && node->ToComment() == NULL)
	{
		XMLElement* elem = node->ToElement();
		if (!elem)
		{
			return NULL;
		}
	    StringA tagType = _CS("");
	    StringA tagId = _CS("");
	    StringA tagName = _CS("");
	    StringA tagClass = _CS("");

	    const LCharA* tValue = elem->Value();
	    if (NULL != tValue)
	    {
	        tagType = (const LUint8*)tValue;
	    }

	    const LCharA* tId = elem->Attribute("id");
	    if (NULL != tId)
	    {
	        tagId = (const LUint8*)tId;
	        KLOG("tagId");
	        KSTRLOG8(tagId);
	    }

	    const LCharA* tName = elem->Attribute("name");
	    if (NULL != tName)
	    {
	        tagName = (const LUint8*)tName;
	    }

	    const LCharA* tClass = elem->Attribute("class");
	    if (NULL != tClass)
	    {
	        tagClass = (const LUint8*)tClass;
	    }

	    //KLOG("type");
	    //KSTRLOG8(tagType);
		int type = htmlTags->symbolAsInt(tagType);

		switch (type)
		{
		case HtmlTags::HTML:
			{
			}
			break;
		case HtmlTags::TITLE:
			{
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
			}
			break;
		case HtmlTags::SCRIPT:
			{

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
				const char* tAction = elem->Attribute("action");
				if (NULL != tAction)
				{
				    tagAction = (const LUint8*)tAction;
				}

				StringA tagMethod = _CS("");
				const char* tMethod = elem->Attribute("action");
				if (NULL != tMethod)
				{
				    tagMethod = (const LUint8*)tMethod;
				}

				util::StringA tagEncoding = _CS("");
				const char* tEncoding = elem->Attribute("enctype");
				if (NULL != tEncoding)
				{
				    tagEncoding = (const LUint8*)tEncoding;
				}

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
//		case HtmlTags::TEXTAREA:
//			{
//
//			}
//			break;
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
//		case HtmlTags::H1:
//		case HtmlTags::H2:
//		case HtmlTags::H3:
//		case HtmlTags::H4:
//			{
//
//			}
//			break;
//		case HtmlTags::BR:
//			{
//
//			}
//			break;

//			{
//
//			}
//			break;
		case HtmlTags::IMG:
			{
			    StringA imageSrc = _CS("");

				const char* src = elem->Attribute("src");
				if (NULL != src)
				{
				    imageSrc = (const LUint8*)src;
				}

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
				const char* tUrl = elem->Attribute("href");
				if (NULL != tUrl)
				{
					tagUrl = (const LUint8*)tUrl;
				}

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
			    const char* tValue = elem->Attribute("value");
			    if (NULL != tValue)
			    {
			        tagValue = (const LUint8*)tValue;
			    }

			    StringA tagTitle = _CS("");
			    const char* tTitle = elem->Attribute("value");
			    if (NULL != tTitle)
			    {
			        tagTitle = (const LUint8*)tTitle;
			    }

			    StringA tagInputType = _CS("");
			    const char* tInputType = elem->Attribute("type");
			    if (NULL != tTitle)
			    {
			        tagInputType = (const LUint8*)tInputType;
			    }

			    StringA tagImageUrl = _CS("");
			    const char* tImageUrl = elem->Attribute("src");
			    if (NULL != tTitle)
			    {
			        tagImageUrl = (const LUint8*)tImageUrl;
			    }

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
//		case HtmlTags::SELECT:
//			{
//
//			}
//			break;
//		case HtmlTags::OPTION:
//			{
//
//			}
//			break;
//		case HtmlTags::FONT:
//			{
//
//			}
//			break;
//		case HtmlTags::UL:
//			{
//
//			}
//			break;
//		case HtmlTags::STRONG:
//			{
//
//			}
//			break;
//		case HtmlTags::B:
//			{
//
//			}
//			break;
//		case HtmlTags::I:
//			{
//
//			}
//			break;
//		case HtmlTags::EM:
//			{
//
//			}
//			break;
//		case HtmlTags::LI:
//			{
//
//			}
//			break;
//		case HtmlTags::SPAN:
//			{
//
//			}
//			break;
//		case HtmlTags::TABLE:
//			{
//
//			}
//			break;
//		case HtmlTags::TR:
//			{
//
//			}
//			break;
//		case HtmlTags::TD:
//			{
//
//			}
//			break;
//		case HtmlTags::CENTER:
//			{
//
//			}
//			break;
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
	}
	else if (node->ToText() != NULL)// text
	{
		XMLText* elem = node->ToText();
		if (NULL != parentElem)
		{
			StringA tagType = (const LUint8*)parentElem->Value();
			int type = htmlTags->symbolAsInt(tagType);
			StringA text = (const LUint8*)elem->Value();
			if (type == HtmlTags::STYLE)
			{
				KLOG("TINYXML2 parseCss");
				util::InputStream is(text);
				m_cssParser->parseCss(is);
			}
			else if (type == HtmlTags::TITLE)
			{
			    m_htmlDoc->setPageTitle(text);
			}
			else if (type == HtmlTags::SCRIPT)
			{
				KLOG("TINYXML2 SCRIPT");
				KFORMATLOG("elem->Value()=%s and length=%d", text.GetBuffer(), text.GetLength());
				compileCode((char*)(text.GetBuffer()));
				KLOG("compile finished");
				int ret = 0;
				callFunction(const_cast<char*>("main()"), &ret);
			}
			else // textitem
			{
				util::StringUtils::replaceSpecialChars(text);
			    if (text.GetLength() > 0)
			    {
					item = new TextItem(m_itemListener, _CS(""), _CS(""), text, LFalse);
					item->m_parent = parent;
					if (NULL != parent)
					{
						parent->addChild(item);
					}
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
}
