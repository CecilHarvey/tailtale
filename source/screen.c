/* ---------------------------------------------------------- */
/*  screen.c                                              */
/* ---------------------------------------------------------- */

/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintaner  Rerorero@fumi.   */
/*                                                        */
/*   screen.c                                         */
/*     画面管理クラス                                     */
/*                                                        */
/*--------------------------------------------------------*/


/*------------------------------------------------------------- */
/** @file
    @brief		描画スクリーン管理
    @author		K.Kunikane (rerofumi)
    @since		Jul.27.2005
*/
/*-----------------------------------------------------
 Copyright (C) 2002,2005 rerofumi <rero2@yuumu.org>
 All Rights Reserved.
 ------------------------------------------------------*/

/*-------------------------------*/
/* include                       */
/*-------------------------------*/

#include "default.h"

#include "table.h"

/*-------------------------------*/
/* local function                */
/*-------------------------------*/

/* --- スプライトを一枚スクリーンに貼り付ける */
void Render(TGameScreen *class, TGameSprite *spr);
void RenderZoomRot(TGameScreen *class, TGameSprite *spr);

/* -------------------------------------------------------------- */
/* --- スクリーン管理クラス                                       */
/* -------------------------------------------------------------- */

SDL_Surface *screenshot = NULL;

/* ---------------------------------------- */
/* --- コンストラクタ・デストラクタ         */
TGameScreen *TGameScreen_Create(int width, int height, int depth)
{
  int  i;
#ifdef X86
  char screen_set[16][128];
#endif

  TGameScreen *class;

  /* --- インスタンスの生成 */
  class = malloc(sizeof(TGameScreen));
  if (class == 0) {
    return(0);
  }

  /* ----- スクリーンの生成 */
#ifdef SDL2
#if defined(ANDROID) || defined(WINPHONE) || defined(__IOS__)
  class->Screen = SCREEN_SetVideoMode (width, height, SDL_SWSURFACE);
#else
  class->Screen = SCREEN_SetVideoMode (width, height, SDL_SWSURFACE|((sys_fullscreen) ? SCREEN_FULLSCREEN : 0));
#endif
#else
#if defined(X86)
  class->Screen = SDL_SetVideoMode (width, height, depth, SDL_HWSURFACE|SDL_DOUBLEBUF|((sys_fullscreen) ? SDL_FULLSCREEN : 0));
#else
  class->Screen = SDL_SetVideoMode (width, height, depth, SDL_SWSURFACE|((sys_fullscreen) ? SDL_FULLSCREEN : 0));
#endif
#endif
  if (class->Screen == NULL)
    printf ("SDL_SetVideoMode failed: %s\n", SDL_GetError ());

  class->Texture = TGameTexture_Create();
  class->Width = width;
  class->Height = height;
  class->Depth = depth;
  /* --- 初期化 */
  for(i=0; i<SPRITEMAX; i++) {
    class->Sprites[i] = TGameSprite_Create();
  }
  class->timer = 0;

#ifndef VS_WIN8
  SDL_ShowCursor (SDL_DISABLE);
#endif

#ifdef X86
  explode (screen_set, screensize[sys_screensize], 'x');
  TGameScreen_ChangeScreenSize (class, atoi (screen_set[0]), atoi (screen_set[1]));
#endif

  /* --- ポインタを返して終了 */
  return(class);
}

void TGameScreen_Destroy(TGameScreen *class)
{
  int  i;

  if (class) {
    /* ----- テクスチャの解放 */
    TGameTexture_Destroy(class->Texture);
    /* ----- スクリーンの解放 */
    SDL_FreeSurface(class->Screen);
    /* ----- スプライトの解放 */
    for(i=0; i<SPRITEMAX; i++) {
      TGameSprite_Destroy(class->Sprites[i]);
    }

    /* ----- インスタンスの破棄 */
    free(class);
  }
}

