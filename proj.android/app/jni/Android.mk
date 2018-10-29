LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)



LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

#define all-cpp-files-under
#$(patsubst ./%,%, \
#  $(shell cd $(LOCAL_PATH) ; \
#          find $(1) -name "*.cpp" -and -not -name ".*") \
# )
#endef

#define all-subdir-cpp-files
#$(call all-cpp-files-under,.)
#endef

#LOCAL_SRC_FILES += $(call all-subdir-cpp-files)

LOCAL_SRC_FILES := $(LOCAL_PATH)/hellocpp/main.cpp \
                   $(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \
                   $(LOCAL_PATH)/../../../Classes/ErrorLog.cpp \
                   $(LOCAL_PATH)/../../../Classes/controller/ConfigLoader.cpp \
                   $(LOCAL_PATH)/../../../Classes/controller/RessourcenManager.cpp \
                   $(LOCAL_PATH)/../../../Classes/lib/DRHashList.cpp \
                   $(LOCAL_PATH)/../../../Classes/lib/TemplateMemoryManager.cpp \
                   $(LOCAL_PATH)/../../../Classes/model/SpriteAnimationState.cpp \
                   $(LOCAL_PATH)/../../../Classes/model/LevelData.cpp \
                   $(LOCAL_PATH)/../../../Classes/nodes/Grid.cpp \
                   $(LOCAL_PATH)/../../../Classes/nodes/CustomAnimationSprite.cpp \
                   $(LOCAL_PATH)/../../../Classes/nodes/PlantNode.cpp \
                   $(LOCAL_PATH)/../../../Classes/model/ViewData.cpp \
                   $(LOCAL_PATH)/../../../Classes/model/PlantType.cpp \
                   $(LOCAL_PATH)/../../../Classes/PlantTypesManager.cpp \
                   $(LOCAL_PATH)/../../../Classes/MainGameScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/levelStates/DisplayInfo.cpp \
                   $(LOCAL_PATH)/../../../Classes/levelStates/DragSeed.cpp \
                   $(LOCAL_PATH)/../../../Classes/levelStates/DropSeedInvalid.cpp \
                   $(LOCAL_PATH)/../../../Classes/levelStates/DropSeedValid.cpp \
                   $(LOCAL_PATH)/../../../Classes/levelStates/RandomSeed.cpp \
                   $(LOCAL_PATH)/../../../Classes/levelStates/PlantSeed.cpp \
                   $(LOCAL_PATH)/../../../Classes/levelStates/PlayerChooseSeed.cpp \
                   $(LOCAL_PATH)/../../../Classes/levelStates/PlayerChooseActionWithSeed.cpp \
                   $(LOCAL_PATH)/../../../Classes/model/Points.cpp


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(LOCAL_PATH)/../../../cocos2d)
$(call import-module, cocos)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
