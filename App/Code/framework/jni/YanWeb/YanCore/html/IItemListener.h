/*
 * IItemListener.h
 *
 *  Created on: 2011-6-30
 *      Author: yanbo
 */

#ifndef IItemListener_h
#define IItemListener_h

#include "String.h"
#include "IHtmlForm.h"
namespace yanbo
{

class IItemListener
{
public:
    /**
     * Called when the user has selected and
     * clikced on a link in the HTML view.
     * Return false if you want the view to open the url.
     * Return true if you will handle the event yourself
     * @return url the URL that was clicked on.
     */
	virtual void onClick(const String& url, const StringA& script) = 0;
	
   /**
	 * Called when the user is submitting a form
	 * Return true if you will handle this event yourself. (for instance store form)
	 * Return false if you would like the default handliing, which is to do a post this form to a web server.
	 * @param HtmlForm - the form that is about to be submitted.
	 */
	virtual void onFormSubmit(const IHtmlForm& form) = 0;
};

}
#endif /* IITEMLISTENER_H_ */
