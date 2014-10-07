/* ---------------------------------------------------------- */
/*  trial.c                                                   */
/* ---------------------------------------------------------- */

/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintaner  Rerorero@fumi.   */
/*                                                        */
/*   trial.c                                              */
/*     �p�Y���Ƃ��Ƃ񃂁[�h                               */
/*                                                        */
/*--------------------------------------------------------*/

/*------------------------------------------------------------- */
/** @file
    @brief		�p�Y���Ƃ��Ƃ񃂁[�h
    @author		K.Kunikane (rerofumi)
    @since		Sep.30.2005
*/
/*-----------------------------------------------------
 Copyright (C) 2002,2005 rerofumi <rero2@yuumu.org>
 All Rights Reserved.
 ------------------------------------------------------*/

/*-------------------------------*/
/* include                       */
/*-------------------------------*/

#include "default.h"

/*-------------------------------*/
/* local value                   */
/*-------------------------------*/

/*-------------------------------*/
/* local function                */
/*-------------------------------*/

void  ChangeCharacter(TPuzzleTrial *class, int level);

#ifdef MOVIE
extern int play_movie (char *filename);
#endif

/*-------------------------------*/
/* data table                    */
/*-------------------------------*/


/* -------------------------------------------------------------- */
/* --- �Ƃ��Ƃ񃂁[�h�p�Y���N���X                                 */
/* -------------------------------------------------------------- */

/* ---------------------------------------- */
/* --- �R���X�g���N�^�E�f�X�g���N�^         */
TPuzzleTrial *TPuzzleTrial_Create(TGameScreen *scr, int level)
{
  TPuzzleTrial *class;

  /* --- �C���X�^���X�̊m�� */
  class = malloc(sizeof(TPuzzleTrial));
  /* --- �N���X�����o�[������ */
  class->step = TInit;
  class->status = TRUE;
  class->screen = scr;
  class->game_level = level;
  /* --- �e�N�X�`���[�ǂݍ��� */
  TPuzzleTrial_LoadTexture(class);

  /* --- �m�ۂł����珉���� */
  if (class) {
    /* -- �X�[�p�[�N���X�̎擾 */
    class->super = TPuzzleDisp_Create(MODE_CHALLENGE, level, scr);
  }

  /* --- �C���X�^���X��n���ďI�� */
  return(class);
}

void  TPuzzleTrial_Destroy(TPuzzleTrial *class)
{
  /* --- �X�[�p�[�N���X�̉�� */
  TPuzzleDisp_Destroy(class->super);
  /* --- �C���X�^���X�̉�� */
  free(class);
}

/* ---------------------------------------- */
/* --- �e�N�X�`���[�̓ǂݍ���               */
/* ---------------------------------------- */

void TPuzzleTrial_LoadTexture(TPuzzleTrial *class)
{
	char buffer_tex[64];

	sprintf (buffer_tex, "graphic/background/%s/lv1.tga", time_dayandnight);

#ifdef TAILTALE_HD
  TGameScreen_LoadTexturePure(class->screen, 0, "graphic/background/game_480.tga");
#else
  TGameScreen_LoadTexturePure(class->screen, 0, "graphic/background/game_320.tga");
#endif

#ifdef NETWORK
  if (network_game == 1) {
	if (evolution || atoi (network_words[9])) {
		TGameScreen_LoadTexturePure(class->screen, 2, "graphic/character/chara01_1.tga");
		TGameScreen_LoadTexture(class->screen, 3, buffer_tex);
	} else {
		TGameScreen_LoadTexturePure(class->screen, 2, "graphic/character/chara02_1.tga");
		TGameScreen_LoadTexture(class->screen, 3, buffer_tex);
	}
    TGameScreen_LoadTexturePure(class->screen, 5, "graphic/system/evo.tga");
  } else {
    TGameScreen_LoadTexturePure(class->screen, 2, "graphic/character/chara01_1.tga");
	TGameScreen_LoadTexture(class->screen, 3, buffer_tex);
  }
#else
  TGameScreen_LoadTexturePure(class->screen, 2, "graphic/character/chara01_1.tga");
  TGameScreen_LoadTexture(class->screen, 3, buffer_tex);
#endif

  TGameScreen_LoadTexturePure(class->screen, 4, "graphic/system/text.tga");
}


/* ---------------------------------------- */
/* --- �Q�[�����s�����ǂ�����Ԃ��t���O     */
/* ---------------------------------------- */
int TPuzzleTrial_GameStat(TPuzzleTrial *class)
{
  int  r;

  r = TRUE;
  if (class->step == TEnd) {
    r = FALSE;
  }
  return(r);
}

