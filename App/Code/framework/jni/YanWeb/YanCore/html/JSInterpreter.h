/*
 * JSInterpreter.h
 *
 *  Created on: 2011-7-12
 *      Author: yanbo
 */

#ifndef JSINTERPRETER_H_
#define JSINTERPRETER_H_

#include "jsapi.h"
#include "LString.h"

namespace yanbo
{

// js Interpreter use spidermonkey
class JSInterpreter
{
public:
	static JSInterpreter* New(const util::StringA& script);
	~JSInterpreter();

public:
	// execute a js code or execute a function, previous defined
	util::StringA evaluateJavaScript(const util::StringA& script);
	
private:	
	JSInterpreter();
	void construct(const util::StringA& script);
	
private:
	JSRuntime *m_rt;
	JSContext *m_cx;
	JSObject  *m_glob;
};

}
#endif /* JSINTERPRETER_H_ */
