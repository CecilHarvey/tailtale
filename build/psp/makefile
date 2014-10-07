TARGET = tailtale.elf

HOST = psp-
AS = $(HOST)as
CC = $(HOST)gcc
CXX = $(HOST)g++
OBJCOPY = $(HOST)objcopy
STRIP = $(HOST)strip

INC = -I. -I../../include
SRC = . ../../source

LIBDIR = -L/psp/psp/lib -L/psp/psp/sdk/lib
LIBINC = -I/psp/psp/include -I/psp/psp/include/SDL -I/psp/psp/sdk/include
LIBS = $(LIBDIR) -lSDL_image -ljpeg -lpng -lSDL_mixer -lvorbisidec \
	-lmikmod -lmad -lSDL_ttf -lfreetype -lSDLmain -lSDL -lGL -lz -lm -lc -lstdc++ \
	-lpthread-psp -lpthreadlite -lpspvfpu -lpspgu -lpsphprm -lpsprtc -lpspaudiolib \
	-lpspaudio -lpsppower -lpspdebug -lpspdisplay -lpspge -lpspctrl -lpspsdk \
	-lpspnet -lpspnet_inet -lpspnet_apctl -lpspnet_resolver \
	-lpsputility -lpspuser -lpspkernel

CFLAGS = -g -W -Wall -G0 $(INC) $(LIBINC) -DSDL -DNETWORK -Dmain=SDL_main -DPSP
LDFLAGS = -D_PSP_FW_VERSION=150 $(LIBS)

CFILES = $(foreach dir, $(SRC), $(wildcard $(dir)/*.c))
OFILES = $(CFILES:.c=.o)

$(TARGET): $(OFILES)
	$(CC) $(OFILES) -o ./$@ $(LDFLAGS)
	psp-fixup-imports ./$@
	mksfo 'TailTale - Shippo no Puzzle' ./PARAM.SFO
	$(STRIP) ./$@
	pack-pbp EBOOT.PBP ./PARAM.SFO ./icon0.png NULL ./pic0.png ./pic1.png ./snd0.at3 ./$@ NULL

%.o: %.c
	$(CC) $(CFLAGS) -c ./$< -o ./$@

clean:
	rm -f EBOOT.PBP ./$(TARGET) ./PARAM.SFO $(OFILES)
