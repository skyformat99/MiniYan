#include "AutoObject.h"

namespace util
{
static JavaVM* sJavaVM = 0;

AutoJObject getRealObject(JNIEnv* env, jobject obj)
{
    jobject real = env->NewLocalRef(obj);
    return AutoJObject(env, real);
}

jmethodID GetJMethod(JNIEnv* env, jclass clazz, const char name[], const char signature[])
{
    jmethodID m = env->GetMethodID(clazz, name, signature);
    return m;
}

// C字符串转java字符串
jstring strToJstring(JNIEnv* env, const char* s)
{
    int        strLen    = strlen(s);
    jclass     jstrObj   = env->FindClass("java/lang/String");
    jmethodID  methodId  = env->GetMethodID(jstrObj, "<init>", "([BLjava/lang/String;)V");
    jbyteArray byteArray = env->NewByteArray(strLen);
    jstring    encode    = env->NewStringUTF("utf-8");

    env->SetByteArrayRegion(byteArray, 0, strLen, (jbyte*)s);

    jstring str = (jstring)(env->NewObject(jstrObj, methodId, byteArray, encode));

    env->DeleteLocalRef(byteArray);
    env->DeleteLocalRef(encode);
    env->DeleteLocalRef(jstrObj);
    return str;
}

char* jstringTostr(JNIEnv* env, jstring jstr)
{
    char* pStr = NULL;

    jclass     jstrObj   = env->FindClass("java/lang/String");
    jstring    encode    = env->NewStringUTF("utf-8");
    jmethodID  methodId  = env->GetMethodID(jstrObj, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray byteArray = (jbyteArray)env->CallObjectMethod(jstr, methodId, encode);
    jsize      strLen    = env->GetArrayLength(byteArray);
    jbyte      *jBuf     = env->GetByteArrayElements(byteArray, JNI_FALSE);

    if (jBuf > 0)
    {
        pStr = (char*)malloc(strLen + 1);
        if (!pStr)
        {
            return NULL;
        }

        memcpy(pStr, jBuf, strLen);
        pStr[strLen] = 0;
    }

    env->ReleaseByteArrayElements(byteArray, jBuf, 0);
    env->DeleteLocalRef(encode);
    env->DeleteLocalRef(jstrObj);
    return pStr;
}

void setJavaVM(JavaVM* javaVM)
{
	sJavaVM = javaVM;
}

JNIEnv* getJNIEnv()
{
    JNIEnv* env;
    jint jniError = sJavaVM->AttachCurrentThread(&env, 0);

    if (jniError == JNI_OK)
        return env;
    return 0;
}

void detachJNIEnv(JNIEnv* env)
{
	if (env != 0)
	{
	    sJavaVM->DetachCurrentThread();
	}
}

AutoJNIEnv::AutoJNIEnv()
    : m_env(NULL)
{
    if (sJavaVM != NULL)
    {
        sJavaVM->AttachCurrentThread(&m_env, 0);
    }
}

AutoJNIEnv::~AutoJNIEnv()
{
    if (sJavaVM != NULL)
    {
        sJavaVM->DetachCurrentThread();
    }
}

JNIEnv* AutoJNIEnv::getEnv()
{
    return m_env;
}

}
