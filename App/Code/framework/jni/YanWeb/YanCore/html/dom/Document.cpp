#include "Document.h"
#include "Tiny2Dom.h"
#include "TinyDom.h"
#include "ExpatDom.h"

namespace yanbo
{
Document* Document::create(DocType type)
{
	Document* document = NULL;
    switch (type)
    {
    case ETinyDoc:
    	document = new TinyDom();
    	break;
    case ETiny2Doc:
    	document = new Tiny2Dom();
    	break;
    case EExpatDoc:
    	document = new ExpatDom();
    	break;
    }

    return document;
}
}
