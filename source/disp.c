/* ---------------------------------------------------------- */
/*  disp.c                                                */
/* ---------------------------------------------------------- */

/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintaner  Rerorero@fumi.   */
/*                                                        */
/*   disp.c                                           */
/*     パズル画面表示管理                                  */
/*                                                        */
/*--------------------------------------------------------*/

/*------------------------------------------------------------- */
/** @file
    @brief		パズル画面表示管理
    @author		K.Kunikane (rerofumi)
    @since		Sep.24.2005
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

void  DispCursor(TPuzzleDisp *class);
void  DispBlock(TPuzzleDisp *class);
void  DispEraseBlock(TPuzzleDisp *class,
		     TGameSprite *obj,
		     Block *bl);
void  DispBack(TPuzzleDisp *class);
void  DispChara(TPuzzleDisp *class);
void  DispNext(TPuzzleDisp *class);
void  DispScore(TPuzzleDisp *class);
void  DispNum(TPuzzleDisp *class,
	      int x, int y,
	      int n, int o,
	      int num);
void  DispKira(TPuzzleDisp *class);

#ifdef NETWORK
void DispNet(TPuzzleDisp *class);
#endif

#ifdef TOUCH
void DispTouch (TPuzzleDisp *class);
#endif

/*-------------------------------*/
/* data table                    */
/*-------------------------------*/

int  animekoma[8] = {
  0, 0, 0, 0, 0, 1, 2, 1
};

const int disp_sin[360] = {
  0x8000,0x823b,0x8477,0x86b2,0x88ed,0x8b27,0x8d61,0x8f99,
  0x91d0,0x9406,0x963a,0x986c,0x9a9c,0x9ccb,0x9ef7,0xa120,
  0xa348,0xa56c,0xa78d,0xa9ac,0xabc7,0xaddf,0xaff3,0xb203,
  0xb40f,0xb618,0xb81c,0xba1c,0xbc17,0xbe0e,0xbfff,0xc1ec,
  0xc3d4,0xc5b6,0xc793,0xc96a,0xcb3c,0xcd08,0xcecd,0xd08d,
  0xd246,0xd3f9,0xd5a6,0xd74b,0xd8ea,0xda82,0xdc13,0xdd9c,
  0xdf1f,0xe09a,0xe20d,0xe379,0xe4dd,0xe639,0xe78d,0xe8d9,
  0xea1d,0xeb59,0xec8c,0xedb7,0xeed9,0xeff3,0xf104,0xf20c,
  0xf30b,0xf401,0xf4ef,0xf5d3,0xf6ad,0xf77f,0xf847,0xf906,
  0xf9bc,0xfa68,0xfb0a,0xfba3,0xfc32,0xfcb8,0xfd33,0xfda5,
  0xfe0e,0xfe6c,0xfec1,0xff0b,0xff4c,0xff83,0xffb0,0xffd3,
  0xffec,0xfffb,0xffff,0xfffb,0xffec,0xffd3,0xffb0,0xff83,
  0xff4c,0xff0b,0xfec1,0xfe6c,0xfe0e,0xfda5,0xfd33,0xfcb8,
  0xfc32,0xfba3,0xfb0a,0xfa68,0xf9bc,0xf906,0xf847,0xf77f,
  0xf6ad,0xf5d3,0xf4ef,0xf401,0xf30b,0xf20c,0xf104,0xeff3,
  0xeed9,0xedb7,0xec8c,0xeb59,0xea1d,0xe8d9,0xe78d,0xe639,
  0xe4dd,0xe379,0xe20d,0xe09a,0xdf1f,0xdd9c,0xdc13,0xda82,
  0xd8ea,0xd74b,0xd5a6,0xd3f9,0xd246,0xd08d,0xcecd,0xcd08,
  0xcb3c,0xc96a,0xc793,0xc5b6,0xc3d4,0xc1ec,0xc000,0xbe0e,
  0xbc17,0xba1c,0xb81c,0xb618,0xb40f,0xb203,0xaff3,0xaddf,
  0xabc7,0xa9ac,0xa78d,0xa56c,0xa348,0xa120,0x9ef7,0x9ccb,
  0x9a9c,0x986c,0x963a,0x9406,0x91d0,0x8f99,0x8d61,0x8b27,
  0x88ed,0x86b2,0x8477,0x823b,0x8000,0x7dc5,0x7b89,0x794e,
  0x7713,0x74d9,0x729f,0x7067,0x6e30,0x6bfa,0x69c6,0x6794,
  0x6564,0x6335,0x6109,0x5ee0,0x5cb8,0x5a94,0x5873,0x5654,
  0x5439,0x5222,0x500d,0x4dfd,0x4bf1,0x49e8,0x47e4,0x45e4,
  0x43e9,0x41f2,0x4001,0x3e14,0x3c2c,0x3a4a,0x386d,0x3696,
  0x34c4,0x32f8,0x3133,0x2f73,0x2dba,0x2c07,0x2a5a,0x28b5,
  0x2716,0x257e,0x23ed,0x2264,0x20e1,0x1f66,0x1df3,0x1c87,
  0x1b23,0x19c7,0x1873,0x1727,0x15e3,0x14a7,0x1374,0x1249,
  0x1127,0x100d,0x0efc,0x0df4,0x0cf5,0x0bff,0x0b11,0x0a2d,
  0x0953,0x0881,0x07b9,0x06fa,0x0644,0x0598,0x04f6,0x045d,
  0x03ce,0x0348,0x02cd,0x025b,0x01f2,0x0194,0x013f,0x00f5,
  0x00b4,0x007d,0x0050,0x002d,0x0014,0x0005,0x0001,0x0005,
  0x0014,0x002d,0x0050,0x007d,0x00b4,0x00f5,0x013f,0x0194,
  0x01f2,0x025b,0x02cd,0x0348,0x03ce,0x045d,0x04f6,0x0598,
  0x0644,0x06fa,0x07b9,0x0881,0x0953,0x0a2d,0x0b11,0x0bff,
  0x0cf5,0x0df4,0x0efc,0x100d,0x1127,0x1249,0x1374,0x14a7,
  0x15e3,0x1727,0x1873,0x19c7,0x1b23,0x1c87,0x1df3,0x1f66,
  0x20e1,0x2263,0x23ed,0x257e,0x2716,0x28b5,0x2a5a,0x2c07,
  0x2dba,0x2f73,0x3133,0x32f8,0x34c4,0x3696,0x386d,0x3a4a,
  0x3c2c,0x3e14,0x4000,0x41f2,0x43e9,0x45e4,0x47e4,0x49e8,
  0x4bf1,0x4dfd,0x500d,0x5221,0x5439,0x5654,0x5873,0x5a94,
  0x5cb8,0x5ee0,0x6109,0x6335,0x6564,0x6794,0x69c6,0x6bfa,
  0x6e30,0x7067,0x729f,0x74d9,0x7713,0x794e,0x7b89,0x7dc5,
};

