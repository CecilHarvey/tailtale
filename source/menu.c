/* ---------------------------------------------------------- */
/*  menu.c                                               */
/* ---------------------------------------------------------- */

/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintaner  Rerorero@fumi.   */
/*                                                        */
/*   menu.c                                          */
/*     ゲーム本体のメインフロー              
             */
/*                                                        */
/*--------------------------------------------------------*/

/*------------------------------------------------------------- */
/** @file
    @brief		ゲーム本体ステップ
    @author		K.Kunikane (rerofumi)
    @since		Sep.20.2005
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

extern int AppExit;

/*-------------------------------*/
/* local function                */
/*-------------------------------*/

extern int Language ();

/* -------------------------------------------------------------- */
/* --- ゲームメインステップ                                       */
/* -------------------------------------------------------------- */

/* ---------------------------------------- */
/* --- コンストラクタ・デストラクタ         */
TGameMain *TGameMain_Create(TGameScreen *mainscreen)
{
  TGameMain *class;

  class = malloc(sizeof(TGameMain));
  if (class == NULL) return(0);

  class->screen = mainscreen;
  class->step = TitleInit;
  class->level = sys_level; // LevelEasy;

  return(class);
}

void TGameMain_Destroy(TGameMain *class)
{
  if (class == NULL) return;
  /* インスタンスの解放 */
  free(class);
}

/* ---------------------------------------- */
/* --- ゲームメイン                         */
/* ---------------------------------------- */

char *screensize [] = {
#ifndef TAILTALE_HD
  "320x240", "400x300",
#endif
  "480x272", "600x400", "640x480",
  "800x480", "800x600", "960x720",
  "1024x600", "1024x768", "1280x720"
};

char *skill [] = {
  "Easy", "Normal", "Hard", "Ultra"
};

#ifdef OSK
char *osk_text_key [] = {
	" 0 1 2 3 4 5 6 7 8 9 ",
	" a b c d e f g h i j ",
	" k l m n o p q r s t ",
	" u v w x y z . - _   "
};

char *osk_text_shift_key [] = {
	" 0 1 2 3 4 5 6 7 8 9 ",
	" A B C D E F G H I J ",
	" K L M N O P Q R S T ",
	" U V W X Y Z . - _   "
};
#endif

