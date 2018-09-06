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
/* --- ���C�����[�`��                          --- */
/* --------------------------------------------- */

#if defined(ANDROID) || defined(WINPHONE)
int main (int argc, char* args[]) {
#else
int main (int argc, char** argv) {
#endif
  int  i;

  /* --- �^�C���J�E���g */
  int  SystemTime;
  int  BeforeTiming;
  int  WorkTime;
  int  DispTime;
  int  FrameCounter;
  int  FrameSkip;

  /* --- ���C�����[�`�����[�v�t���O */
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

  /* ----- SDL ������ */
#if defined(_DINGOO) || defined(WINPHONE) // NATIV
  i = SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO);
#elif defined(VS_WIN) || defined(VS_WIN8)
  i = SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
#else
  i = SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK);
#endif
  if (i != 0) {
    /* --- SDL���������ł��Ȃ����� */
    printf ("SDL_Init failed: %s\n", SDL_GetError ());
    exit(-1);
  }

#if defined(X86)
  TGameScreen_SetWMName ("TailTale - Shippo no Puzzle");
#endif

  /* --- �I���t���O */
  endflag = 0;

  scr = TGameScreen_Create (SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH);

  /* ----- �n�[�h�E�F�A������ */
  InputInit ();
  SoundInit ();
  FontInit ();
  Language ();

  SystemTime = SDL_GetTicks();
  BeforeTiming = SystemTime;
  DispTime = 0;
  FrameCounter = 0;
  srand(1);

  /* ----- �Q�[���{�̏����� */
  gamemain = TGameMain_Create(scr);

  /* ---------------------------------------- */
  /* ----- ���C�����[�`��                     */
  /* ---------------------------------------- */
  while (endflag == 0) {
    /* --- �Q�[���{�̃��[�v */
    for(i = 0; i <= FrameSkip; i++) {
      FrameCounter = FrameCounter + 1;
      InputPoll();
      /* ------------------------------- */
      /* ----- �Q�[�����C���� */
      if ((TGameMain_Poll(gamemain, FrameCounter) == FALSE) || (InputExit() != 0)) {
        BeforeTiming = SDL_GetTicks();
	    break;
      }
    }

    WorkTime = SDL_GetTicks() - BeforeTiming;

    /* --- �t���[���I���A�X�N���[���`�� */
	TGameScreen_RefreshScreen(scr);

    /* --- �t���[���^�C�}�[ */
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
    /* ----- ���C�����[�v�R�R�܂�               */
  }

  /* --- �Q�[�����C�����[�v�I�� */
  TGameMain_Destroy(gamemain);

  /* --- �X�N���[����� */
  TGameScreen_Destroy(scr);

  /* --- �I���ASDL �㏈�� */
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
