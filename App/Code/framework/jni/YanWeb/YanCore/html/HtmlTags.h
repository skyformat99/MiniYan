#ifndef HtmlTags_h
#define HtmlTags_h

#include "kmap.h"
#include "UtilString.h"

namespace yanbo
{

typedef KMap<LUint, LInt> HtmlTagMap;

class HtmlTags
{
public:
	typedef enum HtmlType
	{
		TAGNONE = 0,
		HTML,
		HEAD,
		META,
		BR,
		UL,
		DIV,
		TR,
		TD,
		LI,
		P,
		H1,
		H2,
		H3,
		H4,
		STRONG,
		B,
		A,
		PRE,
		CENTER,
		FONT,
		IMG,
		I,
		EM,
		STYLE,
		SCRIPT,
		FORM,
		INPUT,
		SELECT,
		BUTTON,
		TEXTAREA,
		BODY,
		TABLE,
		HR,
		TITLE,
		OBJECT,
		OPTION,
		SPAN,
		LINK,
		BASE,
		GAME,
		VIDEO,
	} HtmlType;
	
public:
	static HtmlTags* getInstance();
	static void destroyInstance();
	
	void defineInitialSymbol(const StringA& htmlText, LInt htmlType);
	~HtmlTags();
	
	int symbolAsInt(StringA& inSymbol);
	void clear();
	
private:
	HtmlTags();
	
private:
	static HtmlTags* m_htmlTags;
	HtmlTagMap m_map;
};

}
#endif
