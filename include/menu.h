
/* ---------------------------------------------------------- */
/*  menu.h                                                */
/* ---------------------------------------------------------- */

/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintaner  Rerorero@fumi.   */
/*                                                        */
/*   menu.h                                           */
/*     ゲームの本体フロー                                 */
/*                                                        */
/*--------------------------------------------------------*/

#ifndef MENU_H
#define MENU_H

/*-------------------------------*/
/* define                        */
/*-------------------------------*/

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* --- ゲームステップのラベル */
enum {
  TitleInit,
  TitleIn,
  TitleHandle,
  TitleSplash,
  TitleMain,
  TitleStart,
  TitleFree,
  PuzzleInit,
  PuzzleMain,
  PuzzleFree,
  MenuInit,
  MenuMain,
  MenuFree,
#ifdef NETWORK
  NetworkInit,
  NetworkMain,
  NetworkFree,
#endif
  ConfigInit,
  ConfigMain,
  ConfigFree,
  HelpInit,
  HelpMain,
  HelpFree,
  QuitInit,
  QuitMain,
  QuitFree
} MainStep;

/*-------------------------------*/
/* value                         */
/*-------------------------------*/

extern char *screensize [];
extern char *skill [];

/*-------------------------------*/
/* struct                        */
/*-------------------------------*/ 

typedef struct {
  int  titletimer;
  SDL_Surface  *tex;
  TGameSprite  *bg, *logo, *lang, *splash1, *splash2, *cursor, *help, *menu_bottom, *menu_top, *menu
#ifdef OSK
  , *osk, *osk_select
#endif
#ifdef TOUCH
  , *tarrow, *tbtn1, *tbtn2, *tbtn3, *tbtn4
#endif
  , ready, go, gameover;
  TGameScreen  *screen;
  int  level;
  int  step;
  int  init;
  TPuzzleTrial  *puzzle;
} TGameMain, *PTGameMain;

/* ---------------------------------------------- */
/* --- extern                                  -- */
/* ---------------------------------------------- */

TGameMain *TGameMain_Create(TGameScreen *mainscreen);
void TGameMain_Destroy(TGameMain *class);

int TGameMain_Poll(TGameMain *class,
		   int  counter);


#endif //MENU_H
