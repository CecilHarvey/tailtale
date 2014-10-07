#include "pak.h"

#ifdef LIBPAK
void createPak (char* filename) {
	pakfile mypak;

	mypak.header.head[0] = 'P';
	mypak.header.head[1] = 'A';
	mypak.header.head[2] = 'C';
	mypak.header.head[3] = 'K';

	mypak.header.dir_offset = sizeof(mypak.header.head) + 1;
	mypak.header.dir_length = 1;

	mypak.handle = fopen (filename, "wb");
		fwrite (&mypak.header, sizeof (mypak.header), 1, mypak.handle);
	fclose (mypak.handle);
}

int checkPak (pakfile *fp) {
	if (fp->header.head[0] != 'P' || fp->header.head[1] != 'A' || fp->header.head[2] != 'C' || fp->header.head[3] != 'K')
		return -1;

	if (fp->header.dir_offset < (sizeof(fp->header.head) + 1) || fp->header.dir_length < 1)
		return -2;

	return 0;
}

pakfile *openPak (char* filename) {
	pakfile *mypak;
	FILE *fp;

	fp = fopen (filename, "r+");

	mypak = malloc(sizeof(pakfile));
	mypak->handle = fp;

	if (mypak->handle == NULL)
		return NULL;

	fread (&mypak->header, sizeof(pakheader), 1, mypak->handle);

	return mypak;
}

int insertPak (char* file, char* inpak, pakfile *fp) {
	unsigned int count;
	dirsection sect;
	dirsection *files;
	int x =0;
	byte* buffer;
	int i;

	if (sizeof(inpak) > 56)
		return -1;

	buffer = readFile (file, &count);

	if(fp->header.dir_length == 1) {
		fseek(fp->handle, 4, SEEK_SET);
		fp->header.dir_offset = sizeof(fp->header) + sizeof(byte)*count;
		fp->header.dir_length = sizeof(dirsection);

		fwrite(&fp->header.dir_offset, sizeof(fp->header.dir_offset), 1, fp->handle);
		fwrite(&fp->header.dir_length, sizeof(fp->header.dir_length), 1, fp->handle);

		fwrite(buffer, sizeof(byte), count, fp->handle);

		strcpy(sect.filename, inpak);
		sect.file_position = 12;
		sect.file_length = sizeof(byte) * count;

		fwrite(&sect, sizeof(sect), 1, fp->handle);
	} else {
		if (fp->header.dir_length % 64 != 0)
			return -1;

		x = fp->header.dir_length / 64;

		files = malloc ((x+1) * sizeof(dirsection));
		fseek (fp->handle, fp->header.dir_offset, SEEK_SET);
		fread(files, sizeof(dirsection), x, fp->handle);

		for (i = 0; i < x; i++) {
			char *name = files[i].filename;
			if (strcmp(name, inpak) == 0)
				return -1;
		}

		fseek(fp->handle, 4, SEEK_SET);
		fp->header.dir_offset = fp->header.dir_offset + count;
		fp->header.dir_length = fp->header.dir_length + sizeof(dirsection);

		fwrite (&fp->header.dir_offset, sizeof(fp->header.dir_offset), 1, fp->handle);
		fwrite (&fp->header.dir_length, sizeof(fp->header.dir_length), 1, fp->handle);

		fseek(fp->handle, (fp->header.dir_offset - count), SEEK_SET);

		fwrite(buffer, sizeof(byte), count, fp->handle);

		strcpy(files[x].filename, inpak);

		files[x].file_position = fp->header.dir_offset - count;
		files[x].file_length = sizeof(byte) * count;

		fwrite(files, sizeof(dirsection), x+1, fp->handle);
	}                     

	free(buffer);

	return 0;
}

byte *readFile (char *filename, unsigned int *size) {
	FILE *fil;
	byte c;
	int b;
	unsigned int count =0;
	byte *buffer;

	fil = fopen (filename, "r");
	if (fil == NULL)
		return NULL;

	while (fread (&c, sizeof(c), 1, fil) > 0)
		count++;

	buffer = malloc (count * sizeof(byte));

	rewind (fil);

	count = 0;
	while ((b = fgetc (fil)) != EOF) {
		buffer[count] = b;
		count++;
	}

	fclose(fil);
	*size = count;

	return buffer;
}

int closePak (pakfile *fp) {
	fclose (fp->handle);
	free (fp);

	return 0;
}

dirsection *inPak (char *file, pakfile *fp) {
	dirsection *sect = malloc(sizeof(dirsection));

	int x = 0;

	dirsection *files = loadDirSection(fp, &x);

	int i;
	for (i = 0; i < x; i++) {
		char *name = files[i].filename;
		unsigned int position = files[i].file_position;
		unsigned int length = files[i].file_length;

		if (strcmp(name, file) == 0) {
			strcpy(sect->filename, name);
			sect->file_position = position;
			sect->file_length = length;

			free(files);return sect;
		}
	}

	free(files);

	return NULL;
}

int extractPak (char *file, char *output, pakfile *fp) {
	FILE *fil;
	dirsection *mysect;
	byte *buffer;

	if(fp == NULL)
		return -1;

	mysect = inPak(file, fp);
	if (mysect == NULL)
		return -1;

	fil = fopen (output, "w");

	fseek(fp->handle, mysect->file_position, SEEK_SET);
	buffer = malloc(mysect->file_length * sizeof(byte));

	fread(buffer, sizeof(byte), mysect->file_length, fp->handle);

	fwrite(buffer, sizeof(byte), mysect->file_length, fil);

	fclose (fil);
	free (buffer);

	return 0;
}

byte *extractMem (char *file, pakfile* fp, unsigned int *size) {
	byte *buffer;
	dirsection *mysect;

	if(fp == NULL)
		return NULL;

	mysect = inPak(file, fp);
	if (mysect == NULL)
		return NULL;

	fseek(fp->handle, mysect->file_position, SEEK_SET);
	buffer = malloc(mysect->file_length * sizeof(byte));

	*size = mysect->file_length;

	fread (buffer, sizeof(byte), mysect->file_length, fp->handle);

	return buffer;
}

dirsection *loadDirSection (pakfile *fp, int *x) {
	dirsection *files;

	*x = fp->header.dir_length / 16;

	files = malloc (*x * sizeof(dirsection));
	fseek(fp->handle, fp->header.dir_offset, SEEK_SET);
	fread(files, sizeof(dirsection), *x, fp->handle);

	return files;
}

int getLongestFile (dirsection *files, int n) {
	int longest = 0;
	int curr;

	while (n >= 0 ) {
		curr = strlen (files[n].filename);
		if (curr > longest)
			longest = curr;

		n--;
	}

	return longest;
}	

/* SDL_Surface *loadBMP (char* file, pakfile *fp) {
	unsigned int size;
	SDL_RWops *rw;
	SDL_Surface *plane;

	byte *buffer = extractMem(file, fp, &size);
	if (buffer == NULL)
		return NULL;

	rw = SDL_RWFromMem(buffer, size);
	plane = SDL_LoadBMP_RW (rw, 0);

	free (buffer);
	SDL_FreeRW (rw);

	return plane;
} */
#endif
