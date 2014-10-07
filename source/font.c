/*
	SDL puzzle project
*/

#include "default.h"

int FontInit () {
#ifdef LIBPAK
    unsigned int size;
    byte *buffer;
#endif
#ifndef SDL2
	SDL_EnableUNICODE (1);
#endif

	if (TTF_Init () == -1) {
		printf ("TTF_Init failed: %s\n", TTF_GetError ());
		return 0;
	}

#ifdef LIBPAK
  buffer = extractMem("language/code2000.ttf", openPak("data.pak"), &size);
  if (buffer == NULL)
    return 0;

  FontSmall = TTF_OpenFontRW (SDL_RWFromMem (buffer, size), 0, 13);
#else
	FontSmall = TTF_OpenFont (va ("%s/language/code2000.ttf", BASEDIR), 13);
#endif
	if (FontSmall == NULL) {
		printf ("TTF_OpenFont failed: %s\n", TTF_GetError ());
		return 0;
	}

#ifdef LIBPAK
  buffer = extractMem("language/code2000.ttf", openPak("data.pak"), &size);
  if (buffer == NULL)
    return 0;

  FontBig = TTF_OpenFontRW (SDL_RWFromMem (buffer, size), 0, 16);
#else
	FontBig = TTF_OpenFont (va ("%s/language/code2000.ttf", BASEDIR), 16);
#endif
	if (FontBig == NULL) {
		printf ("TTF_OpenFont failed: %s\n", TTF_GetError ());
		return 0;
	}

	return 1;
}

void FontFree () {
	TTF_CloseFont (FontSmall);
	TTF_CloseFont (FontBig);

	TTF_Quit ();
}

void FontPrint (TTF_Font *ttf, TGameScreen *class, int x, int y, int center, char *txt) {
	Uint16 unicode[1024];
	SDL_Rect rect_bg, rect;
	SDL_Surface *text_bg = NULL, *text = NULL;
	va_list arg;
	int i;

	SDL_Color black = { 0, 0, 0, 0 };
	SDL_Color white = { 255, 255, 255, 0 };

	switch (rendertype) {
		case RENDER_LATIN1:
			text_bg = TTF_RenderText_Solid (ttf, txt, black);
			text = TTF_RenderText_Solid (ttf, txt, white);
			break;

		case RENDER_UTF8:
			text_bg = TTF_RenderUTF8_Solid (ttf, txt, black);
			text = TTF_RenderUTF8_Solid (ttf, txt, white);
			break;

		case RENDER_UNICODE:
			for (i = 0; txt[i]; i++)
				unicode[i] = txt[i];
			unicode[i] = 0;

			text_bg = TTF_RenderUNICODE_Solid (ttf, unicode, black);
			text = TTF_RenderUNICODE_Solid (ttf, unicode, white);
			break;
	}

	if ((text_bg == NULL) || (text == NULL)) {
		fprintf (stdout, "TTF_RenderUTF8_Solid failed: %s\n", TTF_GetError ());
	} else {
		if (center) {
#ifdef TAILTALE_HD
			rect_bg.x = ((480 - text_bg->w) / 2) + 1, rect_bg.y = y + 1;
			rect.x = (480 - text_bg->w) / 2, rect.y = y + 1;
#else
			rect_bg.x = ((320 - text_bg->w) / 2) + 1, rect_bg.y = y + 1;
			rect.x = (320 - text_bg->w) / 2, rect.y = y;
#endif
		} else {
			rect_bg.x = x + 1, rect_bg.y = y + 1;
			rect.x = x, rect.y = y;
		}

		SDL_BlitSurface (text_bg, NULL, class->Screen, &rect_bg);
		SDL_BlitSurface (text, NULL, class->Screen, &rect);

		SDL_FreeSurface (text_bg);
		SDL_FreeSurface (text);
	}
}

#ifdef NETWORK
int osk_pos_x, osk_pos_y;
int osk_text_shift;
unsigned int max_len;

char *osk_out_buff;
char osk_buffer[128];

void Font_GetOSK (char *input, char *output, int outlen) {
	max_len = outlen;
	strncpy (osk_buffer, input, max_len);
	osk_buffer[outlen] = '\0';
	osk_out_buff = output;
}
#endif

/*
	Rikku2000: Draw Game Font, its not the best way...
*/

extern int NowTiming;
int disptitle, dispmenu, dispsetting, dispquit, cursorsetting;

#ifdef NETWORK
int dispnetwork, osk_text_shift;

