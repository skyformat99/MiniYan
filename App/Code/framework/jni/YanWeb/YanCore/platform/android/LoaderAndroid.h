/*
 * LoaderAndroid.h
 *
 *  Created on: 2014-2-23
 *      Author: yanbo
 */

#ifndef LoaderAndroid_h
#define LoaderAndroid_h

#include "UtilString.h"
#include "NetWorkBase.h"
#include <jni.h>

namespace yanbo
{

struct JLoader;
class LoaderAndroid : public NetWorkBase
{
public:
	LoaderAndroid();
	virtual ~LoaderAndroid();
	
	void initLoader(JNIEnv* env, jobject o);

public:
	virtual void loadUrl(const String& url);
	virtual void postData(const ParamMap& params);
	virtual void cancel();

private:
	struct JLoader* m_privateLoader;
};

}

#endif /* INPUTSTREAM_H_ */