const int disp_cos[360] = {
  0x0000,0xfffb,0xffec,0xffd3,0xffb0,0xff83,0xff4c,0xff0b,
  0xfec1,0xfe6c,0xfe0e,0xfda5,0xfd33,0xfcb8,0xfc32,0xfba3,
  0xfb0a,0xfa68,0xf9bc,0xf906,0xf847,0xf77f,0xf6ad,0xf5d3,
  0xf4ef,0xf401,0xf30b,0xf20c,0xf104,0xeff3,0xeed9,0xedb7,
  0xec8c,0xeb59,0xea1d,0xe8d9,0xe78d,0xe639,0xe4dd,0xe379,
  0xe20d,0xe09a,0xdf1f,0xdd9c,0xdc13,0xda82,0xd8ea,0xd74b,
  0xd5a6,0xd3f9,0xd246,0xd08d,0xcecd,0xcd08,0xcb3c,0xc96a,
  0xc793,0xc5b6,0xc3d4,0xc1ec,0xc000,0xbe0e,0xbc17,0xba1c,
  0xb81c,0xb618,0xb40f,0xb203,0xaff3,0xaddf,0xabc7,0xa9ac,
  0xa78d,0xa56c,0xa348,0xa120,0x9ef7,0x9ccb,0x9a9c,0x986c,
  0x963a,0x9406,0x91d0,0x8f99,0x8d61,0x8b27,0x88ed,0x86b2,
  0x8477,0x823b,0x8000,0x7dc5,0x7b89,0x794e,0x7713,0x74d9,
  0x729f,0x7067,0x6e30,0x6bfa,0x69c6,0x6794,0x6564,0x6335,
  0x6109,0x5ee0,0x5cb8,0x5a94,0x5873,0x5654,0x5439,0x5222,
  0x500d,0x4dfd,0x4bf1,0x49e8,0x47e4,0x45e4,0x43e9,0x41f2,
  0x4001,0x3e14,0x3c2c,0x3a4a,0x386d,0x3696,0x34c4,0x32f8,
  0x3133,0x2f73,0x2dba,0x2c07,0x2a5a,0x28b5,0x2716,0x257e,
  0x23ed,0x2264,0x20e1,0x1f66,0x1df3,0x1c87,0x1b23,0x19c7,
  0x1873,0x1727,0x15e3,0x14a7,0x1374,0x1249,0x1127,0x100d,
  0x0efc,0x0df4,0x0cf5,0x0bff,0x0b11,0x0a2d,0x0953,0x0881,
  0x07b9,0x06fa,0x0644,0x0598,0x04f6,0x045d,0x03ce,0x0348,
  0x02cd,0x025b,0x01f2,0x0194,0x013f,0x00f5,0x00b4,0x007d,
  0x0050,0x002d,0x0014,0x0005,0x0001,0x0005,0x0014,0x002d,
  0x0050,0x007d,0x00b4,0x00f5,0x013f,0x0194,0x01f2,0x025b,
  0x02cd,0x0348,0x03ce,0x045d,0x04f6,0x0598,0x0644,0x06fa,
  0x07b9,0x0881,0x0953,0x0a2d,0x0b11,0x0bff,0x0cf5,0x0df4,
  0x0efc,0x100d,0x1127,0x1249,0x1374,0x14a7,0x15e3,0x1727,
  0x1873,0x19c7,0x1b23,0x1c87,0x1df3,0x1f66,0x20e1,0x2264,
  0x23ed,0x257e,0x2716,0x28b5,0x2a5a,0x2c07,0x2dba,0x2f73,
  0x3133,0x32f8,0x34c4,0x3696,0x386d,0x3a4a,0x3c2c,0x3e14,
  0x4000,0x41f2,0x43e9,0x45e4,0x47e4,0x49e8,0x4bf1,0x4dfd,
  0x500d,0x5221,0x5439,0x5654,0x5873,0x5a94,0x5cb8,0x5ee0,
  0x6109,0x6335,0x6564,0x6794,0x69c6,0x6bfa,0x6e30,0x7067,
  0x729f,0x74d9,0x7713,0x794e,0x7b89,0x7dc5,0x8000,0x823b,
  0x8477,0x86b2,0x88ed,0x8b27,0x8d61,0x8f99,0x91d0,0x9406,
  0x963a,0x986c,0x9a9c,0x9ccb,0x9ef7,0xa120,0xa348,0xa56c,
  0xa78d,0xa9ac,0xabc7,0xadde,0xaff3,0xb203,0xb40f,0xb618,
  0xb81c,0xba1c,0xbc17,0xbe0e,0xbfff,0xc1ec,0xc3d4,0xc5b6,
  0xc793,0xc96a,0xcb3c,0xcd08,0xcecd,0xd08d,0xd246,0xd3f9,
  0xd5a6,0xd74b,0xd8ea,0xda82,0xdc13,0xdd9c,0xdf1f,0xe09a,
  0xe20d,0xe379,0xe4dd,0xe639,0xe78d,0xe8d9,0xea1d,0xeb59,
  0xec8c,0xedb7,0xeed9,0xeff3,0xf104,0xf20c,0xf30b,0xf401,
  0xf4ef,0xf5d3,0xf6ad,0xf77f,0xf847,0xf906,0xf9bc,0xfa68,
  0xfb0a,0xfba3,0xfc32,0xfcb8,0xfd33,0xfda5,0xfe0e,0xfe6c,
  0xfec1,0xff0b,0xff4c,0xff83,0xffb0,0xffd3,0xffec,0xfffb,
};


/* -------------------------------------------------------------- */
/* --- テクスチャ管理クラス                                       */
/* -------------------------------------------------------------- */

/* ---------------------------------------- */
/* --- コンストラクタ・デストラクタ         */
TPuzzleDisp *TPuzzleDisp_Create(int  mode, int level, TGameScreen *scr)
{
  TPuzzleDisp *class;
  int  i, j;
  int  objstep;

  j = 0;
  objstep = 0;
  /* --- インスタンスの確保 */
  class = malloc(sizeof(TPuzzleDisp));
  /* ------------------------------------- */

  /* ----- 初期化あれこれ */
  class->puz_mode = mode;
  class->screen = scr;
  class->disptimer = 0;
  /* --- モードによってフィールド表示位置が違う */
  switch(mode) {

  case MODE_CHALLENGE:
#ifdef TAILTALE_HD
    class->fieldpos_x = 81;
#else
    class->fieldpos_x = 4;
#endif
    objstep = 0;
    j = 0;
    break;

  case MODE_1P:
    class->fieldpos_x = 0;
    objstep = 0;
    j = 0;
    break;

  case MODE_2P:
    class->fieldpos_x = 240;
    objstep = 1;
    j = 1;
    break;
  }

  /* ------------------------------------- */
  /* ----- 画面表示初期化 */
  for(i=0; i<TEXMAX; i++) {
    class->texture[i] = TGameScreen_GetTexture(scr, 1+j+(i*2)+objstep);
  }
  for(i=0; i<OBJMAX; i++) {
    class->obj[i] = TGameScreen_GetSprite(scr, 1+j+(i*2)+objstep);
  }
  for(i=0; i<KIRAMAX; i++) {
    class->kirakira[i].DispSw = FALSE;
    class->kirakira[i].obj = class->obj[128+i];
  }
  class->tanuki[0].y = 400;
  class->tanuki[1].y = 500;
  class->tanuki[2].y = 600;
  class->tanuki[3].y = 700;
  class->tanuki[0].x = rand() % (BLOCK_WIDTH*FIELD_WIDTH-60);
  class->tanuki[1].x = rand() % (BLOCK_WIDTH*FIELD_WIDTH-60);
  class->tanuki[2].x = rand() % (BLOCK_WIDTH*FIELD_WIDTH-60);
  class->tanuki[3].x = rand() % (BLOCK_WIDTH*FIELD_WIDTH-60);

  if (class) {
    /* -- スーパークラスの取得 */
    class->super = TPuzzleBase_Create(level);
  }

  /* --- インスタンスを渡して終了 */
  return(class);
}