/* ---------------------------------------- */
/* --- ウィンドウタイトルの設定             */
static const unsigned int burningsmell_icon_32x32_length = 630;

#if defined(X86) || defined(WIN32)
void TGameScreen_SetWMName (char *name) {
#ifndef SDL2
  SDL_WM_SetCaption (name, name);

#if defined(ICON)
#ifdef LIBPAK
  unsigned int size;
  SDL_RWops *rw;

  byte *buffer = extractMem("graphic/icon.tga", openPak("data.pak"), &size);
  if (buffer == NULL)
    return;

  rw = SDL_RWFromMem (buffer, size);
  SDL_WM_SetIcon (IMG_Load_RW (rw, 0), NULL);

  free (buffer);
  SDL_FreeRW (rw);
#else
  SDL_WM_SetIcon (IMG_Load (va ("%s/graphic/icon.tga", BASEDIR)), NULL);
#endif
#endif
#endif
}
#endif

/* ---------------------------------------- */
/* --- フレームタイミングによる全描画       */
void TGameScreen_DispScreen(TGameScreen *class)
{
  int  i;

  /* --- スプライト描画 */
  for(i=0; i<SPRITEMAX; i++) {
    if ((class->Sprites[i]->DispSw == TRUE) &&
	(class->Sprites[i]->Texture != NULL)) {
      /* --- スプライトの描画 */
      if ((class->Sprites[i]->zoomx == 1.0) &&
	  (class->Sprites[i]->zoomy == 1.0) &&
	  (class->Sprites[i]->rotation_z == 0.0)) {
	/* 回転拡大縮小なし、単純矩形コピー */
	Render(class, class->Sprites[i]);
      }
      else {
	/* 回転拡大縮小あり */
	RenderZoomRot(class, class->Sprites[i]);
      }
    }
  }
}

void TGameScreen_RefreshScreen(TGameScreen *class) {
#if defined(X86) || defined(GEKKO)
  SDL_Surface* backbuffer;
  SDL_Rect srcrect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
  SDL_Rect dstrect = { 0, 0, class->Width, class->Height };
#endif

  /* -- 画面更新 */
  TGameScreen_DispScreen (class);

  FontDraw (class);

// Other devices have Problem with that...
#ifdef SDL2
  SCREEN_Flip ();
#else
#if defined(X86) || defined(GEKKO)
  backbuffer = SDL_DisplayFormat (class->Screen);

  SDL_SoftStretch (backbuffer, &srcrect, class->Screen, &dstrect);

  /* --- ‘S?N???A */
  SDL_Flip (class->Screen);

  SDL_FreeSurface (backbuffer);
#else
  SDL_UpdateRect (class->Screen, 0, 0, class->Width, class->Height);
  SDL_FillRect (class->Screen, 0, 0x00000000); // Make Screen Black (PSP FIX)
#endif
#endif

// Update Screenshot Surface...
  screenshot = class->Screen;
}

#ifdef MOVIE
void TGameScreen_RefreshMovieScreen () {
	extern TGameScreen  *scr;

#if defined(X86) || defined(GEKKO)
  SDL_Surface* backbuffer;
  SDL_Rect srcrect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
  SDL_Rect dstrect = { 0, 0, scr->Screen->w, scr->Screen->h };
#endif

// Other devices have Problem with that...
#ifdef SDL2
	SCREEN_Flip ();
#else
#if defined(X86) || defined(GEKKO)
	backbuffer = SDL_DisplayFormat (scr->Screen);

	SDL_SoftStretch (backbuffer, &srcrect, scr->Screen, &dstrect);

	/* --- ‘S?N???A */
	SDL_Flip (scr->Screen);

	SDL_FreeSurface (backbuffer);
#else
	SDL_UpdateRect (scr->Screen, 0, 0, scr->Screen->w, scr->Screen->h);
	SDL_FillRect (scr->Screen, 0, 0x00000000); // Make Screen Black (PSP FIX)
#endif
#endif
}
#endif

