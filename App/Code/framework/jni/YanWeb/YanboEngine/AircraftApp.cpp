#include <jni.h>
#include <android/log.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ShaderUtil.h"
#include "MatrixState.h"
#include "Shape.h"

#include "BaseShape.h"

#define  LOG_TAG    "libmini"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

enum TouchEventType
{
    ACTION_DOWN = 0,
    ACTION_UP = 1,
    ACTION_MOVE = 2,
    ACTION_CANCEL = 3,
};


GLuint gProgram;
GLuint gvPositionHandle;
yanbo::Shape* ha[6];
float TOUCH_SCALE_FACTOR = 180.0f/360;//角度缩放比例
float mPreviousY = 0.0f;
float mPreviousX = 0.0f;

yanbo::BaseShape* pShape = NULL;

extern void nativeGetAssetManager(JNIEnv* env, jobject obj, jobject assetManager);
extern void nativeUpdatePatch
  (JNIEnv *env, jobject object, jstring oldApkPath, jstring newApkPath, jstring patchFilePath);

bool setupGraphics(int w, int h) {

	yanbo::ShaderUtil::setScreenSize(w, h);
    glViewport(0, 0, w, h);
	//计算GLSurfaceView的宽高比
	float ratio = (float) w / h;
    //设置平行投影
//	mini::MatrixState::setProjectFrustum(-ratio, ratio, -1, 1, 1, 10);
//	mini::MatrixState::setProjectFrustum(-ratio*0.4f, ratio*0.4f, -1*0.4f, 1*0.4f, 1.0f, 50);
	yanbo::MatrixState::setProjectFrustum(-1.0f*ratio, 1.0f*ratio, -1.0f*ratio, 1.0f*ratio, 1.0f, 50);
//	mini::MatrixState::setCamera(
//						0.0f, 0.0f, 1.0f,
//						0.0f, 0.0f, 0.0f,
//						0.0f, 1.0f, 0.0f
//						);
	yanbo::MatrixState::setCamera(0,0,1,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
	yanbo::MatrixState::copyMVMatrix();
    return true;
}

void renderFrame()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    for(int i=0;i<6;i++)
    {
    	ha[i]->drawSelf();
    }

    pShape->drawSelf();
}

static void init(JNIEnv * env, jobject obj,  jint width, jint height)
{
	yanbo::MatrixState::init();

	setupGraphics(width, height);
    //设置屏幕背景色RGBA
    glClearColor(0.5f,0.5f,0.5f, 1.0f);
    //创建六角星数组中的各个对象
    for(int i=0;i<6;i++)
    {
    	ha[i] = new yanbo::Shape(0.4f, 1.0f,-1.0f * i);
    }

    pShape = new yanbo::BaseShape();

    pShape->setRect(util::LRect(0,0,200,200));
    pShape->setColor(util::LRgb(255,0,0));
    //打开深度检测
    glEnable(GL_DEPTH_TEST);
}

static void step(JNIEnv * env, jobject obj)
{
	renderFrame();
}

static void handleAndroidEvent(JNIEnv* env, jobject obj, jint type, jfloat x, jfloat y)
{
    switch (type) {
    case ACTION_MOVE:
        float dy = y - mPreviousY;//计算触控笔Y位移
        float dx = x - mPreviousX;//计算触控笔X位移
        for(int i=0;i<6;i++)
        {
        	ha[i]->m_yAngle += dx * TOUCH_SCALE_FACTOR;//设置六角星数组中的各个六角星绕y轴旋转角度
        	ha[i]->m_xAngle += dy * TOUCH_SCALE_FACTOR;//设置六角星数组中的各个六角星绕x轴旋转角度
        }
    }
    mPreviousY = y;//记录触控笔位置
    mPreviousX = x;//记录触控笔位置
}


static void createShape(JNIEnv* env, jobject obj)
{

}

static JNINativeMethod gGLApiMethods[] = {
		{"init", "(II)V", (void*)init},
		{"step", "()V", (void*)step},
		{"handleAndroidEvent", "(IFF)V", (void*)handleAndroidEvent},
		{"createShape", "()V", (void*)createShape},
		{"nativeGetAssetManager", "(Landroid/content/res/AssetManager;)V", (void*)nativeGetAssetManager},
		{"nativeUpdatePatch", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", (void*)nativeUpdatePatch},
};

const char* GLApiClass = "com/mini/app/core/MiniViewCore";

extern int registerNativeMethods(JNIEnv* env, const char* className,
        JNINativeMethod* gMethods, int numMethods);

/*
* Register native methods for all classes we know about.
*/
int registerGLNatives(JNIEnv* env)
{
	if (!registerNativeMethods(env, GLApiClass, gGLApiMethods,
                                 sizeof(gGLApiMethods) / sizeof(gGLApiMethods[0])))
		return JNI_FALSE;

	return JNI_TRUE;
}
