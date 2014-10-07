/*
	SDL puzzle project
*/

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

extern int sys_joystick_swap, sys_debug, sys_language, sys_level, sys_fullscreen, sys_frameskip,
		   sys_fps, sys_screensize, sys_sound, sys_bgm_volume, sys_se_volume;
#ifdef NETWORK
extern char net_name[64];
extern int net_score, net_block, net_evomode;
#endif

void Config_Load ();
void Config_Save ();

#ifdef TAILTALE_HD
char time_result[24];
#endif
char *time_dayandnight;

#ifndef _DINGOO // NATIV Dingoo
void DateTime ();
#endif

#if defined(ANDROID) || defined(WIN32)
FILE *fopenMEM (void *buf, size_t size, const char *opentype);
#endif

char *va (const char *fmt, ...);

int explode (char (*ap)[128], char *msg, char split);