void  TPuzzleDisp_Destroy(TPuzzleDisp *class)
{
  int  i;

  /* --- オブジェを消しておく */
  for(i=0; i<OBJMAX; i++) {
    class->obj[i]->DispSw = FALSE;
  }

  /* --- スーパークラスの解放 */
  TPuzzleBase_Destroy(class->super);
  /* --- インスタンスの解放 */
  free(class);
}



/* ---------------------------------------- */
/* --- 表示リクエスト                       */
/* ---------------------------------------- */
void TPuzzleDisp_DispField(TPuzzleDisp *class)
{
  class->disptimer += 1;

  /* ------------------------------------- */
  /* ----- 背景の表示 */
  DispBack(class);

  /* ------------------------------------- */
  /* ----- ブロックの表示 */
  DispBlock(class);

  /* ------------------------------------- */
  /* ----- カーソルの表示 */
  DispCursor(class);

  /* ------------------------------------- */
  /* ----- キャラクターの表示 */
  DispChara(class);

  /* ------------------------------------- */
  /* ----- NEXT の表示 */
  DispNext(class);

  /* ------------------------------------- */
  /* ----- スコア類の表示 */
  DispScore(class);

  /* ------------------------------------- */
  /* ----- きらきらエフェクトの表示 */
  DispKira(class);

#ifdef NETWORK
  if (network_game) {
    TNetwork_UpdateDisp();
    DispNet(class);
  }
#endif

#ifdef TOUCH
  DispTouch (class);
#endif
}

/* ---------------------------------------- */
/* --- Ready-Go! の表示                     */
/* ---------------------------------------- */
void TPuzzleDisp_DispReady(TPuzzleDisp *class, int time)
{
  int  i;

  if (time < 15*2) {
    /* -- Ready せりあがり */
    class->obj[160]->DispSw = TRUE;
    class->obj[160]->x = class->fieldpos_x + 37;
    class->obj[160]->y = 160 + (15 - (time/2)) * 20;
    class->obj[160]->w = 118;
    class->obj[160]->h = 54;
    class->obj[160]->tx = 0;
    class->obj[160]->ty = 0;
    class->obj[160]->Texture = TGameScreen_GetTexture(class->screen, 4);
    class->obj[160]->alpha = 255;
  }
  else if (time < 38*2) {
    /* -- Ready */
    class->obj[160]->DispSw = TRUE;
    class->obj[160]->x = class->fieldpos_x + 37;
    class->obj[160]->y = 160;
    class->obj[160]->w = 118;
    class->obj[160]->h = 54;
    class->obj[160]->tx = 0;
    class->obj[160]->ty = 0;
    class->obj[160]->Texture = TGameScreen_GetTexture(class->screen, 4);
    class->obj[160]->alpha = 255;
  }
  else if (time < 75*2) {
    /* -- Go!! */
    class->obj[160]->DispSw = TRUE;
    class->obj[160]->x = class->fieldpos_x + 46;
    class->obj[160]->y = 160;
    class->obj[160]->w = 99;
    class->obj[160]->h = 45;
    class->obj[160]->tx = 0;
    class->obj[160]->ty = 54;
    class->obj[160]->Texture = TGameScreen_GetTexture(class->screen, 4);
    class->obj[160]->alpha = 255;
    /* - スパークエフェクト */
    if (time == 38*2) {
      for(i=0; i<KIRAMAX; i++) {
        TPuzzleDisp_KiraRequest(class,
				class->fieldpos_x + 40 + 57,
				160+22,
				200);
      }
    }
  }
  else {
    /* -- 消す */
    class->obj[160]->DispSw = FALSE;
  }
}


