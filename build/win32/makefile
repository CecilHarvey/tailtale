TARGET = tailtale.exe

MOVIE = YES
PAK = NO

INC = -I. -I../../include
SRC = . ../../source

LIBDIR = -L/usr/lib
LIBINC = -I/usr/include -I/usr/include/SDL
LIBS = $(LIBDIR) -lSDL_image -lSDL_mixer -lvorbis -logg -lSDL_ttf \
	-lfreetype -lSDLmain -lSDL -lz -lstdc++

CFLAGS = -g -W -Wall -Os -mwindows $(INC) $(LIBINC) \
	-DSDL -DX86 -DICON -DNETWORK -D__STRICT_ANSI__
LDFLAGS = -mwindows $(LIBS)

ifeq ($(MOVIE), YES)
INC += -I../../shared/include/theora -I../../shared/source/theora
SRC += ../../shared/source/theora
CFLAGS += -DMOVIE
endif

ifeq ($(PAK), YES)
INC += -I../../shared/source/pak
SRC += ../../shared/source/pak
CFLAGS += -DLIBPAK
endif

CFILES = $(foreach dir, $(SRC), $(wildcard $(dir)/*.c))
RCFILES = $(foreach dir, $(SRC), $(wildcard $(dir)/*.rc))

OFILES = $(CFILES:.c=.o) $(RCFILES:.rc=.o)

$(TARGET): $(OFILES)
	gcc.exe $(OFILES) -o ./$@ $(LDFLAGS)
	strip.exe $(TARGET)

%.o: %.c
	gcc.exe $(CFLAGS) -c ./$< -o ./$@

%.o: %.rc
	windres.exe -i ./$< --input-format=rc -o ./$@ -O coff

clean:
	rm.exe -f $(TARGET) $(OFILES)
