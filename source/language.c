/* TailTale - Language */

#include "default.h"

char language[256][256];

char *setlang [] = {
  "english",
  "german",
  "chinese",
  "japanese",
  "polish",
  "french",
  "spanish",
  "portuguese",
  "korean"
};

int Language () {
  char line[256], name[256];
  char buffer_lang[64];
  FILE *file;
  int l, n;

  sprintf (buffer_lang, "language/%s.txt", setlang[sys_language]);

#ifdef LIBPAK
  unsigned int size;

  byte *buffer = extractMem(buffer_lang, openPak("data.pak"), &size);
  if (buffer == NULL)
    return 0;

  file = fmemopen (buffer, size, "r");
#else
  file = fopen (va ("%s/%s", BASEDIR, buffer_lang), "r");
#endif
  if (file == NULL) {
    printf ("Language failed: Cannot open %s.txt\n", setlang[sys_language]);
    return 0;
  } else {
	/* if (setlang[sys_language] == "german")
		rendertype = RENDER_LATIN1;
	else */
		rendertype = RENDER_UTF8;

    while (feof (file) == FALSE) {
      fgets (line, 255, file);

      if (strlen (line) < 6)
	    continue;

      if (line[0] == ' ')
        continue;

      if (line[0] == '#')
        continue;

      if (line[0] == 'L') {
        n = (int)((line[1] - '0') * 100 + (line[2] - '0') * 10 + (line[3] - '0'));
        strcpy (name, (line + 5));
        l = strlen (name);

        if ((name[l - 1] == '\n') || (name[l - 1] == '\r'))
          name[l - 1] = 0;

        if ((name[l - 2] == '\n') || (name[l - 2] == '\r'))
          name[l - 2] = 0;

        strcpy (language[n], name);
      }
	}

    fclose (file);
  }

  return 1;
}
