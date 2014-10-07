LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := SDL2_mixer

OGG_LIBRARY_PATH := external/libogg-1.3.1
SUPPORT_TREMOR := false
TREMOR_LIBRARY_PATH := external/libvorbisidec-1.2.1
SUPPORT_VORBIS := true
VORBIS_LIBRARY_PATH := external/libvorbis-1.3.3
SUPPORT_MAD := true
MAD_LIBRARY_PATH := external/libmad-0.15.1b

LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_CFLAGS := -DWAV_MUSIC

LOCAL_SRC_FILES := $(notdir $(wildcard $(LOCAL_PATH)/*.c)) \
	$(OGG_LIBRARY_PATH)/src/framing.c \
	$(OGG_LIBRARY_PATH)/src/bitwise.c

LOCAL_LDLIBS :=
LOCAL_STATIC_LIBRARIES :=
LOCAL_SHARED_LIBRARIES := SDL2

ifeq ($(SUPPORT_TREMOR),true)
    LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(OGG_LIBRARY_PATH)/include $(LOCAL_PATH)/$(TREMOR_LIBRARY_PATH)
    LOCAL_CFLAGS += -DOGG_MUSIC -DOGG_USE_TREMOR -DFPM_64BIT -DOPT_SPEED -DSTDC_HEADERS -DHAVE_UNISTD_H \
	-DSIZEOF_INT=4 -DSIZEOF_LONG=4 -DSIZEOF_LONG_LONG=8
    LOCAL_SRC_FILES += \
        $(TREMOR_LIBRARY_PATH)/mdct.c \
        $(TREMOR_LIBRARY_PATH)/block.c \
        $(TREMOR_LIBRARY_PATH)/window.c \
        $(TREMOR_LIBRARY_PATH)/synthesis.c \
        $(TREMOR_LIBRARY_PATH)/info.c \
        $(TREMOR_LIBRARY_PATH)/floor1.c \
        $(TREMOR_LIBRARY_PATH)/floor0.c \
        $(TREMOR_LIBRARY_PATH)/vorbisfile.c \
        $(TREMOR_LIBRARY_PATH)/res012.c \
        $(TREMOR_LIBRARY_PATH)/mapping0.c \
        $(TREMOR_LIBRARY_PATH)/registry.c \
        $(TREMOR_LIBRARY_PATH)/codebook.c \
        $(TREMOR_LIBRARY_PATH)/sharedbook.c
endif

ifeq ($(SUPPORT_VORBIS),true)
	LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(OGG_LIBRARY_PATH)/include $(LOCAL_PATH)/$(VORBIS_LIBRARY_PATH)/include \
		$(LOCAL_PATH)/$(VORBIS_LIBRARY_PATH)/lib
	LOCAL_CFLAGS += -DOGG_MUSIC -DFPM_64BIT -DOPT_SPEED -DSTDC_HEADERS -DHAVE_UNISTD_H \
		-DSIZEOF_INT=4 -DSIZEOF_LONG=4 -DSIZEOF_LONG_LONG=8 -DHAVE_CONFIG_H
	LOCAL_SRC_FILES += \
		$(VORBIS_LIBRARY_PATH)/lib/analysis.c \
		$(VORBIS_LIBRARY_PATH)/lib/barkmel.c \
		$(VORBIS_LIBRARY_PATH)/lib/bitrate.c \
		$(VORBIS_LIBRARY_PATH)/lib/block.c \
		$(VORBIS_LIBRARY_PATH)/lib/codebook.c \
		$(VORBIS_LIBRARY_PATH)/lib/envelope.c \
		$(VORBIS_LIBRARY_PATH)/lib/floor0.c \
		$(VORBIS_LIBRARY_PATH)/lib/floor1.c \
		$(VORBIS_LIBRARY_PATH)/lib/info.c \
		$(VORBIS_LIBRARY_PATH)/lib/lookup.c \
		$(VORBIS_LIBRARY_PATH)/lib/lpc.c \
		$(VORBIS_LIBRARY_PATH)/lib/lsp.c \
		$(VORBIS_LIBRARY_PATH)/lib/mapping0.c \
		$(VORBIS_LIBRARY_PATH)/lib/mdct.c \
		$(VORBIS_LIBRARY_PATH)/lib/psy.c \
		$(VORBIS_LIBRARY_PATH)/lib/registry.c \
		$(VORBIS_LIBRARY_PATH)/lib/res0.c \
		$(VORBIS_LIBRARY_PATH)/lib/sharedbook.c \
		$(VORBIS_LIBRARY_PATH)/lib/smallft.c \
		$(VORBIS_LIBRARY_PATH)/lib/synthesis.c \
		$(VORBIS_LIBRARY_PATH)/lib/vorbisenc.c \
		$(VORBIS_LIBRARY_PATH)/lib/vorbisfile.c \
		$(VORBIS_LIBRARY_PATH)/lib/window.c
endif

ifeq ($(SUPPORT_MAD),true)
    LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(MAD_LIBRARY_PATH)
    LOCAL_CFLAGS += -DMP3_MAD_MUSIC
    LOCAL_SRC_FILES += \
		$(MAD_LIBRARY_PATH)/bit.c \
		$(MAD_LIBRARY_PATH)/decoder.c \
		$(MAD_LIBRARY_PATH)/fixed.c \
		$(MAD_LIBRARY_PATH)/frame.c \
		$(MAD_LIBRARY_PATH)/huffman.c \
		$(MAD_LIBRARY_PATH)/layer12.c \
		$(MAD_LIBRARY_PATH)/layer3.c \
		$(MAD_LIBRARY_PATH)/stream.c \
		$(MAD_LIBRARY_PATH)/synth.c \
		$(MAD_LIBRARY_PATH)/timer.c \
		$(MAD_LIBRARY_PATH)/version.c
endif

LOCAL_EXPORT_C_INCLUDES += $(LOCAL_C_INCLUDES)

include $(BUILD_SHARED_LIBRARY)
