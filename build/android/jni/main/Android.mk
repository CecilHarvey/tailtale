LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := $(lastword $(subst /, ,$(LOCAL_PATH)))

SUPPORT_THEORA := true
THEORA_LIBRARY_PATH := ../../../../shared/source/theora

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include/SDL2 \
	$(LOCAL_PATH)/../SDL2_image \
	$(LOCAL_PATH)/../SDL2_image/external/jpeg-9 \
	$(LOCAL_PATH)/../SDL2_image/external/libpng-1.6.2 \
	$(LOCAL_PATH)/../SDL2_mixer \
	$(LOCAL_PATH)/../SDL2_mixer/external/libogg-1.3.1/include/ogg \
	$(LOCAL_PATH)/../SDL2_mixer/external/libvorbis-1.3.3/include \
	$(LOCAL_PATH)/../SDL2_mixer/external/libvorbisidec-1.2.1 \
	$(LOCAL_PATH)/../SDL2_ttf \
	$(LOCAL_PATH)/../SDL2_ttf/external/freetype-2.4.12 \
	$(LOCAL_PATH)/../../../../include
LOCAL_CFLAGS := -DSDL2 -DANDROID -DNETWORK -DTOUCH
LOCAL_SRC_FILES := ../SDL2/src/main/android/SDL_android_main.c \
	../../../../source/base.c \
	../../../../source/common.c \
	../../../../source/disp.c \
	../../../../source/font.c \
	../../../../source/input.c \
	../../../../source/language.c \
	../../../../source/main.c \
	../../../../source/menu.c \
	../../../../source/netcode.c \
	../../../../source/network.c \
	../../../../source/port.c \
	../../../../source/screen.c \
	../../../../source/sound.c \
	../../../../source/sprite.c \
	../../../../source/texture.c \
	../../../../source/trial.c

ifeq ($(SUPPORT_THEORA),true)
    LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(THEORA_LIBRARY_PATH) $(LOCAL_PATH)/$(THEORA_LIBRARY_PATH)/../../include/theora
    LOCAL_CFLAGS += -DMOVIE
    LOCAL_SRC_FILES += \
		../../../../source/movie.c \
		../../../../shared/source/theora/analyze.c \
		../../../../shared/source/theora/apiwrapper.c \
		../../../../shared/source/theora/bitpack.c \
		../../../../shared/source/theora/decapiwrapper.c \
		../../../../shared/source/theora/decinfo.c \
		../../../../shared/source/theora/decode.c \
		../../../../shared/source/theora/dequant.c \
		../../../../shared/source/theora/encapiwrapper.c \
		../../../../shared/source/theora/encfrag.c \
		../../../../shared/source/theora/encinfo.c \
		../../../../shared/source/theora/encode.c \
		../../../../shared/source/theora/enquant.c \
		../../../../shared/source/theora/fdct.c \
		../../../../shared/source/theora/fragment.c \
		../../../../shared/source/theora/huffdec.c \
		../../../../shared/source/theora/huffenc.c \
		../../../../shared/source/theora/idct.c \
		../../../../shared/source/theora/info.c \
		../../../../shared/source/theora/internal.c \
		../../../../shared/source/theora/mathops.c \
		../../../../shared/source/theora/mcenc.c \
		../../../../shared/source/theora/quant.c \
		../../../../shared/source/theora/rate.c \
		../../../../shared/source/theora/state.c \
		../../../../shared/source/theora/theoraplay.c \
		../../../../shared/source/theora/tokenize.c
endif

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_mixer SDL2_image SDL2_ttf
LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -ldl -llog -lz -lstdc++

include $(BUILD_SHARED_LIBRARY)
