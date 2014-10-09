/*
	SDL puzzle project
*/

#include "default.h"

int sys_joystick_swap, sys_debug, sys_language, sys_level, sys_fullscreen, sys_frameskip, \
	sys_fps, sys_screensize, sys_sound, sys_bgm_volume, sys_se_volume;
#ifdef NETWORK
char net_name[64];
int net_score, net_block, net_evomode;
#endif

#if defined(MACOSX)

const char *GetINIDir() {
	static char buf[256] = "";
	if (buf[0] == '\0') {
		sprintf(buf, "%s/Library/Preferences/", getenv("HOME"));
	}
	return buf;
}

#elif defined(__IOS__)

const char *GetINIDir() {
	static char buf[256] = "";
	if (buf[0] == '\0') {
        char *val = SDL_GetPrefPath("Library", "");
        strcpy(buf, val);
        SDL_free(val);
	}
	return buf;
}

#else

const char *GetINIDir() {
	return BASEDIR;
}

#endif

void Config_Load () {
#if defined(_DINGOO) || defined(GEKKODISC) // NATIV Dingoo and WII Disc
	sys_level = 1;
	sys_debug = 0;
	sys_language = 0;
	sys_fullscreen = 0;
	sys_screensize = 0;
	sys_frameskip = 0;
	sys_fps = 0;
	sys_joystick_swap = 0;
	sys_sound = 1;
	sys_bgm_volume = 85;
	sys_se_volume = 60;
#else
	FILE* config;

	config = fopen (va ("%s/tailtale.ini", GetINIDir()), "r");
	if (config == NULL) { // Setup Default config!
#ifdef LIBPAK
      extractPak("tailtale.ini", "tailtale.ini", openPak("data.pak"));
#else
	  config = fopen (va ("%s/tailtale.ini", GetINIDir()), "w");
		fprintf (config, "sys_level=1\n");
		fprintf (config, "sys_debug=1\n");
		fprintf (config, "sys_language=0\n");
		fprintf (config, "sys_fullscreen=0\n");
		fprintf (config, "sys_screensize=0\n");
		fprintf (config, "sys_frameskip=0\n");
		fprintf (config, "sys_fps=0\n");
		fprintf (config, "sys_joystick_swap=0\n");
		fprintf (config, "sys_sound=1\n");
		fprintf (config, "sys_bgm_volume=85\n");
		fprintf (config, "sys_se_volume=60\n");
#ifdef NETWORK
		fprintf (config, "net_name=Akane\n");
		fprintf (config, "net_score=0\n");
		fprintf (config, "net_block=0\n");
		fprintf (config, "net_evomode=0\n");
#endif
	  fclose (config);
#endif

      Config_Load ();
	} else {
		fscanf (config, "sys_level=%d\n", &sys_level);
		fscanf (config, "sys_debug=%d\n", &sys_debug);
		fscanf (config, "sys_language=%d\n", &sys_language);
		fscanf (config, "sys_fullscreen=%d\n", &sys_fullscreen);
		fscanf (config, "sys_screensize=%d\n", &sys_screensize);
		fscanf (config, "sys_frameskip=%d\n", &sys_frameskip);
		fscanf (config, "sys_fps=%d\n", &sys_fps);
		fscanf (config, "sys_joystick_swap=%d\n", &sys_joystick_swap);
		fscanf (config, "sys_sound=%d\n", &sys_sound);
		fscanf (config, "sys_bgm_volume=%d\n", &sys_bgm_volume);
		fscanf (config, "sys_se_volume=%d\n", &sys_se_volume);
#ifdef NETWORK
		fscanf (config, "net_name=%s\n", net_name);
		fscanf (config, "net_score=%d\n", &net_score);
		fscanf (config, "net_block=%d\n", &net_block);
		fscanf (config, "net_evomode=%d\n", &net_evomode);
#endif

		fclose (config);
	}
#endif
}