/* ---------------------------------------- */
/* --- GAMEOVER の表示                      */
/* ---------------------------------------- */
void TPuzzleDisp_DispGameover(TPuzzleDisp *class, int time)
{
  int  mx, my;
  int  homex, homey;

#ifdef TAILTALE_HD
  homex = 84, homey = 114;
#else
  homex = 4, homey = 98;
#endif

  /* -- G */
  if (time > 40) {
    mx = homex;
    my = homey;
  } else {
    mx = homex + (40 - time) * 12;
    my = homey + ((disp_sin[(360 * (40 - time) / 20) % 360] - 0x8000) * (40 - time) * 6) / 0x8000;
  }
  class->obj[186]->DispSw = TRUE;
  class->obj[186]->x = mx;
  class->obj[186]->y = my;
  class->obj[186]->w = 23;
  class->obj[186]->h = 43;
  class->obj[186]->tx = 0;
  class->obj[186]->ty = 99;
  class->obj[186]->Texture = TGameScreen_GetTexture(class->screen, 4);
  class->obj[186]->alpha = 255;

  /* -- A */
  if (time > 45) {
    mx = homex + (23 * 1);
    my = homey;
  } else {
    mx = homex + (23 * 1) + (45 - time) * 12;
    my = homey + ((disp_sin[(360 * (45 - time) / 20 + 7) % 360] - 0x8000) * (45 - time) * 6) / 0x8000;
  }
  class->obj[187]->DispSw = TRUE;
  class->obj[187]->x = mx;
  class->obj[187]->y = my;
  class->obj[187]->w = 23;
  class->obj[187]->h = 43;
  class->obj[187]->tx = 23 * 1;
  class->obj[187]->ty = 99;
  class->obj[187]->Texture = TGameScreen_GetTexture(class->screen, 4);
  class->obj[187]->alpha = 255;

  /* -- M */
  if (time > 50) {
    mx = homex + (23 * 2);
    my = homey;
  } else {
    mx = homex + (23 * 2) + (50 - time) * 12;
    my = homey + ((disp_sin[(360 * (50 - time) / 20 + 2) % 360] - 0x8000) * (50 - time) * 6) / 0x8000;
  }
  class->obj[188]->DispSw = TRUE;
  class->obj[188]->x = mx;
  class->obj[188]->y = my;
  class->obj[188]->w = 23;
  class->obj[188]->h = 43;
  class->obj[188]->tx = 23 * 2;
  class->obj[188]->ty = 99;
  class->obj[188]->Texture = TGameScreen_GetTexture(class->screen, 4);
  class->obj[188]->alpha = 255;

  /* -- E */
  if (time > 55) {
    mx = homex + (23 * 3);
    my = homey;
  } else {
    mx = homex + (23 * 3) + (55 - time) * 12;
    my = homey + ((disp_sin[(360 * (55 - time) / 20 + 6) % 360] - 0x8000) * (55 - time) * 6) / 0x8000;
  }
  class->obj[189]->DispSw = TRUE;
  class->obj[189]->x = mx;
  class->obj[189]->y = my;
  class->obj[189]->w = 23;
  class->obj[189]->h = 43;
  class->obj[189]->tx = 23 * 3;
  class->obj[189]->ty = 99;
  class->obj[189]->Texture = TGameScreen_GetTexture(class->screen, 4);
  class->obj[189]->alpha = 255;

  /* -- O */
  if (time > 60) {
    mx = homex + (23 * 4);
    my = homey;
  } else {
    mx = homex + (23 * 4) + (60 - time) * 12;
    my = homey + ((disp_sin[(360 * (60 - time) / 20 + 4) % 360] - 0x8000) * (60 - time) * 6) / 0x8000;
  }
  class->obj[190]->DispSw = TRUE;
  class->obj[190]->x = mx;
  class->obj[190]->y = my;
  class->obj[190]->w = 23;
  class->obj[190]->h = 43;
  class->obj[190]->tx = 23 * 4;
  class->obj[190]->ty = 99;
  class->obj[190]->Texture = TGameScreen_GetTexture(class->screen, 4);
  class->obj[190]->alpha = 255;

  /* -- V */
  if (time > 65) {
    mx = homex + (23 * 5);
    my = homey;
  } else {
    mx = homex + (23 * 5) + (65 - time) * 12;
    my = homey + ((disp_sin[(360 * (65 - time) / 20 + 2) % 360] - 0x8000) * (65 - time) * 6) / 0x8000;
  }
  class->obj[191]->DispSw = TRUE;
  class->obj[191]->x = mx;
  class->obj[191]->y = my;
  class->obj[191]->w = 23;
  class->obj[191]->h = 43;
  class->obj[191]->tx = 23 * 5;
  class->obj[191]->ty = 99;
  class->obj[191]->Texture = TGameScreen_GetTexture(class->screen, 4);
  class->obj[191]->alpha = 255;

  /* -- E */
  if (time > 70) {
    mx = homex + (23 * 6);
    my = homey;
  } else {
    mx = homex + (23 * 6) + (70 - time) * 12;
    my = homey + ((disp_sin[(360 * (70 - time) / 20 + 6) % 360] - 0x8000) * (70 - time) * 6) / 0x8000;
  }
  class->obj[192]->DispSw = TRUE;
  class->obj[192]->x = mx;
  class->obj[192]->y = my;
  class->obj[192]->w = 23;
  class->obj[192]->h = 43;
  class->obj[192]->tx = 23 * 6;
  class->obj[192]->ty = 99;
  class->obj[192]->Texture = TGameScreen_GetTexture(class->screen, 4);
  class->obj[192]->alpha = 255;

  /* -- R */
  if (time > 75) {
    mx = homex + (23 * 7);
    my = homey;
  } else {
    mx = homex + (23 * 7) + (75 - time) * 12;
    my = homey + ((disp_sin[(360 * (75 - time) / 20 + 1) % 360] - 0x8000) * (75 - time) * 6) / 0x8000;
  }
  class->obj[193]->DispSw = TRUE;
  class->obj[193]->x = mx;
  class->obj[193]->y = my;
  class->obj[193]->w = 23;
  class->obj[193]->h = 43;
  class->obj[193]->tx = 23 * 7;
  class->obj[193]->ty = 99;
  class->obj[193]->Texture = TGameScreen_GetTexture(class->screen, 4);
  class->obj[193]->alpha = 255;
}

/* ---------------------------------------- */
/* --- オブジェクトの消去                   */
/* ---------------------------------------- */
void TPuzzleDisp_DispClear(TPuzzleDisp *class)
{
  int  i;

  /* --- オブジェを消しておく */
  for(i=0; i<OBJMAX; i++) {
    class->obj[i]->DispSw = FALSE;
  }
}


/* ---------------------------------------- */
/* --- キラキラエフェクトのリクエスト       */
/* ---------------------------------------- */
void TPuzzleDisp_KiraRequest(TPuzzleDisp *class,
			     int x, int y, int l)
{
  int  i, j;
  int  hit;

  hit = FALSE;
  /* -- 空きエフェクトを探す */
  for(i=0; i<KIRAMAX; i++) {
    if (class->kirakira[i].DispSw == FALSE) {
      hit = TRUE;
      j = i;
      break;
    }
  }
  /* -- リクエスト */
  if (hit == TRUE) {
    class->kirakira[j].DispSw = TRUE;
    class->kirakira[j].timer = 0;
    class->kirakira[j].timer_end = 22;
    class->kirakira[j].x = x * 0x10000;
    class->kirakira[j].y = y * 0x10000;
    class->kirakira[j].dx = (disp_cos[(rand()%360)] - 0x8000) * (l / 15.0);
    class->kirakira[j].dy = (disp_sin[(rand()%360)] - 0x8000) * (l / 15.0);
  }
}


/* -------------------------------------------------------------- */
/* --- クラスメソッド                                             */
/* -------------------------------------------------------------- */


/* ---------------------------------------- */
/* --- 手のひらカーソルの表示               */
/* ---------------------------------------- */
void  DispCursor(TPuzzleDisp *class)
{
  int  x, y;

  /* ----- ハンドカーソル */
  x = class->fieldpos_x + (class->super->UA.X * BLOCK_WIDTH) + 4;
  y = FIELDTOP + ((FIELD_HEIGHT - class->super->UA.Y - 1) * BLOCK_WIDTH) + 4;
  class->obj[122]->DispSw = TRUE;
  class->obj[122]->x = x + class->super->UA.SwapOffsetX;
  class->obj[122]->y = y + class->super->UA.SwapOffsetY + class->super->UA.PopupOffset;
  class->obj[122]->w = BLOCK_WIDTH;
  class->obj[122]->h = BLOCK_HEIGHT;
  if (class->super->UA.HaveBlock == FALSE) {
    class->obj[122]->tx = 24 * 2;
  }
  else {
    class->obj[122]->tx = 24 * 3;
  }
  class->obj[122]->ty = (24 * 2)+72;
  class->obj[122]->Texture = class->texture[0];
  class->obj[122]->alpha = 255;
  /* ----- つかみブロック枠 */
  if (class->super->UA.HaveBlock == TRUE) {
    x = class->fieldpos_x + (class->super->UA.X * BLOCK_WIDTH);
    y = FIELDTOP + ((FIELD_HEIGHT - class->super->UA.Y - 1) * BLOCK_WIDTH);
    class->obj[121]->DispSw = TRUE;
    class->obj[121]->x = x + class->super->UA.SwapOffsetX;
    class->obj[121]->y = y + class->super->UA.SwapOffsetY + class->super->UA.PopupOffset;
    class->obj[121]->w = BLOCK_WIDTH;
    class->obj[121]->h = BLOCK_HEIGHT;
    if ((class->disptimer % 2) == 0) {
      class->obj[121]->tx = 24 * 0;
    }
    else {
      class->obj[121]->tx = 24 * 1;
    }
    class->obj[121]->ty = (24 * 2)+72;
    class->obj[121]->Texture = class->texture[0];
    class->obj[121]->alpha = 255;
  }
  else {
    class->obj[121]->DispSw = FALSE;
  }
}


