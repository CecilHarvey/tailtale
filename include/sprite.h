/* ---------------------------------------------------------- */
/*  sprite.h                                              */
/* ---------------------------------------------------------- */

/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintaner  Rerorero@fumi.   */
/*                                                        */
/*   sprite.h                                         */
/*     スプライト管理構造体                               */
/*                                                        */
/*--------------------------------------------------------*/

#ifndef SPRITE_H
#define SPRITE_H

/*-------------------------------*/
/* define                        */
/*-------------------------------*/

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/*-------------------------------*/
/* struct                        */
/*-------------------------------*/

typedef struct {
  int  TextureId;
  /* - public */
  int  DispSw;
  int  x, y;
  int  w, h;
  int  tx, ty;
  float  zoomx, zoomy;
  float  rotation_z;
  unsigned char  alpha;
  SDL_Surface  *Texture;
} TGameSprite, *PTGameSprite;

/* ---------------------------------------------- */
/* --- extern                                  -- */
/* ---------------------------------------------- */

TGameSprite *TGameSprite_Create(void);
void TGameSprite_Destroy(TGameSprite *class);

void TGameSprite_SetTextureDirect(TGameSprite *class,
				  int  texture_id,
				  SDL_Surface *bitmap);


#endif //SPRITE_H
