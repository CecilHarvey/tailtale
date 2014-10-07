TARGET = boot

MOVIE = YES

HOST = powerpc-eabi-
AS = $(HOST)as
CC = $(HOST)gcc
CXX = $(HOST)g++
OBJCOPY = $(HOST)objcopy
STRIP = $(HOST)strip

INC = -I. -I../../include
SRC = . ../../source

LIBDIR = -L../../shared/lib/wii -L/libogc/lib/wii -L/libogc/lib
LIBINC = -I../../shared/include/wii -I/libogc/include/ogc -I/libogc/include/SDL -I/libogc/include
LIBS = $(LIBDIR) -lSDL_image -ljpeg -lpng -lSDL_mixer -lvorbisidec -lSDL_ttf -lfreetype -lSDL \
	-lfat -lwiiuse -lnetport -lbte -lwiikeyboard -logc -lz -lc -lm -lstdc++

CFLAGS = -g -W -Wall -Os $(INC) $(LIBINC) -DSDL -D__WII__ -DGEKKO -DNETWORK
LDFLAGS = -mrvl $(LIBS)

ifeq ($(MOVIE), YES)
INC += -I../android/jni/SDL2_mixer/external/libogg-1.3.1/include -I../../shared/include/theora -I../../shared/source/theora
SRC += ../android/jni/SDL2_mixer/external/libogg-1.3.1/src ../../shared/source/theora
CFLAGS += -DMOVIE
endif

CFILES = $(foreach dir, $(SRC), $(wildcard $(dir)/*.c))
OFILES = $(CFILES:.c=.o)

$(TARGET): $(OFILES)
	$(CC) $(OFILES) -o ./$@ $(LDFLAGS)
	elf2dol $(TARGET) $(TARGET).dol

%.o: %.c
	$(CC) $(CFLAGS) -c ./$< -o ./$@

clean:
	rm -f boot.dol ./$(TARGET) $(OFILES)