char *osk_textdisp[] = {
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
	"a", "b", "c", "d", "e", "f", "g", "h", "i", "j",
	"k", "l", "m", "n", "o", "p", "q", "r", "s", "t",
	"u", "v", "w", "x", "y", "z", ".", "-", "_", " "
};

char *osk_textdisp_shift[] = {
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
	"A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
	"K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
	"U", "V", "W", "X", "Y", "Z", ".", "-", "_", " "
};

extern int client_connected, is_server, wait_client;
extern int network_game_win_score;
#endif

extern char language[256][256];

void FontDraw (TGameScreen *class)
{
#ifdef NETWORK
  int n, x = 0, y = 0;
#ifdef OSK
  int i;
#endif
#endif
  int addmenu, is_hd_x = 0, is_hd_y = -21;
#ifdef TAILTALE_HD
  is_hd_x = 80, is_hd_y = 0; // for center some...
#endif

#ifdef NETWORK
  if ((dispnetwork == 3) || (dispnetwork == 5))
    x = 84, y = 139;

  addmenu = 12;
#else
  addmenu = 0;
#endif

  if (sys_fps == 1)
    FontPrint (FontSmall, class, 8, 8, 0, va ("FPS: %02.2f", 1000.0 / NowTiming));

  if (disptitle) {
#ifdef TAILTALE_HD
    FontPrint (FontBig, class, 0, 128, 1, "www.tailtale.u-ac.net");

    if (((class->timer / 8) % 2) == 1)
      FontPrint (FontBig, class, 0, 180, 1, language[0]);

    FontPrint (FontBig, class, 0, 226, 1, "BigVioSoft Entertainment");
    FontPrint (FontBig, class, 0, 244, 1, "Copyright 2013, All Right Reserved.");
#else
    FontPrint (FontBig, class, 0, 100, 1, "www.tailtale.u-ac.net");

    if (((class->timer / 8) % 2) == 1)
      FontPrint (FontBig, class, 0, 143, 1, language[0]);

    FontPrint (FontBig, class, 0, 194, 1, "BigVioSoft Entertainment");
    FontPrint (FontBig, class, 0, 212, 1, "Copyright 2013, All Right Reserved.");
#endif
  }

  if (dispmenu) {
#ifdef TAILTALE_HD
    FontPrint (FontBig, class, 176 + is_hd_x, 20 + is_hd_y, 0, language[1]);
#else
    FontPrint (FontBig, class, 16 + is_hd_x, 26 + is_hd_y, 0, language[1]);
#endif

    FontPrint (FontSmall, class, 32 + is_hd_x, 154 + is_hd_y, 0, language[2]);
#ifdef NETWORK
    FontPrint (FontSmall, class, 32 + is_hd_x, 166 + is_hd_y, 0, language[3]);
#endif
    FontPrint (FontSmall, class, 32 + is_hd_x, 166 + addmenu + is_hd_y, 0, language[4]);
    FontPrint (FontSmall, class, 32 + is_hd_x, 178 + addmenu + is_hd_y, 0, language[5]);
    FontPrint (FontSmall, class, 32 + is_hd_x, 190 + addmenu + is_hd_y, 0, language[6]);
  }

#ifdef NETWORK
  if (network_game) { // Draw some Client/Server Data
	if ((is_server) && (client_connected == 0)) {
#ifdef TAILTALE_HD
	  FontPrint (FontSmall, class, 0, 130, 1, language[60]);
#else
	  FontPrint (FontSmall, class, 0, 114, 1, language[60]);
#endif
	}

  if (net_msgstart == 1) { // ChatMSG
	FontPrint (FontSmall, class, 8, 8, 0, va ("Say: %s", net_msgprev));
  } else { // ChatLog
	FontPrint (FontSmall, class, 8, 8, 0, va ("%s: %s", network_words[0], network_words[10]));
  }

#ifdef TAILTALE_HD
	FontPrint (FontSmall, class, 308, 190, 0, va ("%s", network_words[0]));
	  FontPrint (FontSmall, class, 308, 202, 0, "Score");
	   FontPrint (FontSmall, class, 308, 214, 0, va ("%s / %s", network_words[1], network_words[8]));
	  FontPrint (FontSmall, class, 308, 225, 0, "Level");
	   FontPrint (FontSmall, class, 308, 237, 0, va ("%s", network_words[2]));
#else
	FontPrint (FontSmall, class, 206, 172, 0, va ("%s", network_words[0]));
	  FontPrint (FontSmall, class, 206, 184, 0, "Score");
	   FontPrint (FontSmall, class, 206, 196, 0, va ("%s / %s", network_words[1], network_words[8]));
	  FontPrint (FontSmall, class, 206, 208, 0, "Level");
	   FontPrint (FontSmall, class, 206, 220, 0, va ("%s", network_words[2]));
#endif
  }

  if (dispnetwork == 1) {
#ifdef TAILTALE_HD
    FontPrint (FontBig, class, 176 + is_hd_x, 20 + is_hd_y, 0, language[7]);
#else
    FontPrint (FontBig, class, 16 + is_hd_x, 26 + is_hd_y, 0, language[7]);
#endif
      FontPrint (FontSmall, class, 32 + is_hd_x, 62 + is_hd_y, 0, language[8]);
      FontPrint (FontSmall, class, 32 + is_hd_x, 74 + is_hd_y, 0, language[9]);
      FontPrint (FontSmall, class, 32 + is_hd_x, 86 + is_hd_y, 0, language[58]);
      FontPrint (FontSmall, class, 32 + is_hd_x, 98 + is_hd_y, 0, language[10]);

    if (cursorsetting == 0)
      FontPrint (FontSmall, class, 0, 192 + is_hd_y, 1, language[11]);
    else if (cursorsetting == 1)
      FontPrint (FontSmall, class, 0, 192 + is_hd_y, 1, language[12]);
    else if (cursorsetting == 2)
      FontPrint (FontSmall, class, 0, 192 + is_hd_y, 1, language[59]);
    else if (cursorsetting == 3)
      FontPrint (FontSmall, class, 0, 192 + is_hd_y, 1, language[13]);
  } else if (dispnetwork == 2) {
#ifdef TAILTALE_HD
    FontPrint (FontBig, class, 176 + is_hd_x, 20 + is_hd_y, 0, language[14]);
#else
    FontPrint (FontBig, class, 16 + is_hd_x, 26 + is_hd_y, 0, language[14]);
#endif
      FontPrint (FontSmall, class, 32 + is_hd_x, 62 + is_hd_y, 0, language[15]);
        FontPrint (FontSmall, class, 232 + is_hd_x, 62 + is_hd_y, 0, skill[sys_level]);
      FontPrint (FontSmall, class, 32 + is_hd_x, 74 + is_hd_y, 0, language[16]);
        FontPrint (FontSmall, class, 232 + is_hd_x, 74 + is_hd_y, 0, va ("%d", 10000 + (net_score * 5000)));
      FontPrint (FontSmall, class, 32 + is_hd_x, 86 + is_hd_y, 0, language[61]);
        FontPrint (FontSmall, class, 232 + is_hd_x, 86 + is_hd_y, 0, va ("%d", 120 + (net_block * 80)));
      FontPrint (FontSmall, class, 32 + is_hd_x, 98 + is_hd_y, 0, language[62]);
        FontPrint (FontSmall, class, 232 + is_hd_x, 98 + is_hd_y, 0, va ("%s", (net_evomode) ? language[47] : language[48]));

    if (cursorsetting == 0)
      FontPrint (FontSmall, class, 0, 192 + is_hd_y, 1, language[17]);
    else if (cursorsetting == 1)
      FontPrint (FontSmall, class, 0, 192 + is_hd_y, 1, language[18]);
  } else if ((dispnetwork == 3) || (dispnetwork == 5)) {
    if (dispnetwork == 3) {
      FontPrint (FontBig, class, 176 + is_hd_x, 20 + is_hd_y, 0, language[19]);
        FontPrint (FontSmall, class, 0, 62, 1, language[20]);
        FontPrint (FontSmall, class, 0, 74, 1, language[21]);

      FontPrint (FontSmall, class, 55 + is_hd_x, 115 + is_hd_y, 0, va ("IP: %s_", osk_buffer));
	} else if (dispnetwork == 5) {
#ifdef TAILTALE_HD
	  FontPrint (FontBig, class, 176 + is_hd_x, 20 + is_hd_y, 0, language[22]);
#else
	  FontPrint (FontBig, class, 16 + is_hd_x, 26 + is_hd_y, 0, language[22]);
#endif
        FontPrint (FontSmall, class, 0, 62, 1, language[23]);

      FontPrint (FontSmall, class, 55 + is_hd_x, 115 + is_hd_y, 0, va ("Name: %s_", osk_buffer));
	}

#ifdef OSK
    for (n = 0; n <= MAX_X; n++)
      for (i = 0; i <= MAX_Y; i++)
        if (osk_text_shift == 1)
          FontPrint (FontSmall, class, x + is_hd_x + (16 * n), y + (16 * i) + is_hd_y, 0, osk_textdisp_shift[n + i * 10]);
        else
          FontPrint (FontSmall, class, x + is_hd_x + (16 * n), y + (16 * i) + is_hd_y, 0, osk_textdisp[n + i * 10]);
#endif
  } else if (dispnetwork == 4) {
#ifdef TAILTALE_HD
	FontPrint (FontBig, class, 176 + is_hd_x, 20 + is_hd_y, 0, language[58]);
#else
	FontPrint (FontBig, class, 16 + is_hd_x, 26 + is_hd_y, 0, language[58]);
#endif
	if (wait_slist == 1) {
		if (slist == NULL) {
#ifdef TAILTALE_HD
	  FontPrint (FontSmall, class, 0, 130, 1, "No Servers found!");
#else
	  FontPrint (FontSmall, class, 0, 114, 1, "No Servers found!");
#endif
		} else {
			FontPrint (FontSmall, class, 32 + is_hd_x, 56 + is_hd_y, 1, "IP                  Host           System");
			FontPrint (FontSmall, class, 32 + is_hd_x, 59 + is_hd_y, 1, "_________________________________________");
			for (n = 0; n <= 9; n++) {
				FontPrint (FontSmall, class, 32 + is_hd_x, 72 + (12 * n) + is_hd_y, 0, slist[n]);
				FontPrint (FontSmall, class, 132 + is_hd_x, 72 + (12 * n) + is_hd_y, 0, netlist_words[1 + (n * 2)]);
				FontPrint (FontSmall, class, 222 + is_hd_x, 72 + (12 * n) + is_hd_y, 0, netlist_words[2 + (n * 2)]);
			}
		}
	} else {
#ifdef TAILTALE_HD
	  FontPrint (FontSmall, class, 0, 130, 1, "Search for Servers!");
#else
	  FontPrint (FontSmall, class, 0, 114, 1, "Search for Servers!");
#endif
	}

    FontPrint (FontSmall, class, 0, 192 + is_hd_y, 1, language[59]);
  }
#endif

  if (dispsetting == 1) {
#ifdef TAILTALE_HD
	FontPrint (FontBig, class, 176 + is_hd_x, 20 + is_hd_y, 0, language[24]);
#else
	FontPrint (FontBig, class, 16 + is_hd_x, 26 + is_hd_y, 0, language[24]);
#endif
      FontPrint (FontSmall, class, 32 + is_hd_x, 60 + is_hd_y, 0, language[25]);
        FontPrint (FontSmall, class, 232 + is_hd_x, 60 + is_hd_y, 0, skill[sys_level]);
      FontPrint (FontSmall, class, 32 + is_hd_x, 72 + is_hd_y, 0, language[26]);
        FontPrint (FontSmall, class, 232 + is_hd_x, 72 + is_hd_y, 0, va ("%s", (sys_debug) ? language[47] : language[48]));
      FontPrint (FontSmall, class, 32 + is_hd_x, 84 + is_hd_y, 0, language[27]);
        FontPrint (FontSmall, class, 232 + is_hd_x, 84 + is_hd_y, 0, language[49 + sys_language]);
      FontPrint (FontSmall, class, 32 + is_hd_x, 96 + is_hd_y, 0, language[28]);
        FontPrint (FontSmall, class, 232 + is_hd_x, 96 + is_hd_y, 0, va ("%s", (sys_fullscreen) ? language[47] : language[48]));
      FontPrint (FontSmall, class, 32 + is_hd_x, 108 + is_hd_y, 0, language[29]);
#if defined(DINGOO) || defined(GPH)
        FontPrint (FontSmall, class, 232 + is_hd_x, 108 + is_hd_y, 0, "320x240");
#elif defined(GEKKO)
        FontPrint (FontSmall, class, 232 + is_hd_x, 108 + is_hd_y, 0, "640x480");
#elif defined(ANDROID) || defined(PSP)
        FontPrint (FontSmall, class, 232 + is_hd_x, 108 + is_hd_y, 0, "480x272");
#else
        FontPrint (FontSmall, class, 232 + is_hd_x, 108 + is_hd_y, 0, va ("%s", screensize[sys_screensize]));
#endif
      FontPrint (FontSmall, class, 32 + is_hd_x, 120 + is_hd_y, 0, language[30]);
        FontPrint (FontSmall, class, 232 + is_hd_x, 120 + is_hd_y, 0, va ("%d", sys_frameskip));
      FontPrint (FontSmall, class, 32 + is_hd_x, 132 + is_hd_y, 0, language[31]);
        FontPrint (FontSmall, class, 232 + is_hd_x, 132 + is_hd_y, 0, va ("%s", (sys_fps) ? language[47] : language[48]));
      FontPrint (FontSmall, class, 32 + is_hd_x, 144 + is_hd_y, 0, language[32]);
        FontPrint (FontSmall, class, 232 + is_hd_x, 144 + is_hd_y, 0, va ("%s", (sys_joystick_swap) ? language[47] : language[48]));
      FontPrint (FontSmall, class, 32 + is_hd_x, 156 + is_hd_y, 0, language[33]);
        FontPrint (FontSmall, class, 232 + is_hd_x, 156 + is_hd_y, 0, va ("%s", (sys_sound) ? language[47] : language[48]));
      FontPrint (FontSmall, class, 32 + is_hd_x, 168 + is_hd_y, 0, language[34]);
        FontPrint (FontSmall, class, 232 + is_hd_x, 168 + is_hd_y, 0, va ("%d", sys_bgm_volume));
      FontPrint (FontSmall, class, 32 + is_hd_x, 180 + is_hd_y, 0, language[35]);
        FontPrint (FontSmall, class, 232 + is_hd_x, 180 + is_hd_y, 0, va ("%d", sys_se_volume));

    if (cursorsetting == 0)
      FontPrint (FontSmall, class, 0, 198 + is_hd_y, 1, language[36]);
    else if (cursorsetting == 1)
      FontPrint (FontSmall, class, 0, 198 + is_hd_y, 1, language[37]);
    else if (cursorsetting == 2)
      FontPrint (FontSmall, class, 0, 198 + is_hd_y, 1, language[38]);
    else if (cursorsetting == 3)
      FontPrint (FontSmall, class, 0, 198 + is_hd_y, 1, language[39]);
    else if (cursorsetting == 4)
      FontPrint (FontSmall, class, 0, 198 + is_hd_y, 1, language[40]);
    else if (cursorsetting == 5)
      FontPrint (FontSmall, class, 0, 198 + is_hd_y, 1, language[41]);
    else if (cursorsetting == 6)
      FontPrint (FontSmall, class, 0, 198 + is_hd_y, 1, language[42]);
    else if (cursorsetting == 7)
      FontPrint (FontSmall, class, 0, 198 + is_hd_y, 1, language[43]);
    else if (cursorsetting == 8)
      FontPrint (FontSmall, class, 0, 198 + is_hd_y, 1, language[44]);
    else if (cursorsetting == 9)
      FontPrint (FontSmall, class, 0, 198 + is_hd_y, 1, language[45]);
    else if (cursorsetting == 10)
      FontPrint (FontSmall, class, 0, 198 + is_hd_y, 1, language[46]);
  }

  if (dispquit == 1) {
    // FontPrint (FontBig, class, 176 + is_hd_x, 20 + is_hd_y, 0, va ("Credits: %d/4", 1 + cursorsetting));
    if (cursorsetting == 0) {
		FontPrint (FontBig, class, 0, 98, 1, "TailTale");
		FontPrint (FontSmall, class, 0, 110, 1, "Shippo no Puzzle");
		FontPrint (FontSmall, class, 0, 122, 1, VERSION);
		FontPrint (FontSmall, class, 0, 146, 1, "www.tailtale.u-ac.net");
	} else if (cursorsetting == 1) {
		FontPrint (FontBig, class, 0, 48, 1, "Developer-Team");
		FontPrint (FontSmall, class, 0, 66, 1, "Kentaru Kunikane");
		FontPrint (FontSmall, class, 0, 78, 1, "Martin Dieter");
		FontPrint (FontSmall, class, 0, 90, 1, "Misaki Kimiko");
		FontPrint (FontSmall, class, 0, 102, 1, "Wei Mingzhi");
		FontPrint (FontSmall, class, 0, 114, 1, "Qinrui Guo");
		FontPrint (FontSmall, class, 0, 126, 1, "Francisco de la Pena");
		FontPrint (FontSmall, class, 0, 138, 1, "Artur Rojek");
		FontPrint (FontSmall, class, 0, 150, 1, "Asceline Dury");
		FontPrint (FontSmall, class, 0, 162, 1, "Akizuki Katane");
		FontPrint (FontSmall, class, 0, 174, 1, "Scott Smith");
		FontPrint (FontSmall, class, 0, 186, 1, "Nilton Rufino");
	} else if (cursorsetting == 2) {
		FontPrint (FontSmall, class, 0, 114, 1, "Thank You for Playing");
		FontPrint (FontSmall, class, 0, 126, 1, "TailTale");
		FontPrint (FontSmall, class, 0, 138, 1, "Shippo no Puzzle");
	}
  }

  class->timer = class->timer + 1;
}
