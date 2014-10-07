TARGET = tailtale

MOVIE = YES
PAK = YES

INC = -I. -I../../include
SRC = . ../../source

LIBDIR = -L/usr/lib
LIBINC = -I/usr/include -I/usr/include/SDL
LIBS = $(LIBDIR) -lSDL_image -ljpeg -lpng -lSDL_mixer -lvorbisidec \
	-lSDL_ttf -lfreetype -lSDL -lz -lstdc++

CFLAGS = -g -W -Wall -Os $(INC) $(LIBINC) -DSDL -DX86
LDFLAGS = $(LIBS)

ifeq ($(MOVIE), YES)
INC += -I../../shared/include/ogg -I../../shared/include/theora -I../../shared/source/theora
SRC += ../../shared/source/sdl_mixer/ogg ../../shared/source/theora
CFLAGS += -DMOVIE
endif

ifeq ($(PAK), YES)
INC += -I../../shared/source/pak
SRC += ../../shared/source/pak
CFLAGS += -DLIBPAK
endif

CFILES = $(foreach dir, $(SRC), $(wildcard $(dir)/*.c))
OFILES = $(CFILES:.c=.o)

$(TARGET): $(OFILES)
	mkdir -p ../../release
	gcc $(OFILES) -o ./$@ $(LDFLAGS)
	strip $(TARGET)

%.o: %.c
	gcc $(CFLAGS) -c ./$< -o ./$@

clean:
	rm -f $(TARGET) $(OFILES)
