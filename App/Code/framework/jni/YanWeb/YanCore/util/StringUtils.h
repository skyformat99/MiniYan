/*
 * StringUtils.h
 *
 *  Created on: 2011-6-25
 *      Author: yanbo
 */

#ifndef StringUtils_h
#define StringUtils_h

#include "UtilString.h"
#include "kvector.h"
namespace util
{

// string utility tools
class StringUtils
{
public:
	// delete the begin and end blank string from the source
	static StringA             skipBlank(const StringA& src);
	
	// convert string to int
	static LInt                stringToInt(const StringA& src, LInt radix = 10);
	
	// return heap data need to be deleted when it's not needed any more
	static KVector<StringA>*   split(const StringA& src, const StringA& splitSrc);
	
	static void                strTostrW(const StringA& src, String& dest);

	static void                strWtoStr(const String& src, StringA& dest);

	static LUint               hashCode(const StringA& keyName);

	static LBool               isspace(LInt ch);
	static LBool               isdigit(LInt ch);
	static LBool               isalpha(LInt ch);
	static LBool               isalnum(LInt ch);
	static LInt                tolower(LInt ch);
	static LInt                toupper(LInt ch);

	static void                replaceSpecialChars(util::StringA& text);
};

}

using util::StringUtils;
#endif /* STRINGUTILS_H_ */