/* ---------------------------------------- */
/* --- �Q�[���{�̕����̎��s                 */
/* ---------------------------------------- */

#ifdef NETWORK
extern char network_words[16][128];
extern int network_game, network_run, is_server;
extern TCPsocket client, server;
extern int winner, loser;
int getnetinfo;
#endif

#ifdef TOUCH
extern int ingame;
#endif

void TPuzzleTrial_GameMain(TPuzzleTrial *class)
{
  int  i, j, k;

  /* --------------------------------------------- */
  /* --- �p�Y���Q�[���X�e�b�v  */
  switch(class->step) {
    /* --- �Q�[������ */
  case TInit:
    /* -- �Q�[�����[�N������(TPuzzleBase) */
    TPuzzleBase_GameInit(class->super->super, 1);
    TPuzzleBase_GamePause(class->super->super, STEP_PAUSE);
#ifdef TOUCH
	ingame = 1;
#endif
#ifdef NETWORK
	if (network_game) {
		if (is_server) {
			TNetwork_Check (); // check for Client
			if (client_connected) {
				SoundSE(3);
				class->readytimer = 0;
				class->step = TReady;
			}
		} else {
#ifdef MOVIE
			if (network_game == 0)
			  play_movie ("media/cg1.ogv");
#endif
			SoundSE(3);
			class->readytimer = 0;
			class->step = TReady;
		}
	} else {
#endif
#ifdef MOVIE
	play_movie ("media/cg1.ogv");
#endif
    SoundSE(3);
    class->readytimer = 0;
    class->step = TReady;
#ifdef NETWORK
	}
#endif
    break;

    /* --- �Q�[���J�n */
  case TReady:
    TPuzzleBase_GameExec(class->super->super);
    TPuzzleDisp_DispField(class->super);
    TPuzzleDisp_DispReady(class->super, class->readytimer);
    /* - ��莞�ԂŎ��� */
    class->readytimer = class->readytimer + 1;
    if (class->readytimer > (84*2)) {
#ifdef NETWORK
      if (network_game) {
		if (evolution || atoi (network_words[9])) {
		  SoundMusic(4);
		} else {
		  SoundMusic(3);
		}
	  } else { SoundMusic(4); }
#else
      SoundMusic(4);
#endif
      TPuzzleBase_GamePause(class->super->super, STEP_LINECHECK);
	  SoundSE(19);
      class->step = TGame;
    }
    break;

    /* --- �Q�[���{�� */
  case TGame:
    TPuzzleTrial_UserControl(class);
    TPuzzleBase_GameExec(class->super->super);
    if (TPuzzleBase_LevelCheck(class->super->super) == TRUE) {
      /* --- ���x���A�b�v���A�g���N�g */
      TPuzzleBase_GameLevel(class->super->super, class->super->super->Level);


#ifdef NETWORK
	if (network_game == 1) {
	  if (evolution || atoi (network_words[9]))
        ChangeCharacter(class, class->super->super->Level);
	} else {
#endif
      ChangeCharacter(class, class->super->super->Level);
#ifdef NETWORK
	}
#endif
	  SoundSE(2);
      for (i = 0; i < 16; i++) {
		TPuzzleDisp_KiraRequest (class->super, 444-152, 152, 80);
      }
    }
    TPuzzleDisp_DispField(class->super);
    /* - �Q�[���I�[�o�[�Ŏ��� */
#ifdef NETWORK
	  if (network_game) {
		if (atoi (network_words[7]) == 1) { // Player Win
			if (atoi (network_words[8]) == 1) { // Player Lose
				SoundSE(24);
				SoundMusicOneshot (1);
				getnetinfo = 1, class->readytimer = 0;
				class->step = TNetStatus;
			} else { // Player Win
				SoundMusicOneshot (10);
				getnetinfo = 0, class->readytimer = 0;
				class->step = TNetEnd;
			}
		} else if (atoi (network_words[8]) == 1) { // Player Lose
			SoundSE(24);
			SoundMusicOneshot (1);
			getnetinfo = 1, class->readytimer = 0;
			class->step = TNetStatus;
		} else {
			if (class->super->super->GameOver == TRUE) {
				if (atoi (network_words[7]) == 0)
					loser = 1; // FIXME: Let both win coz 1 have already lose...
				SoundSE(24);
				SoundMusicOneshot (1);
				TNetwork_UpdateDisp ();
				getnetinfo = 1, class->readytimer = 0;
				class->step = TNetStatus;
				// winner = 1;
			}
		}
	  } else {
#endif
		if (class->super->super->GameOver == TRUE) {
		  SoundMusicStop();
		  SoundSE (7);
		  class->readytimer = 0;
		  class->step = TMiss;
		}
#ifdef NETWORK
	  }
#endif
    break;

#ifdef NETWORK
  case TNetStatus:
    k = InputJoyKeyTriger(0);
	j = (class->readytimer / 2) + 1;

	for (i = 0; i < FIELD_WIDTH; i++) {
	  if (class->super->super->Field[j * FIELD_WIDTH + i] != 0) {
		if (class->super->super->Field[j * FIELD_WIDTH + i]->Color < 0x10) {
		  class->super->super->Field[j * FIELD_WIDTH + i]->Color = class->super->super->Field[j * FIELD_WIDTH + i]->Color + 0x10;
		}
	  }
	}

	TPuzzleDisp_DispField(class->super);

    class->readytimer = class->readytimer + 1;
    if (class->readytimer == ((FIELD_HEIGHT - 1) * 2)) {
	  class->readytimer = 0;
	  class->step = TNetEnd;
    }
    break;

  case TNetEnd:
	TPuzzleDisp_DispNetStatus (class->super, class->readytimer, getnetinfo);
    k = InputJoyKeyTriger(0);
    class->readytimer = class->readytimer + 1;
    if (class->readytimer > 160) {
      if ((k & (IN_Button1)) != 0) {
        SoundMusicStop();
		TNetwork_ReInit ();
	    class->step = TEnd;
      }
    }
    break;
#endif

    /* --- �I�� */
  case TMiss:
    /* -- �u���b�N���D�F�� */
    j = (class->readytimer / 2) + 1;

    for(i=0; i<FIELD_WIDTH; i++) {
      if (class->super->super->Field[j*FIELD_WIDTH + i] != 0) {
	    if (class->super->super->Field[j*FIELD_WIDTH + i]->Color < 0x10) {
		  class->super->super->Field[j*FIELD_WIDTH + i]->Color = class->super->super->Field[j*FIELD_WIDTH + i]->Color + 0x10;
	    }
      }
    }

    TPuzzleDisp_DispField(class->super);

    /* -- */
    class->readytimer = class->readytimer + 1;
    if (class->readytimer == ((FIELD_HEIGHT - 1) * 2)) {
	  class->readytimer = 0;
	  class->step = TGameover;
    }
    break;

    /* --- �Q�[���I�[�o�[ */
  case TGameover:
    TPuzzleDisp_DispGameover(class->super, class->readytimer);
    k = InputJoyKeyTriger(0);
    class->readytimer = class->readytimer + 1;
    if (class->readytimer > 160) {
      if ((k & (IN_Button1)) != 0) {
#ifdef TOUCH
		ingame = 0;
#endif
        SoundMusicStop();
	    class->step = TEnd;
      }
    }
    break;

    /* --- �p�Y���N���X�I�� */
  case TEnd:
    break;
  }
}


