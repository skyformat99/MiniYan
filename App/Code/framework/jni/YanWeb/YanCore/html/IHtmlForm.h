/*
 * IHtmlForm.h
 *
 *  Created on: 2011-7-8
 *      Author: Administrator
 */

#ifndef IHtmlForm_h
#define IHtmlForm_h

#include "UtilString.h"

namespace yanbo
{

/**
 * Objects of this class can be sent to the server as
 * a form that has been filled out in the browser. One subclass
 * implements a true Html Form while another is just some stored data
 * that is sent to the server pretending to be a filled in form.
 */
class IHtmlForm
{
public:
	/** Return url to post to. */
	virtual StringA getAction() = 0;
	
	/** Return GET or POST. */
	virtual StringA getMethod() = 0;
	
	/** Return URL encoded parameter string. */
	virtual StringA encodeFormItems() = 0;
	
	/** Return encoding for From. */
	virtual StringA getEncoding() = 0;
	
	virtual StringA getItemValue(const StringA& itemName) = 0;
	virtual StringA getItemMimeType(const StringA& itemName) = 0;
	virtual void setItemValue(const StringA& name, const StringA& value) = 0;
	virtual LBool hasPassword() = 0;
	
	/** Write the form data to the output stream. */
	virtual void postForm(StringA& outstream) = 0;
};

}
#endif /* IHTMLFORM_H_ */
