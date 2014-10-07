/* ---------------------------------------------------------- */
/*  texture.c                                                 */
/* ---------------------------------------------------------- */

/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintaner  Rerorero@fumi.   */
/*                                                        */
/*   texture.c                                            */
/*     テクスチャー管理クラス                             */
/*                                                        */
/*--------------------------------------------------------*/

/*------------------------------------------------------------- */
/** @file
    @brief		テクスチャー管理
    @author		K.Kunikane (rerofumi)
    @since		Sep.19.2005
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
/* local define                  */
/*-------------------------------*/

#if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
 #define RMASK 0xff000000
 #define GMASK 0x00ff0000
 #define BMASK 0x0000ff00
 #define AMASK 0x000000ff
 #define RMASK16 0x0000f800
 #define GMASK16 0x000007c0
 #define BMASK16 0x0000003f
 #define AMASK16 0x00000001
#else
 #define RMASK 0x000000ff
 #define GMASK 0x0000ff00
 #define BMASK 0x00ff0000
 #define AMASK 0xff000000
 #define RMASK16 0x0000001f
 #define GMASK16 0x000003e0
 #define BMASK16 0x00007c00
 #define AMASK16 0x00008000
#endif

/*-------------------------------*/
/* local value                   */
/*-------------------------------*/

/*-------------------------------*/
/* local function                */
/*-------------------------------*/

/* -------------------------------------------------------------- */
/* --- テクスチャ管理クラス                                       */
/* -------------------------------------------------------------- */

/* ---------------------------------------- */
/* --- コンストラクタ・デストラクタ         */
TGameTexture *TGameTexture_Create(void) {
  int i;
  TGameTexture *class;

  class = malloc (sizeof(TGameTexture));
  if (class == NULL)
    return(0);

  for(i=0; i<TEXTUREMAX; i++) {
    class->texture_id = i;
    class->bitmap[i] = NULL;
  }

  return (class);
}

void TGameTexture_Destroy(TGameTexture *class)
{
  int  i;
  if (class == NULL)
    return;

  /* ----- ロード中のテクスチャを解放する */
  for(i=0; i<TEXTUREMAX; i++) {
    if (class->bitmap[i] != NULL) {
      SDL_FreeSurface(class->bitmap[i]);
      class->bitmap[i] = NULL;
    }
  }

  /* ----- インスタンスの解放 */
  free(class);
}

/* ---------------------------------------- */
/* --- テクスチャーの読み込み、登録         */
/* ---------------------------------------- */
void TGameTexture_Load(TGameTexture *class,
		       int num,
		       char *filename,
                       SDL_Surface *GameScreen,
                       int preconv)
{
  SDL_Surface  *plane;
#ifdef ANDROID
  char buffer_tex[64];
#endif
#ifdef LIBPAK
  unsigned int size;
  byte *buffer;
#endif

  if (class == NULL)
    return;

  /* ----- ?e?N?X?`??”O?†?a?s?3???A???c?I?1 */
  if (num < 0) return;
  if (num > TEXTUREMAX) return;

  /* ----- ?u?E?e?N?X?`???a???A???c‰d?u */
#if 0 // Crash Fix let Reload textures and overwrite it.
  if (class->bitmap[num] != NULL) {
    SDL_FreeSurface(class->bitmap[num]);
    class->bitmap[num] = NULL;
  }
#endif

  /* ----- ?e?N?X?`???[?I“C?Y???Y */
#ifdef LIBPAK
  buffer = extractMem (filename, openPak("data.pak"), &size);
  if (buffer == NULL)
    return;

  plane = IMG_LoadTGA_RW (SDL_RWFromMem (buffer, size+1));
#else
  plane = IMG_Load (va ("%s/%s", BASEDIR, filename));
#endif
  if (plane == NULL) {
    class->bitmap[num] = NULL;
    // printf ("IMG_Load failed: Cannot open %s\n", filename);
	printf ("%s\n", SDL_GetError ()); 
    return;
  }

  if (preconv == TRUE) {
    class->bitmap[num] = SDL_ConvertSurface (plane, GameScreen->format, SDL_SWSURFACE);
    if (plane != NULL)
      SDL_FreeSurface (plane);
  } else
    class->bitmap[num] = plane;
}

/* ---------------------------------------- */
/* --- テクスチャーの渡し                   */
/* ---------------------------------------- */
SDL_Surface  *TGameTexture_GetTexture (TGameTexture *class, int index) {
  if (class == NULL)
    return (NULL);

  return (class->bitmap[index]);
}