/*
  TGameScreen_ChangeScreenSize

  Note: TGameScreen_ChangeScreenSize (class, 400, 300);
*/
void TGameScreen_ChangeScreenSize (TGameScreen *class, int w, int h) {
  class->Width = w, class->Height = h;

  SDL_FreeSurface (class->Screen);
#ifdef SDL2
  class->Screen = SCREEN_SetVideoMode (w, h, SDL_SWSURFACE|((sys_fullscreen) ? SCREEN_FULLSCREEN : 0));
#else
  class->Screen = SDL_SetVideoMode (w, h, class->Depth, SDL_HWSURFACE|SDL_DOUBLEBUF|((sys_fullscreen) ? SDL_FULLSCREEN : 0));
#endif
  if (class->Screen == NULL)
    printf ("SDL_SetVideoMode failed: %s\n", SDL_GetError ());
}

/* ---------------------------------------- */
/* --- テクスチャをロードする               */

void TGameScreen_LoadTexture(TGameScreen *class,
			     int num,
			     char *filename)
{
  TGameTexture_Load(class->Texture, num, filename, class->Screen, TRUE);
}

void TGameScreen_LoadTexturePure(TGameScreen *class,
				 int num,
				 char *filename)
{
  TGameTexture_Load(class->Texture, num, filename, class->Screen, FALSE);
}

/* ---------------------------------------- */
/* --- スプライトを渡す                     */
TGameSprite *TGameScreen_GetSprite(TGameScreen *class, int id)
{
  if (id < 0) return(0);
  if (id > SPRITEMAX) return(0);

  return(class->Sprites[id]);
}


/* ---------------------------------------- */
/* --- テクスチャを渡す                     */
SDL_Surface *TGameScreen_GetTexture(TGameScreen *class, int id)
{
  return(TGameTexture_GetTexture(class->Texture, id));
}


/* ---------------------------------------- */
/* --- スプライトの表示(画面転送)           */
void Render(TGameScreen *class, TGameSprite *spr)
{
  unsigned char org_alpha;
  SDL_Rect rect1, rect2;

  /* --- 表示スイッチ */
  if (spr->DispSw == FALSE)
    return;
  if (spr->Texture == NULL)
    return;

  rect1.w = spr->w;
  rect1.h = spr->h;
  rect2.w = spr->w;
  rect2.h = spr->h;
  rect1.x = spr->tx;
  rect1.y = spr->ty;
  rect2.x = spr->x;
  rect2.y = spr->y;

#ifndef SDL2
  org_alpha = spr->Texture->format->alpha;
  if (org_alpha != spr->alpha) {
    if (spr->alpha < 255) {
      SDL_SetAlpha(spr->Texture, SDL_SRCALPHA, spr->alpha);
    }
    else {
      SDL_SetAlpha(spr->Texture, 0, 0);
    }
  }
#endif

  SDL_BlitSurface(spr->Texture, &rect1, class->Screen, &rect2);

#ifndef SDL2
  if (org_alpha != spr->alpha) {
    SDL_SetAlpha(spr->Texture, SDL_SRCALPHA, org_alpha);
  }
#endif
}


