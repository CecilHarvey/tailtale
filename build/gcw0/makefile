TARGET = tailtale.elf

HOST = mipsel-linux-
AS = $(HOST)as
CC = $(HOST)gcc
CXX = $(HOST)g++
OBJCOPY = $(HOST)objcopy
STRIP = $(HOST)strip

INC = -I. -I../../include
SRC = . ../../source

LIBDIR = LI/opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/lib
LIBINC = -I/opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/include \
	-I/opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/include/SDL
LIBS = $(LIBDIR) -lSDL_image -ljpeg -lSDL_mixer -lvorbis \
	-lSDL_ttf -lfreetype -lSDL -lz -lstdc++

CFLAGS = -g -W -Wall -Os $(INC) $(LIBINC) -DSDL -DGCW0
LDFLAGS = $(LIBS)

CFILES = $(foreach dir, $(SRC), $(wildcard $(dir)/*.c))
OFILES = $(CFILES:.c=.o)

$(TARGET): $(OFILES)
	$(CC) $(OFILES) -o ./$@ $(LDFLAGS)
	$(STRIP) $(TARGET)
	mksquashfs ./$@ tailtale.png default.gcw0.desktop ../../data_high/* \
		TailTale.opk -all-root -noappend -no-exports -no-xattrs

%.o: %.c
	$(CC) $(CFLAGS) -c ./$< -o ./$@

clean:
	rm -f $(TARGET) TailTale.opk $(OFILES)
