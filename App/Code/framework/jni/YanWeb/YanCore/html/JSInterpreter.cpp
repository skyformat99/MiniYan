/*
 * JSInterpreter.cpp
 *
 *  Created on: 2011-7-12
 *      Author: yanbo
 */

#include "JSInterpreter.h"

namespace yanbo
{

JSInterpreter::JSInterpreter()
: m_rt(NULL)
, m_cx(NULL)
, m_glob(NULL)
{	
}

JSInterpreter::~JSInterpreter()
{	
	  JS_DestroyContext(m_cx);
	  JS_DestroyRuntime(m_rt);
}

void JSInterpreter::construct(const util::StringA& script)
{
	m_rt = JS_NewRuntime(8L * 1024L * 1024L);
	
	if(m_rt)
	{
	    // create new context
	    m_cx = JS_NewContext(m_rt, 8192);
	    
	    if(m_cx)
	    {
		    JS_SetOptions(m_cx, JSOPTION_VAROBJFIX);
		    JS_SetVersion(m_cx, JSVERSION_1_6);
		    
			JSClass globalClass =
			{
			    "Global", 0,
			    JS_PropertyStub,  JS_PropertyStub,
			    JS_PropertyStub, JS_PropertyStub,
			    JS_EnumerateStub, JS_ResolveStub,
			    JS_ConvertStub,  JS_FinalizeStub
			};
			
			// create global object
			m_glob = JS_NewObject(m_cx, &globalClass, 0, 0);
		    
			// initial standard classes
			JS_InitStandardClasses(m_cx, m_glob);
		    
			jsval rval;
			uintN lineno = 0;
			
			JSBool ok = JS_EvaluateScript(m_cx, m_glob, (const char*)script.GetBuffer(), script.GetLength(), "script", lineno, &rval);
	    }
	}
	
}

JSInterpreter* JSInterpreter::New(const util::StringA& script)
{
	JSInterpreter* interpreter = new JSInterpreter();
	if(NULL != interpreter)
	{
	    interpreter->construct(script);
	}
	
	return interpreter;
}

util::StringA JSInterpreter::evaluateJavaScript(const util::StringA& script)
{	
	util::StringA ret = _CS("");
	jsval rval;
	uintN lineno = 0;
	
	JSBool ok = JS_EvaluateScript(m_cx, m_glob, (const char*)script.GetBuffer(), script.GetLength(), "script", lineno, &rval);
    if(ok == JS_TRUE)
    {
	    JSString *result = JS_ValueToString(m_cx, rval);
	    ret = (const util::LUint8*)JS_GetStringBytes(result);
    }
    
    return ret;
}

}