/* ---------------------------------------- */
/* --- スプライトの表示(回転拡大縮小あり) */
void RenderZoomRot(TGameScreen *class, TGameSprite *spr)
{
  SDL_Surface  *after;
  SDL_Rect  rect1, rect2;
  int  pp_x, pp_y;
  int  pp_dw_x, pp_dh_x, pp_dw_y, pp_dh_y;
  int  fw, fh, rzoomx, rzoomy;
  int  max_w, max_h, max_w_int, max_h_int;
  int  step_x, step_y;
  int  rot;
  int  loop_x, loop_y;
  int  ix, iy;
  unsigned long  *dstpix, *srcpix;
    
  /* --- 表示スイッチ */
  if (spr->DispSw == FALSE) return;
  if (spr->Texture == NULL) return;

  /* --- 回転拡大前処理 */
  rot = (int)spr->rotation_z;
  rot %= 360;
  /* ピクセルステップ */
  rzoomx = (1024*1024) / (int)(spr->zoomx * 1024);
  rzoomy = (1024*1024) / (int)(spr->zoomy * 1024);
  pp_dw_x = (rzoomx * sprite_cos[rot]) / 1024;
  pp_dh_x = (rzoomx * sprite_sin[rot]) / 1024;
  pp_dw_y = -(rzoomy * sprite_sin[rot]) / 1024;
  pp_dh_y = (rzoomy * sprite_cos[rot]) / 1024;
  /* 回転後の画像の大きさ */
  if (rot != 0) {
    max_w = (spr->w * (int)(spr->zoomx * 1448));
    max_h = (spr->h * (int)(spr->zoomy * 1448));
  }
  else {
    max_w = spr->w * (int)(spr->zoomx * 1024);
    max_h = spr->h * (int)(spr->zoomy * 1024);
  }
  if (max_w > max_h) max_h = max_w;
  if (max_h > max_w) max_w = max_h;
  max_w_int = max_w >> 10;
  max_h_int = max_h >> 10;
  after = SDL_CreateRGBSurface(SDL_SWSURFACE,
			       max_w_int, max_h_int, 16,
			       DRmask, DGmask, DBmask, DAmask);
  SDL_FillRect(after, 0, 0x00000000);
  /* 開始点 */
  fw = -(max_w_int) * rzoomx / 2;
  fh = -(max_h_int) * rzoomy / 2;
  pp_x = ((fw * sprite_cos[rot]) - (fh * sprite_sin[rot])) / 1024;
  pp_y = ((fw * sprite_sin[rot]) + (fh * sprite_cos[rot])) / 1024;
  pp_x += (spr->w >> 1) * 1024;
  pp_y += (spr->h >> 1) * 1024;

  /* --- 回転転送処理 */
  dstpix = after->pixels;
  for(loop_y=0; loop_y<max_h_int; loop_y++) {
    step_x = pp_x;
    step_y = pp_y;
    for(loop_x=0; loop_x<max_w_int; loop_x++) {
      if ((step_x >= 0) && (step_x < (spr->w << 10)) &&
	  (step_y >= 0) && (step_y < (spr->h << 10))) {
	ix = step_x >> 10;
	iy = step_y >> 10;
	srcpix = (unsigned long *)spr->Texture->pixels;
	srcpix += (spr->tx + ix + ((spr->ty + iy) * spr->Texture->w));
	*dstpix = *srcpix;
      }
      else {
	*dstpix = 0;
      }
      step_x += pp_dw_x;
      step_y += pp_dh_x;
      dstpix += 1;
    }
    pp_x += pp_dw_y;
    pp_y += pp_dh_y;
  }

  /* --- スクリーンに描画 */
  rect1.w = after->w;
  rect1.h = after->h;
  rect2.w = after->w;
  rect2.h = after->h;
  rect1.x = 0;
  rect1.y = 0;
  rect2.x = spr->x + (spr->w / 2) - (after->w / 2);
  rect2.y = spr->y + (spr->h / 2) - (after->h / 2);
  SDL_BlitSurface(after, &rect1, class->Screen, &rect2);
  /* あとしまつ */
  SDL_FreeSurface(after);
}

void TGameScreen_ScreenShot () {
	char filename[80];
	int i;

	strcpy (filename, "tt_shot00.bmp");
	for (i = 0; i <= 99; i++) {
		filename[7] = i / 10 + '0';
		filename[8] = i % 10 + '0';

		if (open (filename, 0x0800) == -1) {
			break;
		}
	}

	if (i == 100) {
		printf ("TGameScreen_ScreenShot failed: Couldn't create a BMP file\n");

		return;
	}

	SDL_SaveBMP (screenshot, va ("%s/%s", BASEDIR, filename));
}