/* ---------------------------------------- */
/* --- ブロック群の表示                     */
/* ---------------------------------------- */
void  DispBlock(TPuzzleDisp *class)
{
  int  i;
  int  colx, coly;
  int  x, y;

  for(i = 0; i < (FIELD_WIDTH * FIELD_HEIGHT); i++) {
    if (class->super->Field[i] != 0) {
      if (class->super->Field[i]->Color != 0) {
        if (class->super->Field[i]->Color >= 0x10) {
          colx = ((class->super->Field[i]->Color - 0x10) - 1) * BLOCK_WIDTH;
          coly = BLOCK_HEIGHT;
        } else {
          colx = (class->super->Field[i]->Color - 1) * BLOCK_WIDTH;
          coly = 0;
        }

        x = ((i % FIELD_WIDTH) * BLOCK_WIDTH);
        if ((i / FIELD_WIDTH) == 0) {
          y = ((FIELD_HEIGHT - 1) - (i / FIELD_WIDTH)) * BLOCK_HEIGHT + 4 + FIELDTOP;
        } else {          
          y = ((FIELD_HEIGHT - 1) - (i / FIELD_WIDTH)) * BLOCK_HEIGHT + FIELDTOP;
        }

        class->obj[10+i]->DispSw = TRUE;
        class->obj[10+i]->x = x + class->super->Field[i]->SwapOffsetX + class->fieldpos_x;
        class->obj[10+i]->y = y + class->super->Field[i]->SwapOffsetY + class->super->Field[i]->PopupOffset - class->super->Field[i]->DropOffset;
        class->obj[10+i]->w = BLOCK_WIDTH;
        class->obj[10+i]->h = BLOCK_HEIGHT;
        class->obj[10+i]->tx = colx;
        class->obj[10+i]->ty = coly + 72;
        class->obj[10+i]->Texture = class->texture[0];
        class->obj[10+i]->alpha = 255;

#ifdef NETWORK
        /* Note: This will give the Network info of the Current Board,
        it save the 64 numbers of Color and update it anytime... */
        if (network_game)
		  network_read_board[i] = (class->super->Field[i]->Color - 1 / BLOCK_WIDTH) + '0';
#endif

        /* --- 消えエフェクト中か */
        if (class->super->Field[i]->LineTimer > 0) {
          DispEraseBlock(class, class->obj[10+i], class->super->Field[i]);
        }
      }
    } else {
      class->obj[10+i]->DispSw = FALSE;
#ifdef NETWORK
      if (network_game) /* Note: Clean current Board. */
        network_read_board[i] = (8) + '0';
#endif
    }
  }
}

/* ---------------------------------------- */
/* --- 消えるブロックの表示                 */
/* ---------------------------------------- */
void  DispEraseBlock(TPuzzleDisp *class,
		     TGameSprite *obj,
		     Block *bl)
{
  if (bl->LineTimer < 3)
    obj->tx = 0, obj->ty = 367;
  else {
    if ((bl->LineTimer % 2) == 0)
      obj->tx = 0, obj->ty = 367;
  }

  /* -- きらきらエフェクト */
  if (bl->LineTimer == 3) {
    TPuzzleDisp_KiraRequest(class, 
			    obj->x+(BLOCK_WIDTH / 2),
			    obj->y+(BLOCK_HEIGHT / 2),
			    60);
    TPuzzleDisp_KiraRequest(class, 
			    obj->x+(BLOCK_WIDTH / 2),
			    obj->y+(BLOCK_HEIGHT / 2),
			    60);
    TPuzzleDisp_KiraRequest(class, 
			    obj->x+(BLOCK_WIDTH / 2),
			    obj->y+(BLOCK_HEIGHT / 2),
			    60);
  }
}


/* ---------------------------------------- */
/* --- 背景の表示                           */
/* ---------------------------------------- */

void  DispBack(TPuzzleDisp *class)
{
    class->obj[0]->DispSw = TRUE;
#ifdef TAILTALE_HD
    class->obj[0]->x = 81;
#else
    class->obj[0]->x = 4;
#endif
    class->obj[0]->y = 0;
    class->obj[0]->w = 192;
    class->obj[0]->h = 256;
    class->obj[0]->tx = 0;
    class->obj[0]->ty = 0;
    class->obj[0]->Texture = TGameScreen_GetTexture(class->screen, 3);
    class->obj[0]->alpha = 255;

    class->obj[1]->DispSw = TRUE;
    class->obj[1]->x = 0;
    class->obj[1]->y = 0;
#ifdef TAILTALE_HD
    class->obj[1]->w = 480;
    class->obj[1]->h = 272;
    class->obj[1]->tx = 0;
#else
    class->obj[1]->w = 320;
    class->obj[1]->h = 240;
    class->obj[1]->tx = 0;
#endif
    class->obj[1]->ty = 0;
    class->obj[1]->Texture = TGameScreen_GetTexture(class->screen, 0);
    class->obj[1]->alpha = 255;

    /* -------------------------------------- */
    /* --- 隠すべき枠 */
    class->obj[2]->DispSw = TRUE;
#ifdef TAILTALE_HD
    class->obj[2]->x = 81;
    class->obj[2]->y = 272-24-4;
    class->obj[2]->w = 192;
    class->obj[2]->h = 29;
    class->obj[2]->tx = 81;
    class->obj[2]->ty = 244;
#else
    class->obj[2]->x = 4;
    class->obj[2]->y = 220;
    class->obj[2]->w = 192;
    class->obj[2]->h = 23;
    class->obj[2]->tx = 4;
    class->obj[2]->ty = 216;
#endif
    class->obj[2]->Texture = TGameScreen_GetTexture(class->screen, 0);
    class->obj[2]->alpha = 255;
}

