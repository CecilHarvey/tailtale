/**
 *  --- SDL puzzle project 'tailtale'.
 *
 */

/*-------------------------------*/
/* include                       */
/*-------------------------------*/

#include "default.h"

/*-------------------------------*/
/* local value                   */
/*-------------------------------*/

TGameScreen  *scr;
TGameMain  *gamemain;

int NowTiming;

/*-------------------------------*/
/* local function                */
/*-------------------------------*/

extern int Language ();

/* --------------------------------------------- */
/* --- メインルーチン                          --- */
/* --------------------------------------------- */

#if defined(ANDROID) || defined(WINPHONE)
int main (int argc, char* args[]) {
#else
int main (int argc, char** argv) {
#endif
  int  i;

  /* --- タイムカウント */
  int  SystemTime;
  int  BeforeTiming;
  int  WorkTime;
  int  DispTime;
  int  FrameCounter;
  int  FrameSkip;

  /* --- メインルーチンループフラグ */
  int  endflag;

  WorkTime = 0;
  FrameSkip = 0;

#if defined(GEKKO)
  if (!fatInitDefault ())
    printf ("fatInitDefault failed: Unable to initialize FAT!\n");
#endif

#if defined(MACOSX)
  chdir(SDL_GetBasePath());
#endif
    
  Config_Load ();

  /* ----- SDL 初期化 */
#if defined(_DINGOO) || defined(WINPHONE) // NATIV
  i = SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO);
#elif defined(VS_WIN) || defined(VS_WIN8)
  i = SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
#else
  i = SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK);
#endif
  if (i != 0) {
    /* --- SDLが初期化できなかった */
    printf ("SDL_Init failed: %s\n", SDL_GetError ());
    exit(-1);
  }

#if defined(X86)
  TGameScreen_SetWMName ("TailTale - Shippo no Puzzle");
#endif

  /* --- 終了フラグ */
  endflag = 0;

  scr = TGameScreen_Create (SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH);

  /* ----- ハードウェア初期化 */
  InputInit ();
  SoundInit ();
  FontInit ();
  Language ();

  SystemTime = SDL_GetTicks();
  BeforeTiming = SystemTime;
  DispTime = 0;
  FrameCounter = 0;
  srand(1);

  /* ----- ゲーム本体初期化 */
  gamemain = TGameMain_Create(scr);

  /* ---------------------------------------- */
  /* ----- メインルーチン                     */
  /* ---------------------------------------- */
  while (endflag == 0) {
    /* --- ゲーム本体ループ */
    for(i = 0; i <= FrameSkip; i++) {
      FrameCounter = FrameCounter + 1;
      InputPoll();
      /* ------------------------------- */
      /* ----- ゲームメインへ */
      if ((TGameMain_Poll(gamemain, FrameCounter) == FALSE) || (InputExit() != 0)) {
        BeforeTiming = SDL_GetTicks();
	    break;
      }
    }

    WorkTime = SDL_GetTicks() - BeforeTiming;

    /* --- フレーム終了、スクリーン描画 */
	TGameScreen_RefreshScreen(scr);

    /* --- フレームタイマー */
    DispTime = SDL_GetTicks() - BeforeTiming;

    NowTiming = (1000.0 / (FRAME_RATE - 8 * sys_frameskip)) - WorkTime;
	if ((NowTiming > 0) && (NowTiming <= (1000.0 / (FRAME_RATE - 8 * sys_frameskip))))
      SDL_Delay (NowTiming);

    endflag = InputExit();
    FrameSkip = DispTime / (1000.0 / (FRAME_RATE - 8 * sys_frameskip));
    if (FrameSkip > FRAME_SKIP_MAX)
      FrameSkip = FRAME_SKIP_MAX;
    BeforeTiming = SDL_GetTicks();

#ifndef _DINGOO // NATIV Dingoo
	DateTime ();
#endif

    /* ---------------------------------------- */
    /* ----- メインループココまで               */
  }

  /* --- ゲームメインループ終了 */
  TGameMain_Destroy(gamemain);

  /* --- スクリーン解放 */
  TGameScreen_Destroy(scr);

  /* --- 終了、SDL 後処理 */
  disptitle = 0;
  dispmenu = 0;
  dispsetting = 0;
  dispquit = 0;

  SoundFree ();
  InputFree ();
  FontFree ();

#ifdef NETWORK
  dispnetwork = 0;
  TNetwork_Close ();
#endif

  Config_Save ();

#ifdef __GP2X__
  chdir("/usr/gp2x");
  execl("gp2xmenu", "gp2xmenu", NULL);
#else
  SDL_Quit();
#endif

  exit(0);
  return 0;
}
