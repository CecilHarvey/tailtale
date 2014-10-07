#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char byte;

typedef struct pakheader_t {
	char head[4];
	unsigned int dir_offset;
	unsigned int dir_length;
} pakheader;

typedef struct dirsection_t {
	char filename[56];
	unsigned int file_position;
	unsigned int file_length;
} dirsection;

typedef struct pakfile_t {
	FILE *handle;
	pakheader header;
} pakfile;

void createPak(char* filename);
int insertPak(char* file, char* inpak, pakfile *fp);

int extractPak(char *file, char *output, pakfile *fp);
byte *extractMem(char *file, pakfile* fp, unsigned int *size);

pakfile *openPak(char* filename);
int checkPak(pakfile *fp);
int closePak(pakfile *fp);

byte *readFile(char *filename, unsigned int *size);

dirsection *inPak(char *file, pakfile *fp);
dirsection *loadDirSection(pakfile *fp, int *x);

int getLongestFile(dirsection *files, int n);
