LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := ludumdare37

LOCAL_LDLIBS    += -lz
LOCAL_CPP_FEATURES += exceptions
LOCAL_CPP_FEATURES += rtti

FILE_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

FILE_LIST := $(wildcard $(LOCAL_PATH)/States/*.cpp)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

FILE_LIST := $(wildcard $(LOCAL_PATH)/Sources/Components/*.cpp)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)
FILE_LIST := $(wildcard $(LOCAL_PATH)/Sources/Core/*.cpp)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)
FILE_LIST := $(wildcard $(LOCAL_PATH)/Sources/ExtLibs/*.cpp)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)
FILE_LIST := $(wildcard $(LOCAL_PATH)/Sources/ExtLibs/Box2D/*.cpp)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)
FILE_LIST := $(wildcard $(LOCAL_PATH)/Sources/Maths/*.cpp)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)
FILE_LIST := $(wildcard $(LOCAL_PATH)/Sources/System/*.cpp)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_SHARED_LIBRARIES := sfml-system
LOCAL_SHARED_LIBRARIES += sfml-window
LOCAL_SHARED_LIBRARIES += sfml-graphics
LOCAL_SHARED_LIBRARIES += sfml-audio
LOCAL_SHARED_LIBRARIES += sfml-network
LOCAL_SHARED_LIBRARIES += tgui
LOCAL_WHOLE_STATIC_LIBRARIES := sfml-main

include $(BUILD_SHARED_LIBRARY)

$(call import-module,tgui) 
