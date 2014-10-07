/*
	SDL puzzle project
*/

#ifndef _FONT_H_
#define _FONT_H_

enum {
	RENDER_LATIN1,
	RENDER_UTF8,
	RENDER_UNICODE
} rendertype;

TTF_Font *FontSmall, *FontBig;

#define FONT_SIZE 64

int FontInit ();
void FontFree ();

void FontPrint (TTF_Font *ttf, TGameScreen *class, int x, int y, int center, char *txt);

#ifdef NETWORK
#define CHAR_SIZE 8
#define MAX_Y 3
#define MAX_X 9

extern int osk_pos_x, osk_pos_y;
extern int osk_text_shift;
extern unsigned int max_len;

extern char *osk_out_buff;
extern char osk_buffer[128];

void Font_GetOSK (char *input, char *output, int outlen);
#endif

void FontDraw (TGameScreen *class);

#endif
