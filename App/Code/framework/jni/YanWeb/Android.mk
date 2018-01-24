LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#编译器优化会导致一些问题
LOCAL_PROGUARD_ENABLED := disabled  
LOCAL_MODULE    := libYanWeb
LOCAL_CFLAGS    := -Werror -DHAVE_EXPAT_CONFIG_H
LOCAL_LDLIBS    := -llog -lGLESv2 -lGLESv1_CM -landroid -ljnigraphics
LOCAL_MODULE_CLASS := STATIC_LIBRARIES
LOCAL_CPP_EXTENSION := .cpp
# platform instance by gl
# LOCAL_LDLIBS := -llog -lGLESv1_CM -ldl
# draw api instance

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/YanCore/css \
    $(LOCAL_PATH)/YanCore/gui \
    $(LOCAL_PATH)/YanCore/html \
    $(LOCAL_PATH)/YanCore/html/dom \
    $(LOCAL_PATH)/YanCore/loader \
    $(LOCAL_PATH)/YanCore/parser/tinyxml \
    $(LOCAL_PATH)/YanCore/parser/tinyxml2 \
    $(LOCAL_PATH)/YanCore/parser/expat \
    $(LOCAL_PATH)/YanCore/util \
    $(LOCAL_PATH)/YanCore/view \
    $(LOCAL_PATH)/YanCore/platform \
    $(LOCAL_PATH)/YanCore/platform/android \
    $(LOCAL_PATH)/YanCore/platform/graphics/gl \
    $(LOCAL_PATH)/YanCore/engine \
    $(LOCAL_PATH)/YanCore/utils \
    
LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/Bsdiff/include \

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/Lua/include \
    
LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/YanNet \
    $(LOCAL_PATH)/YanNet/threadpool \
    
LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/YanboEngine/core \
    $(LOCAL_PATH)/YanboEngine/shaders \
    $(LOCAL_PATH)/YanboEngine/shape \

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/Json

include $(LOCAL_PATH)/YanCore/Android.YanCore.util.mk

include $(LOCAL_PATH)/YanCore/Android.YanCore.mk

include $(LOCAL_PATH)/YanboEngine/Android.YanboEngine.mk

include $(LOCAL_PATH)/YanNet/Android.YanNet.mk

include $(LOCAL_PATH)/Bsdiff/Android.Bsdiff.mk
    
LOCAL_SRC_FILES += \
    $(LOCAL_PATH)/Porting/MiniJniOnLoad.cpp \
    $(LOCAL_PATH)/Porting/MiniViewCore.cpp

#LOCAL_STATIC_LIBRARIES := liblua
#EXTERNAL_BASE := $(LOCAL_PATH)/../
    
#$(call import-add-path, $(EXTERNAL_BASE))
#$(call import-module, Bsdiff)

include $(BUILD_SHARED_LIBRARY)
    