/* ---------------------------------------- */
/* --- キャラクターの表示                   */
/* ---------------------------------------- */
void  DispChara(TPuzzleDisp *class)
{
  int  i, koma, tanux;

  class->texture[3] = TGameScreen_GetTexture(class->screen, 2);
  koma = (class->disptimer / 8) % 8;
  class->obj[9]->DispSw = TRUE;
  class->obj[9]->x = class->fieldpos_x;
  class->obj[9]->y = FIELDTOP;
  class->obj[9]->w = 192;
#ifdef TAILTALE_HD
  class->obj[9]->h = 240;
#else
  class->obj[9]->h = 240 - 23;
#endif
  class->obj[9]->tx = animekoma[koma] * 192;
  class->obj[9]->ty = 16;
  class->obj[9]->Texture = class->texture[3];
  class->obj[9]->alpha = 255;

  /* --- 後ろのタヌキ絵 */
  for (i = 0; i < 4; i++) {
    if ((class->super->Level > 6) || (class->super->Level > 21))
      class->tanuki[i].y = class->tanuki[i].y - 1;
	else
      class->tanuki[i].y = class->tanuki[i].y + 1;

    if ((class->tanuki[i].y < 0) || (class->tanuki[i].y > 214)) {
      class->tanuki[i].x = rand() % (BLOCK_WIDTH * FIELD_WIDTH - 60);
      if ((class->super->Level > 6) || (class->super->Level > 21))
        class->tanuki[i].y = 214 + rand() % 40;
	  else
        class->tanuki[i].y = 0 + rand() % 40;
	}

#ifdef NETWORK
    if (!network_game) {
#endif
    class->obj[i+2]->DispSw = TRUE;
    tanux = disp_sin[(int)(360*(class->tanuki[i].y % 120) / 120)%360] * 8 / 0x10000;
    class->obj[i+2]->y = class->tanuki[i].y;
    class->obj[i+2]->w = 36;
    class->obj[i+2]->h = 22;
	if (class->super->Level < 6) {
        class->obj[i+2]->x = class->tanuki[i].x + tanux + class->fieldpos_x + 20;
        class->obj[i+2]->tx = 108;
	} else if (class->super->Level < 11) {
        class->obj[i+2]->x = class->tanuki[i].x + tanux + class->fieldpos_x - 20;
        class->obj[i+2]->tx = 144;
	} else if (class->super->Level < 16) {
        class->obj[i+2]->x = class->tanuki[i].x + tanux + class->fieldpos_x + 20;
        class->obj[i+2]->tx = 108;
	} else if (class->super->Level < 21) {
        class->obj[i+2]->x = class->tanuki[i].x + tanux + class->fieldpos_x + 20;
        class->obj[i+2]->tx = 180;
	} else if (class->super->Level < 26) {
        class->obj[i+2]->x = class->tanuki[i].x + tanux + class->fieldpos_x + 20;
        class->obj[i+2]->tx = 72;
	} else if (class->super->Level > 26) {
        class->obj[i+2]->DispSw = FALSE;
	}
    class->obj[i+2]->ty = 144;
    class->obj[i+2]->Texture = class->texture[0];
    class->obj[i+2]->alpha = 255;
#ifdef NETWORK
    }
#endif
  }
}

/* ---------------------------------------- */
/* --- ネクストブロックの 'NEXT'            */
/* ---------------------------------------- */
void  DispNext(TPuzzleDisp *class)
{
  int  mode;

  mode = 0;
  if (class->super->NextTimer < 24*2) {
    mode = 34 * (class->super->NextTimer % 2);
  }
  class->obj[120]->DispSw = TRUE;
  class->obj[120]->x = class->fieldpos_x + 62;
  class->obj[120]->y = FIELDTOP + ((FIELD_HEIGHT - 1) * BLOCK_HEIGHT) - 18;
  class->obj[120]->w = 67;
  class->obj[120]->h = 34;
  class->obj[120]->tx = 118;
  class->obj[120]->ty = mode;
  class->obj[120]->Texture = TGameScreen_GetTexture(class->screen, 4);
  class->obj[120]->alpha = 128;
}


/* ---------------------------------------- */
/* --- SCORE等のインフォメーション          */
/* ---------------------------------------- */
void  DispScore (TPuzzleDisp *class) {
#ifndef TAILTALE_HD
  int shift = 152;
#endif

  /* 168 : SCORE */
  /* 169 : BLOCK */
  /* 170 : LEVEL */
  /* 171-178 : score num (400x48) */
  /* 179-183 : block num (424x96) */
  /* 184-185 : score num (436x144) */

#ifdef NETWORK
  if (network_game == 1) {
    network_game_score = class->super->Score;
    network_game_level = class->super->Level;
	network_game_block = class->super->EraseBlock;
  }
#endif

  /* ------------------------------ */
  /* --- 文字 */
  class->obj[168]->DispSw = TRUE;
#ifdef TAILTALE_HD
  class->obj[168]->x = 416;
#else
  class->obj[168]->x = 416 - shift;
#endif
  class->obj[168]->y = 27;
  class->obj[168]->w = 48;
  class->obj[168]->h = 18;
  class->obj[168]->tx = 0 + 168;
  class->obj[168]->ty = 16 + 72;
  class->obj[168]->Texture = class->texture[0];
  class->obj[168]->alpha = 255;
  class->obj[169]->DispSw = TRUE;
#ifdef TAILTALE_HD
  class->obj[169]->x = 364;
#else
  class->obj[169]->x = 364 - shift;
#endif
  class->obj[169]->y = 75;
  class->obj[169]->w = 100;
  class->obj[169]->h = 18;
  class->obj[169]->tx = 0 + 168;
  class->obj[169]->ty = 34 + 72;
  class->obj[169]->Texture = class->texture[0];
  class->obj[169]->alpha = 255;

  class->obj[170]->DispSw = TRUE;
#ifdef TAILTALE_HD
  class->obj[170]->x = 421;
#else
  class->obj[170]->x = 421 - shift;
#endif
  class->obj[170]->y = 123;
  class->obj[170]->w = 43;
  class->obj[170]->h = 18;
  class->obj[170]->tx = 0 + 168;
  class->obj[170]->ty = 52 + 72;
  class->obj[170]->Texture = class->texture[0];
  class->obj[170]->alpha = 255;

#ifdef TAILTALE_HD
  DispNum(class, 400, 48, 8, 171, class->super->Score);
  DispNum(class, 424, 96, 5, 179, class->super->EraseBlock);
  DispNum(class, 448, 144, 2, 184, class->super->Level);
#else
  DispNum(class, 400 - shift, 48, 8, 171, class->super->Score);
  DispNum(class, 424 - shift, 96, 5, 179, class->super->EraseBlock);
  DispNum(class, 448 - shift, 144, 2, 184, class->super->Level);
#endif
}


/* ---------------------------------------- */
/* --- 数値を表示するサブルーチン           */
/* ---------------------------------------- */
void  DispNum(TPuzzleDisp *class,
	      int x, int y,
	      int n, int o,
	      int num)
{
  int  i, j;
  int  c;
  int  top;

  /* x, y : 表示位置 */
  /* n : 表示文字数 */
  /* o : Obj番号 */
  /* num : 表示する数字 */
  top = TRUE;
  j = 1;
  for(i=0; i<n; i++) {
    j = j * 10;
  }
  if (num >= j) num = j - 1;
  for(i=0; i<n; i++) {
    c = num / (j / 10);
    num = num % (j / 10);
    if (c > 9) c = 9;
    if (i == (n - 1)) top = FALSE;
    /* -- 表示 */
    if ((c == 0) && (top == TRUE)) {
      class->obj[o]->DispSw = FALSE;
    }
    else {
      top = FALSE;
      class->obj[o]->DispSw = TRUE;
      class->obj[o]->x = x;
      class->obj[o]->y = y;
      class->obj[o]->w = 8;
      class->obj[o]->h = 16;
      class->obj[o]->tx = c * 8 + 168;
      class->obj[o]->ty = 0 + 72;
      class->obj[o]->Texture = class->texture[0];
      class->obj[o]->alpha = 255;
    }
    x = x + 8;
    o = o + 1;
    j = j / 10;
  }
}