void Config_Save () {
#if defined(_DINGOO) || defined(GEKKODISC) // NATIV Dingoo and WII Disc
#else
	FILE *config;

	config = fopen (va ("%s/tailtale.ini", GetINIDir()), "w");
	if (config == NULL)
		printf ("Config_Save failed: Cannot open tailtale.ini\n");
	else {
		fprintf (config, "sys_level=%d\n", sys_level);
		fprintf (config, "sys_debug=%d\n", sys_debug);
		fprintf (config, "sys_language=%d\n", sys_language);
		fprintf (config, "sys_fullscreen=%d\n", sys_fullscreen);
		fprintf (config, "sys_screensize=%d\n", sys_screensize);
		fprintf (config, "sys_frameskip=%d\n", sys_frameskip);
		fprintf (config, "sys_fps=%d\n", sys_fps);
		fprintf (config, "sys_joystick_swap=%d\n", sys_joystick_swap);
		fprintf (config, "sys_sound=%d\n", sys_sound);
		fprintf (config, "sys_bgm_volume=%d\n", sys_bgm_volume);
		fprintf (config, "sys_se_volume=%d\n", sys_se_volume);
#ifdef NETWORK
		fprintf (config, "net_name=%s\n", net_name);
		fprintf (config, "net_score=%d\n", net_score);
		fprintf (config, "net_block=%d\n", net_block);
		fprintf (config, "net_evomode=%d\n", net_evomode);
#endif

		fclose (config);
	}
#endif
}

#ifndef _DINGOO // NATIV Dingoo
void DateTime () {
  int ampm, hour;
  struct tm *tm_ptr;
  time_t current_time;

  current_time = time (0);
  localtime (&current_time);
  tm_ptr = gmtime (&current_time);

  hour = tm_ptr->tm_hour, ampm = 0;

  if (hour == 0)
    hour = 12;
  else if (hour >= 12) {
    ampm = 1;

    if (hour >= 13)
      hour -= 12;
  }

#if defined(ANDROID) // Rikku2000: Need an Fix...
    time_dayandnight = "day";
#else
  // This if for the Game Background Day Time Change...
  if ((ampm == 0 && hour < 5) || (ampm == 1 && hour > 5))
    time_dayandnight = "night"; // 18:00 - 6:00
  else
    time_dayandnight = "day"; // 6:00 - 18:00
#endif

#ifdef TAILTALE_HD
  if (ampm == 0)
    sprintf (time_result, "%02d/%02d %02d:%02d AM", tm_ptr->tm_mday, tm_ptr->tm_mon + 1, hour, tm_ptr->tm_min);
  else
    sprintf (time_result, "%02d/%02d %02d:%02d PM", tm_ptr->tm_mday, tm_ptr->tm_mon + 1, hour, tm_ptr->tm_min);
#endif
}
#endif

// Not supported on Windows...
#if defined(ANDROID) || defined(WINPHONE) || defined(VS_WIN) || defined(VS_WIN8)
FILE *fmemopen (void *buf, size_t size, const char *opentype) {
	FILE *f;
#ifdef WIN32
	char* tmp_fname;
#endif
	strcmp (opentype, "r");
#ifdef WIN32
	tmp_fname = _tempnam ("%TMP%", "fmemopen");
	f = fopen (tmp_fname, "wt");
	fwrite (buf, 1, size, f);
	fclose (f);
	f = fopen (tmp_fname, "rt");
#else
	f = tmpfile ();
	fwrite (buf, 1, size, f);
	rewind (f);
#endif
	return f;
}
#else
#ifdef WIN32
FILE *fmemopen (void *buf, size_t size, const char *opentype) {
	char temppath[260 - 13];
	char filename[260 + 1];
	FILE *f;

	if (0 == GetTempPath (sizeof (temppath), temppath))
		return NULL;

	if (0 == GetTempFileName (temppath, "SC", 0, filename))
		return NULL;

	f = fopen(filename, "wb");
	if (NULL == f)
		return NULL;

	fwrite (buf, size, 1, f);
	fclose (f);

	return fopen (filename, opentype);
}
#endif
#endif

char *va (const char *fmt, ...) {
	char *p, *np;
	int n, size = 256;
	va_list ap;

	if ((p = malloc (size)) == NULL)
		return NULL;

	while (1) {
		va_start (ap, fmt);
		/* n = vsnprintf (p, size, fmt, ap); */
		n = vsprintf (p, fmt, ap);
		va_end (ap);

		if (n > -1 && n < size)
			return p;

		if (n > -1)
			size = n + 1;
		else
			size *= 2;

		if ((np = realloc (p, size)) == NULL) {
			free (p);
			return NULL;
		} else
			p = np;
	}
}

int explode (char (*ap)[128], char *msg, char split) {
	int i = 0, k = 0, l = 0;

	while (msg[i] != '\0') {
		if (msg[i] == split) {
			ap[k][l++] = '\0';
			k++;
			l = 0;
		} else 
			ap[k][l++] = msg[i];

		i++;
	}

	ap[k][l] = '\0';

	return (k);
}
