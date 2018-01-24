#ifndef Document_h
#define Document_h

#include "UtilString.h"
#include "HtmlDocument.h"
#include "CssParser.h"
#include "IItemListener.h"

namespace yanbo
{
class Document
{
public:
	enum DocType
	{
        ETinyDoc,
        ETiny2Doc,
        EExpatDoc,
	};
	static Document* create(DocType type);

public:
	virtual ~Document() {};
	virtual void createDocument(
			const StringA& buffer,
			HtmlDocument* doc,
			util::CssParser* cssParser,
			IItemListener* itemListener) = 0;
};
}
#endif