/* ---------------------------------------- */
/* --- きらきらエフェクトの表示と移動       */
/* ---------------------------------------- */
void  DispKira(TPuzzleDisp *class)
{
  int  i;
  int  alpha;
  int  d;

  for(i=0; i<KIRAMAX; i++) {
    if (class->kirakira[i].DispSw == TRUE) {
      /* -- 表示位置 */
      class->kirakira[i].x = class->kirakira[i].x + class->kirakira[i].dx;
      class->kirakira[i].y = class->kirakira[i].y + class->kirakira[i].dy;
      /* -- 移動 */
      alpha = 255;
      if (class->kirakira[i].timer > (class->kirakira[i].timer_end / 4)) {
        class->kirakira[i].dx = (class->kirakira[i].dx * 0xc0) / 0x100;
        class->kirakira[i].dy = (class->kirakira[i].dy * 0xc0) / 0x100;
        d = class->kirakira[i].timer - (class->kirakira[i].timer_end / 2);
        d = (d * 255) / (class->kirakira[i].timer_end / 2);
        alpha = 255 - d;
        if (alpha < 0) alpha = 0;
        if (alpha > 255) alpha = 255;
      }
      /* -- 表示 */
      class->kirakira[i].obj->DispSw = TRUE;
      class->kirakira[i].obj->x = class->kirakira[i].x / 0x10000;
      class->kirakira[i].obj->y = class->kirakira[i].y / 0x10000;
      class->kirakira[i].obj->w = 11;
      class->kirakira[i].obj->h = 11;
      class->kirakira[i].obj->tx = 144;
      class->kirakira[i].obj->ty = 48 + 72;
      class->kirakira[i].obj->Texture = class->texture[0];
      class->kirakira[i].obj->alpha = alpha;
      /* -- 表示時間の管理 */
      class->kirakira[i].timer = class->kirakira[i].timer + 1;
      if (class->kirakira[i].timer >= class->kirakira[i].timer_end) {
        class->kirakira[i].obj->DispSw = FALSE;
        class->kirakira[i].DispSw = FALSE;
      }
    }
  }
}

#ifdef NETWORK
/*
	TPuzzleDisp_DispNetStatus
*/
void TPuzzleDisp_DispNetStatus(TPuzzleDisp *class, int time, int status) {
  int  mx, my;
  int  homex, homey;

#ifdef TAILTALE_HD
  homex = 84, homey = 114;
#else
  homex = 4, homey = 98;
#endif

	/* Y */
	if (time > 45) {
		mx = homex + (23 * 1);
		my = homey;
	} else {
		mx = homex + (23 * 1) + (45 - time) * 12;
		my = homey + ((disp_sin[(360 * (45 - time) / 20 + 7) % 360] - 0x8000) * (45 - time) * 6) / 0x8000;
	}
	class->obj[187]->DispSw = TRUE;
	class->obj[187]->x = mx;
	class->obj[187]->y = my;
	class->obj[187]->w = 23;
	class->obj[187]->h = 43;
	class->obj[187]->tx = 23 * 1;
	class->obj[187]->ty = 142 + (43 * status);
	class->obj[187]->Texture = TGameScreen_GetTexture(class->screen, 4);
	class->obj[187]->alpha = 255;

	/* o */
	if (time > 50) {
		mx = homex + (23 * 2);
		my = homey;
	} else {
		mx = homex + (23 * 2) + (50 - time) * 12;
		my = homey + ((disp_sin[(360 * (50 - time) / 20 + 2) % 360] - 0x8000) * (50 - time) * 6) / 0x8000;
	}
	class->obj[188]->DispSw = TRUE;
	class->obj[188]->x = mx;
	class->obj[188]->y = my;
	class->obj[188]->w = 23;
	class->obj[188]->h = 43;
	class->obj[188]->tx = 23 * 2;
	class->obj[188]->ty = 142 + (43 * status);
	class->obj[188]->Texture = TGameScreen_GetTexture(class->screen, 4);
	class->obj[188]->alpha = 255;

	/* u */
	if (time > 55) {
		mx = homex + (23 * 3);
		my = homey;
	} else {
		mx = homex + (23 * 3) + (55 - time) * 12;
		my = homey + ((disp_sin[(360 * (55 - time) / 20 + 6) % 360] - 0x8000) * (55 - time) * 6) / 0x8000;
	}
	class->obj[189]->DispSw = TRUE;
	class->obj[189]->x = mx;
	class->obj[189]->y = my;
	class->obj[189]->w = 23;
	class->obj[189]->h = 43;
	class->obj[189]->tx = 23 * 3;
	class->obj[189]->ty = 142 + (43 * status);
	class->obj[189]->Texture = TGameScreen_GetTexture(class->screen, 4);
	class->obj[189]->alpha = 255;

	/* L, W */
	if (time > 60) {
		mx = homex + (23 * 4);
		my = homey;
	} else {
		mx = homex + (23 * 4) + (60 - time) * 12;
		my = homey + ((disp_sin[(360 * (60 - time) / 20 + 4) % 360] - 0x8000) * (60 - time) * 6) / 0x8000;
	}
	class->obj[190]->DispSw = TRUE;
	class->obj[190]->x = mx;
	class->obj[190]->y = my;
	class->obj[190]->w = 23;
	class->obj[190]->h = 43;
	class->obj[190]->tx = 23 * 4;
	class->obj[190]->ty = 142 + (43 * status);
	class->obj[190]->Texture = TGameScreen_GetTexture(class->screen, 4);
	class->obj[190]->alpha = 255;

	/* o, i */
	if (time > 65) {
		mx = homex + (23 * 5);
		my = homey;
	} else {
		mx = homex + (23 * 5) + (65 - time) * 12;
		my = homey + ((disp_sin[(360 * (65 - time) / 20 + 2) % 360] - 0x8000) * (65 - time) * 6) / 0x8000;
	}
	class->obj[191]->DispSw = TRUE;
	class->obj[191]->x = mx;
	class->obj[191]->y = my;
	class->obj[191]->w = 23;
	class->obj[191]->h = 43;
	class->obj[191]->tx = 23 * 5;
	class->obj[191]->ty = 142 + (43 * status);
	class->obj[191]->Texture = TGameScreen_GetTexture(class->screen, 4);
	class->obj[191]->alpha = 255;

	/* se, n */
	if (time > 70) {
		mx = homex + (23 * 6);
		my = homey;
	} else {
		mx = homex + (23 * 6) + (70 - time) * 12;
		my = homey + ((disp_sin[(360 * (70 - time) / 20 + 6) % 360] - 0x8000) * (70 - time) * 6) / 0x8000;
	}
	class->obj[192]->DispSw = TRUE;
	class->obj[192]->x = mx;
	class->obj[192]->y = my;
	class->obj[192]->w = 46; // Info: need two characters...
	class->obj[192]->h = 43;
	class->obj[192]->tx = 23 * 6;
	class->obj[192]->ty = 142 + (43 * status);
	class->obj[192]->Texture = TGameScreen_GetTexture(class->screen, 4);
	class->obj[192]->alpha = 255;
}