int TGameMain_Poll(TGameMain *class, int counter) {
  int  i, qvoi = rand() % 5 + 1;
  int  skip;
  TGameSprite  *sp;
  int is_hd_x = 0, is_hd_y = -21;
#ifdef TAILTALE_HD
  is_hd_x = 80, is_hd_y = 0; // for center some...
#endif

  if (class == NULL)
    return(FALSE);

  skip = TRUE;

  /* -------------------------------- */
  /* --- ゲームメインステップ */

  switch(class->step) {
    /* -- タイトル初期化 */
  case TitleInit:
    TGameScreen_LoadTexture(class->screen, 0, "graphic/system/logo.tga");
    TGameScreen_LoadTexturePure(class->screen, 1, "graphic/system/misc.tga");
    TGameScreen_LoadTexturePure(class->screen, 2, "graphic/system/rules.tga");
#ifdef TAILTALE_HD
    TGameScreen_LoadTexturePure(class->screen, 3, "graphic/system/title/title_480.tga");
    TGameScreen_LoadTexturePure(class->screen, 4, "graphic/background/menu_480.tga");
#else
    TGameScreen_LoadTexturePure(class->screen, 3, "graphic/system/title/title_320.tga");
    TGameScreen_LoadTexturePure(class->screen, 4, "graphic/background/menu_320.tga");
#endif
#ifdef TOUCH
    TGameScreen_LoadTexturePure(class->screen, 6, "graphic/system/touch.tga");
#endif

    class->bg = TGameScreen_GetSprite(class->screen, 0);
    class->logo = TGameScreen_GetSprite(class->screen, 1);
    class->splash1 = TGameScreen_GetSprite(class->screen, 2);
#ifdef TAILTALE_HD
	class->splash2 = TGameScreen_GetSprite(class->screen, 3);
#endif
	class->menu_bottom = TGameScreen_GetSprite(class->screen, 4);
	class->menu_top = TGameScreen_GetSprite(class->screen, 5);
	class->menu = TGameScreen_GetSprite(class->screen, 6);
    class->help = TGameScreen_GetSprite(class->screen, 7);
#ifdef OSK
    class->osk = TGameScreen_GetSprite(class->screen, 8);
    class->osk_select = TGameScreen_GetSprite(class->screen, 9);
#endif
    class->cursor = TGameScreen_GetSprite(class->screen, 10); // Must Last!!!
#ifdef TOUCH
    class->tarrow = TGameScreen_GetSprite(class->screen, 11);
    class->tbtn1 = TGameScreen_GetSprite(class->screen, 15);
    class->tbtn2 = TGameScreen_GetSprite(class->screen, 16);
    class->tbtn3 = TGameScreen_GetSprite(class->screen, 17); // A & B
    class->tbtn4 = TGameScreen_GetSprite(class->screen, 18); // 1 & 2
#endif
    class->lang = TGameScreen_GetSprite(class->screen, 19);

#ifdef TAILTALE_HD
    class->bg->DispSw = TRUE;
#else
    class->bg->DispSw = FALSE;
#endif
    class->bg->x = 0;
    class->bg->y = 0;
    class->bg->w = 480;
    class->bg->h = 272;
#ifdef TAILTALE_HD
    class->bg->tx = 0;
#else
    class->bg->tx = 80;
#endif
    class->bg->ty = 0;
    class->bg->Texture = TGameScreen_GetTexture(class->screen, 0);
    class->bg->alpha = 255;
    class->logo->DispSw = FALSE;
	class->logo->x = 0;
    class->logo->y = 0;
#ifdef TAILTALE_HD
    class->logo->w = 400;
    class->logo->h = 112;
#else
    class->logo->w = 300;
    class->logo->h = 84;
#endif
    class->logo->tx = 0;
    class->logo->ty = 0;
    class->logo->Texture = TGameScreen_GetTexture(class->screen, 3);
    class->logo->alpha = 255;
    class->lang->DispSw = TRUE;
    class->lang->x = SCREEN_WIDTH / 2 - 16;
    class->lang->y = SCREEN_HEIGHT - 32;
    class->lang->w = 32;
    class->lang->h = 20;
    class->lang->tx = 136 + (32 * sys_language);
    class->lang->ty = 272;
    class->lang->Texture = TGameScreen_GetTexture(class->screen, 0);
    class->lang->alpha = 255;
#ifdef TAILTALE_HD
    class->splash1->DispSw = TRUE;
	class->splash1->x = 0;
    class->splash1->y = 0;
    class->splash1->w = 301;
    class->splash1->h = 0;
    class->splash1->tx = 480;
    class->splash1->ty = 0;
    class->splash1->Texture = TGameScreen_GetTexture(class->screen, 0);
    class->splash1->alpha = 255;
    class->splash2->DispSw = TRUE;
	class->splash2->x = 232;
    class->splash2->y = 75;
    class->splash2->w = 0;
    class->splash2->h = 50;
    class->splash2->tx = 0;
    class->splash2->ty = 272;
    class->splash2->Texture = TGameScreen_GetTexture(class->screen, 0);
    class->splash2->alpha = 255;
#else
    class->splash1->DispSw = TRUE;
	class->splash1->x = 16 + ((320 / 2) - (269 / 2));
    class->splash1->y = ((240 / 2) - (72 / 2));
    class->splash1->w = 0;
    class->splash1->h = 72;
    class->splash1->tx = 71;
    class->splash1->ty = 0;
    class->splash1->Texture = TGameScreen_GetTexture(class->screen, 1);
    class->splash1->alpha = 255;
#endif
#ifdef OSK
    class->osk->DispSw = FALSE;
    class->osk->x = 66 + is_hd_x;
    class->osk->y = 124 + is_hd_y;
    class->osk->w = 187;
    class->osk->h = 91;
    class->osk->tx = 0;
    class->osk->ty = 181;
    class->osk->Texture = TGameScreen_GetTexture(class->screen, 1);
    class->osk->alpha = 255;
    class->osk_select->DispSw = FALSE; // move 14px from osk
    class->osk_select->x = 0;
    class->osk_select->y = 0;
    class->osk_select->w = 15;
    class->osk_select->h = 15;
    class->osk_select->tx = 187;
    class->osk_select->ty = 181;
    class->osk_select->Texture = TGameScreen_GetTexture(class->screen, 1);
    class->osk_select->alpha = 255;
#endif
    class->cursor->DispSw = FALSE;
    class->cursor->x = 0;
    class->cursor->y = 0;
    class->cursor->w = 24;
    class->cursor->h = 24;
    class->cursor->tx = 0;
    class->cursor->ty = 145;
    class->cursor->Texture = TGameScreen_GetTexture(class->screen, 1);
    class->cursor->alpha = 255;
    class->help->DispSw = FALSE;
    class->help->x = 0 + is_hd_x;
    class->help->y = 0 + is_hd_y;
    class->help->w = 320;
    class->help->h = 452;
    class->help->tx = 0;
    class->help->ty = 0;
    class->help->Texture = TGameScreen_GetTexture(class->screen, 2);
    class->help->alpha = 255;
    class->menu_bottom->DispSw = FALSE;
    class->menu_bottom->x = 0;
#ifdef TAILTALE_HD
    class->menu_bottom->y = 206;
    class->menu_bottom->w = 480;
#else
    class->menu_bottom->y = 174;
    class->menu_bottom->w = 320;
#endif
    class->menu_bottom->h = 66;
    class->menu_bottom->tx = 0;
    class->menu_bottom->ty = 76;
    class->menu_bottom->Texture = TGameScreen_GetTexture(class->screen, 4);
    class->menu_bottom->alpha = 255;
    class->menu_top->DispSw = FALSE;
    class->menu_top->x = 0;
    class->menu_top->y = 0;
#ifdef TAILTALE_HD
    class->menu_top->w = 480;
#else
    class->menu_top->w = 320;
#endif
    class->menu_top->h = 76;
    class->menu_top->tx = 0;
    class->menu_top->ty = 0;
    class->menu_top->Texture = TGameScreen_GetTexture(class->screen, 4);
    class->menu_top->alpha = 255;
    class->menu->DispSw = FALSE;
#ifdef TAILTALE_HD
    class->menu->x = 90;
    class->menu->y = 42;
#else
    class->menu->x = 10;
    class->menu->y = 26;
#endif
    class->menu->w = 300;
    class->menu->h = 188;
    class->menu->tx = 0;
    class->menu->ty = 142;
    class->menu->Texture = TGameScreen_GetTexture(class->screen, 4);
    class->menu->alpha = 255;
#ifdef TOUCH
    class->tarrow->DispSw = TRUE; // Arrow
    // class->tarrow->x = 32;
    // class->tarrow->y = SCREEN_HEIGHT - 96;
    class->tarrow->w = 64;
    class->tarrow->h = 64;
    class->tarrow->tx = 0;
    class->tarrow->ty = 0;
    class->tarrow->Texture = TGameScreen_GetTexture(class->screen, 6);
    class->tarrow->alpha = 255;
    class->tbtn1->DispSw = TRUE; // + & -
    class->tbtn1->x = SCREEN_WIDTH / 2 - 72;
    class->tbtn1->y = SCREEN_HEIGHT - 80;
    class->tbtn1->w = 64;
    class->tbtn1->h = 64;
    class->tbtn1->tx = 256;
    class->tbtn1->ty = 0;
    class->tbtn1->Texture = TGameScreen_GetTexture(class->screen, 6);
    class->tbtn1->alpha = 255;
    class->tbtn2->DispSw = TRUE;
    class->tbtn2->x = SCREEN_WIDTH / 2 + 8;
    class->tbtn2->y = SCREEN_HEIGHT - 80;
    class->tbtn2->w = 64;
    class->tbtn2->h = 64;
    class->tbtn2->tx = 320;
    class->tbtn2->ty = 0;
    class->tbtn2->Texture = TGameScreen_GetTexture(class->screen, 6);
    class->tbtn2->alpha = 255;
	class->tbtn3->DispSw = TRUE; // A & B
    class->tbtn3->x = SCREEN_WIDTH - 136;
    class->tbtn3->y = SCREEN_HEIGHT - 136;
    class->tbtn3->w = 128;
    class->tbtn3->h = 64;
    class->tbtn3->tx = 384;
    class->tbtn3->ty = 0;
    class->tbtn3->Texture = TGameScreen_GetTexture(class->screen, 6);
    class->tbtn3->alpha = 255;
	class->tbtn4->DispSw = TRUE; // 1 & 2
    class->tbtn4->x = SCREEN_WIDTH - 136;
    class->tbtn4->y = SCREEN_HEIGHT - 72;
    class->tbtn4->w = 128;
    class->tbtn4->h = 64;
    class->tbtn4->tx = 512;
    class->tbtn4->ty = 0;
    class->tbtn4->Texture = TGameScreen_GetTexture(class->screen, 6);
    class->tbtn4->alpha = 255;
#endif

    class->titletimer = 0;
    cursorsetting = 0;
    skip = FALSE;
    class->step = TitleHandle;
    break;

  case TitleHandle:
	if (class->init == 1) {
      class->bg->DispSw = TRUE;
	  TGameScreen_LoadTexture(class->screen, 0, "graphic/background/title.tga");
      class->splash1->DispSw = FALSE;
#ifdef TAILTALE_HD
      class->splash2->DispSw = FALSE;
#endif
      class->lang->DispSw = FALSE;
	  class->step = TitleIn;
	} else {
	  TGameScreen_LoadTexture(class->screen, 0, "graphic/system/logo.tga");
	  class->step = TitleSplash;
	}
    class->bg->Texture = TGameScreen_GetTexture(class->screen, 0);
    break;

  case TitleSplash:
    i = InputJoyKeyTriger(0);

#ifdef TAILTALE_HD
    if (class->splash1->h < 272)
		class->splash1->h += 4;
#endif

    if (class->titletimer < 134) {
#ifdef TAILTALE_HD
        if (class->splash2->w < 136)
          class->splash2->w += 2;
#else
        if (class->splash1->w < 270)
          class->splash1->w += 2;
#endif
	}

    class->titletimer = class->titletimer + 1;
#ifdef X86
	SDL_Delay (10);
#else
	SDL_Delay (5);
#endif
    if (class->titletimer == 30)
      SoundSE(13);
    if ((class->titletimer == 300) || ((i & (IN_Button1)) != 0)) {
      class->titletimer = 0;
      class->step = TitleHandle;
      class->init = 1;
	}
    break;

    /* -- タイトルロゴフレームイン */
  case TitleIn:
    /* - SE */
    if (class->titletimer == 15*2)
	  SoundSE(1);

    if (class->titletimer > 20*2) {
      /* はずむ */
#ifdef TAILTALE_HD
      class->logo->x = 38;
#else
      class->logo->x = 10;
#endif
      class->logo->y = 160 - (int)(sin(3.1415926 * 2.0 * (((float)class->titletimer - 40.0) / 160.0)) * 160.0);
      class->logo->DispSw = TRUE;
    } else {
      /* 上から降ってくる */
#ifdef TAILTALE_HD
      class->logo->x = 38;
#else
      class->logo->x = 10;
#endif
      class->logo->y = 160 - ((40 - class->titletimer) * 10);
      class->logo->DispSw = TRUE;
    }
    /* -- 次へ進むステップ */
    class->titletimer = class->titletimer + 1;
    if (class->titletimer == 38*2)
      SoundSE(9);

    if (class->titletimer == 45*2) {
      class->step = TitleMain;
	  SoundMusic (2);
	}
    break;

    /* -- スタートボタン待ち */
  case TitleMain:
    class->logo->DispSw = TRUE;
#ifdef TAILTALE_HD
    class->logo->x = 38;
#else
    class->logo->x = 10;
#endif
    class->logo->y = 10;
    class->titletimer = class->titletimer + 1;
    disptitle = 1;

    i = InputJoyKeyTriger(0);

    if ((i & (IN_Button1)) != 0) {
      class->titletimer = 0;
      SoundSE(6);
      class->step = MenuInit;
	}
    break;

    /* -- スタート時アトラクト */
  case TitleStart:
    class->titletimer = class->titletimer + 1;
    if (class->titletimer == 20) {
	  SoundSEStop ();
      SoundMusicStop();
      class->step = TitleFree;
    }
    break;

    /* -- タイトルロゴ後しまつ */
  case TitleFree:
    srand(counter);
    dispmenu = 0, cursorsetting = 0;
    class->bg->DispSw = FALSE;
    class->logo->DispSw = FALSE;
    class->cursor->DispSw = FALSE;
    class->help->DispSw = FALSE;
#ifdef OSK
    class->osk->DispSw = FALSE;
	class->osk_select->DispSw = FALSE;
#endif
    class->help->DispSw = FALSE;
    class->menu_top->DispSw = FALSE;
    class->menu_bottom->DispSw = FALSE;
    class->menu->DispSw = FALSE;
#ifdef TOUCH
    class->tarrow->DispSw = FALSE;
    class->tbtn1->DispSw = FALSE;
    class->tbtn2->DispSw = FALSE;
    class->tbtn3->DispSw = FALSE;
    class->tbtn4->DispSw = FALSE;
#endif
    class->step = PuzzleInit;
    break;

    /* -- ゲーム初期化 */
  case PuzzleInit:
    class->puzzle = TPuzzleTrial_Create(class->screen, class->level);
    class->step = PuzzleMain;
    skip = FALSE;
    break;

    /* -- ゲームメイン */
  case PuzzleMain:
    TPuzzleTrial_GameMain(class->puzzle);
    if (TPuzzleTrial_GameStat(class->puzzle) == FALSE) {
      class->step = PuzzleFree;
    }
    break;

    /* -- ゲーム終了後しまつ */
    case PuzzleFree:
	  // TPuzzleTrial_Destroy(class->puzzle);
	  free (class->puzzle);
      for( i=0; i<SPRITEMAX; i++) {
	    sp = TGameScreen_GetSprite(class->screen, i);
	    sp->DispSw = FALSE;
      }
      class->step = TitleInit;
      break;

  case MenuInit:
    class->bg = TGameScreen_GetSprite(class->screen, 0);
    class->bg->DispSw = TRUE;
    class->bg->Texture = TGameScreen_GetTexture(class->screen, 0);
    class->logo->DispSw = FALSE;
    class->help->DispSw = FALSE;
    class->cursor->DispSw = FALSE;
    class->help->DispSw = FALSE;
    class->menu_top->DispSw = TRUE;
    class->menu_bottom->DispSw = TRUE;
    class->menu->DispSw = TRUE;
	disptitle = 0, dispmenu = 1;
	class->step = MenuMain;
    break;

  case MenuMain:
    class->cursor->DispSw = TRUE;
    class->cursor->x = 7 + is_hd_x;
    class->cursor->y = 154 + (12 * cursorsetting) + 4 + is_hd_y;

    i = InputJoyKeyTriger(0);

	if ((i & (IN_Button1)) != 0) {
      SoundSE(10);
      class->titletimer = 0;
	  switch (cursorsetting) {
        case 0:
		  SoundSE (14);
#ifdef NETWORK
		  TNetwork_Close ();
#endif
          class->step = TitleStart;
		  break;
#ifdef NETWORK
        case 1:
          class->step = NetworkInit;
		  break;
        case 2:
          class->step = ConfigInit;
		  break;
        case 3:
          class->step = HelpInit;
		  break;
        case 4:
		  class->step = QuitInit;
		  break;
#else
        case 1:
          class->step = ConfigInit;
		  break;
        case 2:
          class->step = HelpInit;
		  break;
        case 3:
		  class->step = QuitInit;
		  break;
#endif
	  }
    }

    if ((i & IN_Up) != 0) {
      SoundSE(12);
      cursorsetting--;

      if (cursorsetting < 0)
#ifdef NETWORK
        cursorsetting = 4;
#else
        cursorsetting = 3;
#endif
    }
    if ((i & IN_Down) != 0) {
      SoundSE(12);
      cursorsetting++;

#ifdef NETWORK
      if (cursorsetting > 4)
#else
      if (cursorsetting > 3)
#endif
        cursorsetting = 0;
    }
    break;

  case MenuFree:
    dispmenu = 0, cursorsetting = 0;
    class->step = TitleInit;
    break;

#ifdef NETWORK
  case NetworkInit:
    class->bg->DispSw = TRUE;
    class->logo->DispSw = FALSE;
    class->help->DispSw = FALSE;
    class->cursor->DispSw = TRUE;
    class->help->DispSw = FALSE;
    dispmenu = 0, dispnetwork = 1, cursorsetting = 0;
    TNetwork_Init ();
	SoundSE (15);
    class->step = NetworkMain;
    break;

  case NetworkMain:
      if ((dispnetwork == 1) || (dispnetwork == 2) || (dispnetwork == 4)) {
        class->cursor->DispSw = TRUE;
        class->cursor->x = 7 + is_hd_x;
		if (dispnetwork == 4)
			class->cursor->y = 74 + (12 * cursorsetting) + 4 + is_hd_y;
		else
			class->cursor->y = 62 + (12 * cursorsetting) + 4 + is_hd_y;
	  } else {
        class->cursor->DispSw = FALSE;
#ifdef OSK
#ifdef TAILTALE_HD
		class->osk_select->x = 80 + is_hd_x + (16 * osk_pos_x);
#else
		class->osk_select->x = 80 + (16 * osk_pos_x);
#endif
		class->osk_select->y = 138 + (16 * osk_pos_y) + is_hd_y;
#endif
	  }

      i = InputJoyKeyTriger(0);

      if ((i & IN_Up) != 0) {
        SoundSE(12);
        if (dispnetwork == 1) {
          cursorsetting--;

          if (cursorsetting < 0)
            cursorsetting = 3;
		} else if (dispnetwork == 2) {
          cursorsetting--;

          if (cursorsetting < 0)
            cursorsetting = 3;
		} else if ((dispnetwork == 3) || (dispnetwork == 5)) {
#ifdef OSK
          osk_pos_y--;

          if (osk_pos_y < 0)
            osk_pos_y = 0;
#endif
		} else if (dispnetwork == 4) {
          cursorsetting--;

          if (cursorsetting < 0)
            cursorsetting = 9;
		}
      }
      if ((i & IN_Down) != 0) {
        SoundSE(12);
        if (dispnetwork == 1) {
          cursorsetting++;

          if (cursorsetting > 3)
            cursorsetting = 0;
		} else if (dispnetwork == 2) {
          cursorsetting++;

          if (cursorsetting > 3)
            cursorsetting = 0;
		} else if ((dispnetwork == 3) || (dispnetwork == 5)) {
#ifdef OSK
          osk_pos_y++;

          if (osk_pos_y > MAX_Y)
            osk_pos_y = MAX_Y;
#endif
		} else if (dispnetwork == 4) {
          cursorsetting++;

          if (cursorsetting > 9)
            cursorsetting = 0;
		}
      }
      if ((i & IN_Left) != 0) {
        SoundSE(12);
        if (dispnetwork == 2) {
          if (cursorsetting == 0) {
            sys_level--;

            if (sys_level < 0)
              sys_level = 0;
		  }
          if (cursorsetting == 1) {
            net_score--;

            if (net_score < 0)
              net_score = 0;
		  }
          if (cursorsetting == 2) {
            net_block--;

            if (net_block < 0)
              net_block = 0;
		  }
          if (cursorsetting == 3) {
            net_evomode--;

            if (net_evomode < 0)
              net_evomode = 0;
		  }
		} else if ((dispnetwork == 3) || (dispnetwork == 5)) {
#ifdef OSK
          osk_pos_x--;

          if (osk_pos_x < 0)
            osk_pos_x = 0;
#endif
		}
      }
      if ((i & IN_Right) != 0) {
        SoundSE(12);
        if (dispnetwork == 2) {
          if (cursorsetting == 0) {
            sys_level++;

            if (sys_level > 3)
              sys_level = 3;
		  }
          if (cursorsetting == 1) {
            net_score++;

            if (net_score > 8)
              net_score = 8;
		  }
          if (cursorsetting == 2) {
            net_block++;

            if (net_block > 8)
              net_block = 8;
		  }
          if (cursorsetting == 3) {
            net_evomode++;

            if (net_evomode > 1)
              net_evomode = 1;
		  }
		} else if ((dispnetwork == 3) || (dispnetwork == 5)) {
#ifdef OSK
          osk_pos_x++;

          if (osk_pos_x > MAX_X)
            osk_pos_x = MAX_X;
#endif
		}
      }
      if ((i & (IN_Button1)) != 0) {
        SoundSE(10);
        if (dispnetwork == 1) {
          if (cursorsetting == 0)
		    cursorsetting = 0, dispnetwork = 2;
          if (cursorsetting == 1) {
#ifdef OSK
            class->osk->DispSw = TRUE;
			class->osk_select->DispSw = TRUE;
#endif
            class->cursor->DispSw = FALSE;
            Font_GetOSK ("127.0.0.1", "127.0.0.1", 15);
            cursorsetting = 0, dispnetwork = 3;
          }
          if (cursorsetting == 2) {
			cursorsetting = 0, dispnetwork = 4;
#if defined(ANDROID) || defined(WINPHONE) || defined(X86) /* Other Device dont support it or need a Fix! */
		    TNetwork_ClientList ();
#endif
          }
          if (cursorsetting == 3) {
#ifdef OSK
            class->osk->DispSw = TRUE;
			class->osk_select->DispSw = TRUE;
#endif
            class->cursor->DispSw = FALSE;
            Font_GetOSK (net_name, net_name, 12);
            cursorsetting = 0, dispnetwork = 5;
          }
		} else if (dispnetwork == 2) {
          if (strlen (osk_buffer) > 0)
            osk_buffer[strlen (osk_buffer) - 1] = '\0';
		  TNetwork_Server ();
		  dispnetwork = 0, cursorsetting = 0;
          class->bg->DispSw = TRUE;
          class->logo->DispSw = FALSE;
          class->cursor->DispSw = FALSE;
#ifdef OSK
          class->osk->DispSw = FALSE;
		  class->osk_select->DispSw = FALSE;
#endif
          class->titletimer = 0;
          class->step = TitleStart;
		  network_game_win_score = 10000 + (net_score * 5000);
		  network_game_win_block = 120 + (net_block * 80);
		  evolution = net_evomode; // Evolution Mode
		  Config_Save ();
		} else if ((dispnetwork == 3) || (dispnetwork == 5)) {
          // strncpy (osk_out_buff, osk_buffer, max_len);
          if (dispnetwork == 3) {
            TNetwork_Connect (osk_buffer, "quit");
            dispnetwork = 0, cursorsetting = 0;
            class->bg->DispSw = TRUE;
            class->logo->DispSw = FALSE;
            class->cursor->DispSw = FALSE;
            class->help->DispSw = FALSE;
#ifdef OSK
            class->osk->DispSw = FALSE;
		    class->osk_select->DispSw = FALSE;
#endif
            class->titletimer = 0;
			TNetwork_UpdateDisp ();
            class->step = TitleStart;
		  }
		} else if (dispnetwork == 4) {
		  TNetwork_Connect (slist[cursorsetting], "quit");
          dispnetwork = 0, cursorsetting = 0;
          class->bg->DispSw = TRUE;
          class->logo->DispSw = FALSE;
          class->cursor->DispSw = FALSE;
          class->help->DispSw = FALSE;
#ifdef OSK
          class->osk->DispSw = FALSE;
		  class->osk_select->DispSw = FALSE;
#endif
          class->titletimer = 0;
		  TNetwork_UpdateDisp ();
          class->step = TitleStart;
		}
	  }
      if ((i & (IN_Button2)) != 0) {
        SoundSE(11);
        if ((dispnetwork == 2) || (dispnetwork == 3) || (dispnetwork == 4) || (dispnetwork == 5)) {
#ifdef OSK
		  class->osk->DispSw = FALSE;
		  class->osk_select->DispSw = FALSE;
#endif
		  dispnetwork = 1, cursorsetting = 0;
          if ((dispnetwork == 2) || (dispnetwork == 5))
            Config_Save ();
	    } else
		  class->step = NetworkFree;
	  }
#ifdef OSK
      if ((i & (IN_Button3)) != 0) {
		 if ((dispnetwork == 3) || (dispnetwork == 5)) { // Add Character
          if (max_len > strlen (osk_buffer)) {
            char *selected_line, selected_char[2];
            if (osk_text_shift == 1)
              selected_line = osk_text_shift_key[osk_pos_y];
			else
              selected_line = osk_text_key[osk_pos_y];

            selected_char[0] = selected_line[1 + (2 * osk_pos_x)];

            if (selected_char[0] == '\t')
              selected_char[0] = ' ';

            selected_char[1] = '\0';
            strcat (osk_buffer, selected_char);
          }
		}
	  }
      if ((i & (IN_Button4)) != 0) {
        SoundSE(6);
        if (dispnetwork == 1)
          class->step = NetworkFree;
        else if ((dispnetwork == 3) || (dispnetwork == 5)) { // Remove Character
          if (strlen (osk_buffer) > 0)
            osk_buffer[strlen (osk_buffer) - 1] = '\0';
		}
	  }
      if ((i & (IN_Button5)) != 0) {
        if ((dispnetwork == 3) || (dispnetwork == 5)) { // Change OSK
          if (osk_text_shift == 1)
		    osk_text_shift = 0;
		  else
		    osk_text_shift = 1;
		}
	  }
#endif
    break;

  case NetworkFree:
    dispnetwork = 0, cursorsetting = 0;
    class->step = MenuInit;
    break;
#endif

    /* -- GP2X キーコンフィグ */
  case ConfigInit:
    class->bg->DispSw = TRUE;
    class->logo->DispSw = FALSE;
    class->help->DispSw = FALSE;
    class->cursor->DispSw = FALSE;
#ifdef OSK
    class->osk->DispSw = FALSE;
	class->osk_select->DispSw = FALSE;
#endif
    class->help->DispSw = FALSE;
    dispmenu = 0, dispsetting = 1, cursorsetting = 0;
	SoundSE (16);
    class->step = ConfigMain;
    break;

    /* -- GP2X キーコンフィグ選択中 */
  case ConfigMain:
	  class->cursor->DispSw = TRUE;
      class->cursor->x = 7 + is_hd_x;
      class->cursor->y = 60 + (12 * cursorsetting) + 4 + is_hd_y;

      i = InputJoyKeyTriger(0);

      if ((i & IN_Up) != 0) {
        SoundSE(12);
        cursorsetting--;

        if (cursorsetting < 0)
          cursorsetting = 10;
      }
      if ((i & IN_Left) != 0) {
        SoundSE(12);
        if (cursorsetting == 0) {
          sys_level--;

          if (sys_level < 0)
            sys_level = 0;
		}
        if (cursorsetting == 2) {
          sys_language--;

          if (sys_language < 0)
            sys_language = 0;
		}
        if (cursorsetting == 4) {
#ifdef X86
          sys_screensize--;

          if (sys_screensize < 0)
            sys_screensize = 0;
#endif
		}
        if (cursorsetting == 5) {
          sys_frameskip--;

          if (sys_frameskip < 0)
            sys_frameskip = 0;
		}
        if (cursorsetting == 9) {
          sys_bgm_volume--;

          if (sys_bgm_volume < 0)
            sys_bgm_volume = 0;
		}
		if (cursorsetting == 10) {
          sys_se_volume--;

          if (sys_se_volume < 0)
            sys_se_volume = 0;
		}
      }
      if ((i & IN_Down) != 0) {
        SoundSE(12);
        cursorsetting++;

        if (cursorsetting > 10)
          cursorsetting = 0;
      }
      if ((i & IN_Right) != 0) {
        SoundSE(12);
        if (cursorsetting == 0) {
          sys_level++;

          if (sys_level > 3)
            sys_level = 3;
		}
        if (cursorsetting == 2) {
          sys_language++;

          if (sys_language > 8)
            sys_language = 8;
		}
        if (cursorsetting == 4) {
#ifdef X86
          sys_screensize++;

#ifndef TAILTALE_HD
          if (sys_screensize > 10)
            sys_screensize = 10;
#else
          if (sys_screensize > 8)
            sys_screensize = 8;
#endif
#endif
		}
        if (cursorsetting == 5) {
          sys_frameskip++;

          if (sys_frameskip > 9)
            sys_frameskip = 9;
		}
        if (cursorsetting == 9) {
          sys_bgm_volume++;

          if (sys_bgm_volume > 100)
            sys_bgm_volume = 100;
		}
		if (cursorsetting == 10) {
          sys_se_volume++;

          if (sys_bgm_volume > 100)
            sys_se_volume = 100;
		}
      }

      if ((i & IN_Button1) != 0) {
        if (cursorsetting == 1)
          sys_debug = ((sys_debug) ? 0 : 1);
        if (cursorsetting == 3)
          sys_fullscreen = ((sys_fullscreen) ? 0 : 1);
        if (cursorsetting == 6)
          sys_fps = ((sys_fps) ? 0 : 1);
        if (cursorsetting == 7)
          sys_joystick_swap = ((sys_joystick_swap) ? 0 : 1);
        if (cursorsetting == 8) {
          sys_sound = ((sys_sound) ? 0 : 1);
		}
	  }

      if ((i & (IN_Button2)) != 0) {
        SoundSE(11);
#ifdef X86
#ifndef VS_WIN
		char screen_set[16][128];
		explode (screen_set, screensize[sys_screensize], 'x');
		TGameScreen_ChangeScreenSize (class->screen, atoi (screen_set[0]), atoi (screen_set[1]));
#endif
#endif

        if (sys_sound == 1)
		  SoundMusic (2);
		else
		  SoundMusicStop ();

        class->step = ConfigFree;
      }
    break;

    /* -- GP2X キーコンフィグ終了 */
  case ConfigFree:
    Config_Save ();
    dispsetting = 0, cursorsetting = 0;
    SoundVolume (); // Update Sound volume...
	Language ();
    class->step = MenuInit;
    break;

  case HelpInit:
    class->bg->DispSw = TRUE;
    class->logo->DispSw = FALSE;
    class->cursor->DispSw = FALSE;
#ifdef OSK
    class->osk->DispSw = FALSE;
	class->osk_select->DispSw = FALSE;
#endif
    class->help->DispSw = FALSE;
    dispmenu = 0, cursorsetting = 0;
	SoundSE (17);
    class->step = HelpMain;
    break;

  case HelpMain:
    class->help->DispSw = TRUE;

    i = InputJoyKeyTriger(0);

	if ((i & (IN_Button2)) != 0) {
      SoundSE(11);
      class->step = HelpFree;
    }

    if ((i & IN_Up) != 0) {
      SoundSE(12);
      class->help->y += 30;

      if (class->help->y > 0)
        class->help->y = 0;
    }
    if ((i & IN_Down) != 0) {
      SoundSE(12);
      class->help->y -= 30;

      if (class->help->y < -180)
        class->help->y = -180;
    }
    break;

  case HelpFree:
    cursorsetting = 0;
    class->step = MenuInit;
    break;

  case QuitInit:
    class->bg->DispSw = TRUE;
    class->logo->DispSw = FALSE;
    class->cursor->DispSw = FALSE;
#ifdef OSK
    class->osk->DispSw = FALSE;
	class->osk_select->DispSw = FALSE;
#endif
    class->help->DispSw = FALSE;
    class->menu->DispSw = FALSE;
    dispmenu = 0, dispquit = 1, cursorsetting = 0;
    class->titletimer = 0;
	SoundSE (18);
    class->step = QuitMain;
    break;

  case QuitMain:
    class->titletimer = class->titletimer + 1;
	if (class->titletimer == 60)
      cursorsetting++;
	else if (class->titletimer == 120)
      cursorsetting++;
	else if (class->titletimer == 150) {
	  if ((qvoi == 2) || (qvoi == 4)) {
	    SoundSE (21);
	  } else {
	    SoundSE (20);
	  }
	} else if (class->titletimer == 225)
      AppExit = 1;

    i = InputJoyKeyTriger(0);
	if ((i & (IN_Button1)) != 0) {
      AppExit = 1;
    }
	if ((i & (IN_Button2)) != 0) {
      SoundSE(11);
      class->step = QuitFree;
    }
    break;

  case QuitFree:
    dispquit = 0, cursorsetting = 0;
    class->step = MenuInit;
    break;
  } // switch

#ifdef TOUCH
  class->tarrow->x = 32 + touch_x;
  class->tarrow->y = SCREEN_HEIGHT - 96 + touch_y;
#endif

  return(skip);
}

