#include "JNIUtil.h"
#include "AutoObject.h"
//#include <stdarg.h>

namespace yanbo
{
// signature代码参数
void JNIUtil::callStaticMethod(const char* className,
		const char* method,
		const char* signature,
        ...
	)
{
	JNIEnv* env = util::getJNIEnv();
	jclass clazz = env->FindClass(className);
	jmethodID jmethod = env->GetStaticMethodID(clazz, method, signature);
	va_list args;
	va_start(args, signature);
	env->CallStaticVoidMethodV(clazz, jmethod, args);
	va_end(args);
	env->DeleteLocalRef(clazz);
}

}