/* ---------------------------------------- */
/* --- ���[�U�[�R���g���[��                 */
/* ---------------------------------------- */
void TPuzzleTrial_UserControl(TPuzzleTrial *class)
{
  int  inp;

  /* --- ����ւ����͐��� */
  if (class->super->super->GameStep == STEP_SWAP) {
    return;
  }
  /* --- ���͂ƃz�[���h */
  class->super->super->UA.HaveBlock = FALSE;
  inp = InputJoyKey(0);

  if (((inp & (IN_Button3|IN_Button4|IN_Button5|IN_Button6)) != 0) && (class->super->super->Animation == FALSE)) {
    /* -- ���݂̃J�[�\���ʒu�Ƀu���b�N�͂��邩 */
    if (class->super->super->Field[class->super->super->UA.X + (class->super->super->UA.Y * FIELD_WIDTH)] != 0) {
      class->super->super->UA.HaveBlock = TRUE;
    }
  } else if ((inp & (IN_Button2)) != 0) {
    SoundMusicStop();
#ifdef NETWORK
	if (network_game)
		TNetwork_Close ();
#endif
#ifdef TOUCH
	ingame = 0;
#endif
    class->step = TEnd;
  }

  /* --- ���͂ƃJ�[�\���ړ� */
  inp = InputJoyKeyTriger(0);
  if (((inp & IN_Up) != 0) && (class->super->super->UA.Y < (FIELD_HEIGHT -1))) {
    if (class->super->super->UA.HaveBlock == TRUE) {
      TPuzzleBase_MoveRequest(class->super->super,
			      class->super->super->UA.X,
			      class->super->super->UA.Y,
			      MOVE_UP);
    }
    class->super->super->UA.Y = class->super->super->UA.Y + 1;
  }
  if (((inp & IN_Down) != 0) && (class->super->super->UA.Y > 1)) {
    if (class->super->super->UA.HaveBlock == TRUE) {
      TPuzzleBase_MoveRequest(class->super->super,
			      class->super->super->UA.X,
			      class->super->super->UA.Y,
			      MOVE_DOWN);
    }
    class->super->super->UA.Y = class->super->super->UA.Y - 1;
  }
  if (((inp & IN_Left) != 0) && (class->super->super->UA.X > 0)) {
    if (class->super->super->UA.HaveBlock == TRUE) {
      TPuzzleBase_MoveRequest(class->super->super,
			      class->super->super->UA.X,
			      class->super->super->UA.Y,
			      MOVE_LEFT);
    }
    class->super->super->UA.X = class->super->super->UA.X - 1;
  }
  if (((inp & IN_Right) != 0) && (class->super->super->UA.X < (FIELD_WIDTH -1))) {
    if (class->super->super->UA.HaveBlock == TRUE) {
      TPuzzleBase_MoveRequest(class->super->super,
			      class->super->super->UA.X,
			      class->super->super->UA.Y,
			      MOVE_RIGHT);
    }
    class->super->super->UA.X = class->super->super->UA.X + 1;
  }

}


