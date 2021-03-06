/* ---------------------------------------------------------- */
/*  disp.h                                                */
/* ---------------------------------------------------------- */

/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintaner  Rerorero@fumi.   */
/*                                                        */
/*   disp.h                                           */
/*     パズル画面表示管理                                 */
/*                                                        */
/*--------------------------------------------------------*/

#ifndef DISP_H
#define DISP_H

/*-------------------------------*/
/* define                        */
/*-------------------------------*/

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* --- 表示関係の定数 */
#define OBJMAX   1024 // dont change!
#define TEXMAX   5
#define FIELDTOP 4
#define KIRAMAX  36

enum GameMode {
  MODE_CHALLENGE,
  MODE_1P,
  MODE_2P
};

/*-------------------------------*/
/* struct                        */
/*-------------------------------*/

/* -------------------------------- */
/* --- きらきらエフェクト */
typedef struct {
  /* - 表示スイッチ */
  int  DispSw;
  /* - 表示オブジェクトの保持 */
  TGameSprite *obj;
  /* - 表示時間タイマー */
  int timer;
  /* - 消える時間 */
  int timer_end;
  /* - 表示位置 */
  int  x;
  int  y;
  /* - 移動量 */
  int  dx;
  int  dy;
} Kira, *PKira;

/* -------------------------------- */
/* --- 背景のタヌキ座標 */
typedef struct {
  int  x, y;
} TanuPos, PTanuPos;


/* -------------------------------- */
/* --- クラスメンバー */
typedef struct {
  /* - 親クラス */
  TPuzzleBase  *super;
  /* - ゲームモード */
  int  puz_mode;
  /* - 表示位置指定 */
  int  fieldpos_x;
  /* - 表示用オブジェクトポインター */
  TGameSprite  *obj[OBJMAX];
  /* - テクスチャーポインター */
  SDL_Surface  *texture[TEXMAX];
  /* --- private */
  /* - きらきらエフェクト */
  Kira  kirakira[KIRAMAX];
  /* - タヌキ表示 */
  TanuPos  tanuki[4];
  /* - スクリーンの保持 */
  TGameScreen  *screen;
  /* - 表示用タイマー */
  int  disptimer;
} TPuzzleDisp, *PTPuzzleDisp;


/* ---------------------------------------------- */
/* --- extern                                  -- */
/* ---------------------------------------------- */

TPuzzleDisp *TPuzzleDisp_Create(int  mode, int level, TGameScreen *scr);
void TPuzzleDisp_Destroy(TPuzzleDisp *class);

void TPuzzleDisp_DispField(TPuzzleDisp *class);
void TPuzzleDisp_DispReady(TPuzzleDisp *class, int time);
void TPuzzleDisp_DispGameover(TPuzzleDisp *class, int time);
void TPuzzleDisp_DispClear(TPuzzleDisp *class);
void TPuzzleDisp_KiraRequest(TPuzzleDisp *class, int x, int y, int l);

#ifdef NETWORK
void TPuzzleDisp_DispNetStatus(TPuzzleDisp *class, int time, int status);
#endif

#ifdef TOUCH
void DispTouch (TPuzzleDisp *class);
#endif

#endif //DISP_H