/*
	DispNet
*/
void DispNet(TPuzzleDisp *class)
{
  int i, n = 0, koma;

  if (network_game) {
    class->obj[198]->DispSw = TRUE;
#ifdef TAILTALE_HD
    class->obj[198]->x = 415;
    class->obj[198]->y = 186;
#else
    class->obj[198]->x = 268;
    class->obj[198]->y = 167;
#endif
    class->obj[198]->w = 51;
    class->obj[198]->h = 67;
    class->obj[198]->tx = 0;
    class->obj[198]->ty = 0;
    class->obj[198]->Texture = class->texture[0];
    class->obj[198]->alpha = 255;

    class->obj[199]->DispSw = TRUE;
#ifdef TAILTALE_HD
    class->obj[199]->x = 416;
    class->obj[199]->y = 187;
#else
    class->obj[199]->x = 269;
    class->obj[199]->y = 168;
#endif
    class->obj[199]->w = 49;
    class->obj[199]->h = 59;
	if ((time_dayandnight) ?  "night" : "day")
		class->obj[199]->tx = 147 + (49 * 1);
	else
		class->obj[199]->tx = 147 + (49 * 0);
	if (evolution || atoi (network_words[9])) {
		switch (atoi (network_words[2])) {
			case 6:
				class->obj[199]->ty = 59;
				break;
			case 11:
				class->obj[199]->ty = 118;
				break;
			case 16:
				class->obj[199]->ty = 177;
				break;
			case 21:
				class->obj[199]->ty = 236;
				break;
			case 26:
				class->obj[199]->ty = 295;
				break;
			default:
				class->obj[199]->ty = 0;
				break;
		}
	} else
		class->obj[199]->ty = 354;
    class->obj[199]->Texture = TGameScreen_GetTexture(class->screen, 5);
    class->obj[199]->alpha = 255;

	koma = (class->disptimer / 8) % 8;
	class->obj[200]->DispSw = TRUE;
#ifdef TAILTALE_HD
    class->obj[200]->x = 416;
    class->obj[200]->y = 187;
#else
    class->obj[200]->x = 269;
    class->obj[200]->y = 168;
#endif
	class->obj[200]->w = 49;
	class->obj[200]->h = 59;
	class->obj[200]->tx = animekoma[koma] * 49;
	if (evolution || atoi (network_words[9])) {
		switch (atoi (network_words[2])) {
			case 6:
				class->obj[200]->ty = 59;
				break;
			case 11:
				class->obj[200]->ty = 118;
				break;
			case 16:
				class->obj[200]->ty = 177;
				break;
			case 21:
				class->obj[200]->ty = 236;
				break;
			case 26:
				class->obj[200]->ty = 295;
				break;
			default:
				class->obj[200]->ty = 0;
				break;
		}
	} else
		class->obj[200]->ty = 354;
    class->obj[200]->Texture = TGameScreen_GetTexture(class->screen, 5);
    class->obj[200]->alpha = 255;

    /* This will show the Client/Server gameboard... */
    for (n = 0; n < FIELD_HEIGHT; n++) { // Height
     for (i = 0; i < FIELD_WIDTH; i++) { // Width
      if ((5 * (network_read_board[i + n * 8] - '0')) > 40)
        class->obj[201 + (i + n * 8)]->DispSw = FALSE;
	  else
        class->obj[201 + (i + n * 8)]->DispSw = TRUE;
#ifdef TAILTALE_HD
      class->obj[201 + (i + n * 8)]->x = 417 + (i * 6);
      class->obj[201 + (i + n * 8)]->y = 247 - (n * 6);
#else
      class->obj[201 + (i + n * 8)]->x = 270 + (i * 6);
      class->obj[201 + (i + n * 8)]->y = 228 - (n * 6);
#endif
      class->obj[201 + (i + n * 8)]->w = 5;
      class->obj[201 + (i + n * 8)]->h = 5;
	  class->obj[201 + (i + n * 8)]->tx = -5 + (5 * (network_write_board[i + n * 8] - '0'));
      class->obj[201 + (i + n * 8)]->ty = 67;
      class->obj[201 + (i + n * 8)]->Texture = class->texture[0];
      class->obj[201 + (i + n * 8)]->alpha = 255;
	 }
    }

    class->obj[290]->DispSw = FALSE;
#ifdef TAILTALE_HD
    class->obj[290]->x = 102;
    class->obj[290]->y = 114;
#else
    class->obj[290]->x = 22;
    class->obj[290]->y = 98;
#endif
    class->obj[290]->w = 147;
    class->obj[290]->h = 43;
    class->obj[290]->tx = 0;
    class->obj[290]->ty = 142;
    class->obj[290]->Texture = TGameScreen_GetTexture(class->screen, 4);
    class->obj[290]->alpha = 255;

    if ((atoi (network_words[5]) == 1) || (atoi (network_words[6]) == 2))
      class->obj[290]->DispSw = TRUE;
	else if ((atoi (network_words[5]) == 2) || (atoi (network_words[6]) == 1)) {
      class->obj[290]->DispSw = TRUE;
      class->obj[290]->ty = 185;
	} else
      class->obj[290]->DispSw = FALSE;
  }
}
#endif

#ifdef TOUCH
void DispTouch (TPuzzleDisp *class) {
    class->obj[292]->DispSw = TRUE; // Arrow
    class->obj[292]->x = 32 + touch_x;
    class->obj[292]->y = SCREEN_HEIGHT - 96 + touch_y;
    class->obj[292]->w = 64;
    class->obj[292]->h = 64;
    class->obj[292]->tx = 0;
    class->obj[292]->ty = 0;
    class->obj[292]->Texture = TGameScreen_GetTexture(class->screen, 6);
    class->obj[292]->alpha = 255;

    class->obj[293]->DispSw = TRUE; // + & -
    class->obj[293]->x = SCREEN_WIDTH / 2 - 72;
    class->obj[293]->y = SCREEN_HEIGHT - 80;
    class->obj[293]->w = 64;
    class->obj[293]->h = 64;
    class->obj[293]->tx = 256;
    class->obj[293]->ty = 0;
    class->obj[293]->Texture = TGameScreen_GetTexture(class->screen, 6);
    class->obj[293]->alpha = 255;
    class->obj[294]->DispSw = TRUE;
    class->obj[294]->x = SCREEN_WIDTH / 2 + 8;
    class->obj[294]->y = SCREEN_HEIGHT - 80;
    class->obj[294]->w = 64;
    class->obj[294]->h = 64;
    class->obj[294]->tx = 320;
    class->obj[294]->ty = 0;
    class->obj[294]->Texture = TGameScreen_GetTexture(class->screen, 6);
    class->obj[294]->alpha = 255;

	class->obj[295]->DispSw = TRUE; // A & B
    class->obj[295]->x = SCREEN_WIDTH - 136;
    class->obj[295]->y = SCREEN_HEIGHT - 136;
    class->obj[295]->w = 128;
    class->obj[295]->h = 64;
    class->obj[295]->tx = 384;
    class->obj[295]->ty = 0;
    class->obj[295]->Texture = TGameScreen_GetTexture(class->screen, 6);
    class->obj[295]->alpha = 255;

	class->obj[296]->DispSw = TRUE; // 1 & 2
    class->obj[296]->x = SCREEN_WIDTH - 136;
    class->obj[296]->y = SCREEN_HEIGHT - 72;
    class->obj[296]->w = 128;
    class->obj[296]->h = 64;
    class->obj[296]->tx = 512;
    class->obj[296]->ty = 0;
    class->obj[296]->Texture = TGameScreen_GetTexture(class->screen, 6);
    class->obj[296]->alpha = 255;
}
#endif