/* -------------------------------------------------------------- */
/* --- �N���X���\�b�h                                             */
/* -------------------------------------------------------------- */


/* ---------------------------------------- */
/* --- �L�����N�^�[�ύX                     */
/* ---------------------------------------- */
void  ChangeCharacter(TPuzzleTrial *class, int level) {
	char buffer_tex[64];

	switch (level) {
		case 6:
			SoundMusic(5);
			TGameScreen_LoadTexturePure(class->screen, 2, "graphic/character/chara01_2.tga");
			sprintf (buffer_tex, "graphic/background/%s/lv2.tga", time_dayandnight);
			TGameScreen_LoadTexture(class->screen, 3, buffer_tex);
			SoundSE(23);
			break;
		case 11:
#ifdef MOVIE
#ifdef NETWORK
			if (network_game == 0)
#endif
			  play_movie ("media/cg2.ogv");
#endif
			SoundMusic(6);
			TGameScreen_LoadTexturePure(class->screen, 2, "graphic/character/chara01_3.tga");
			sprintf (buffer_tex, "graphic/background/%s/lv3.tga", time_dayandnight);
			TGameScreen_LoadTexture(class->screen, 3, buffer_tex);
			SoundSE(23);
			break;
		case 16:
			SoundMusic(7);
			TGameScreen_LoadTexturePure(class->screen, 2, "graphic/character/chara01_4.tga");
			sprintf (buffer_tex, "graphic/background/%s/lv4.tga", time_dayandnight);
			TGameScreen_LoadTexture(class->screen, 3, buffer_tex);
			SoundSE(23);
			break;
		case 21:
			SoundMusic(8);
			TGameScreen_LoadTexturePure(class->screen, 2, "graphic/character/chara01_5.tga");
			sprintf (buffer_tex, "graphic/background/%s/lv5.tga", time_dayandnight);
			TGameScreen_LoadTexture(class->screen, 3, buffer_tex);
			SoundSE(23);
			break;
		case 26:
#ifdef MOVIE
#ifdef NETWORK
			if (network_game == 0)
#endif
			  play_movie ("media/cg3.ogv");
#endif
			SoundMusic(9);
			TGameScreen_LoadTexturePure(class->screen, 2, "graphic/character/chara01_6.tga");
			sprintf (buffer_tex, "graphic/background/%s/lv5.tga", time_dayandnight);
			TGameScreen_LoadTexture(class->screen, 3, buffer_tex);
			SoundSE(23);
			break;
		case 31:
#ifdef MOVIE
#ifdef NETWORK
			if (network_game == 0) {
#endif
			  play_movie ("media/cg4.ogv");
#ifdef NETWORK
			  class->step = TEnd;
			}
#endif
#endif
			break;
	}
}
