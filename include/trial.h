/* ---------------------------------------------------------- */
/*  trial.h                                               */
/* ---------------------------------------------------------- */

/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintaner  Rerorero@fumi.   */
/*                                                        */
/*   trial.h                                          */
/*     �ѥ���Ȥ��Ȥ�⡼��                               */
/*                                                        */
/*--------------------------------------------------------*/

#ifndef TRIAL_H
#define TRIAL_H

/*-------------------------------*/
/* define                        */
/*-------------------------------*/

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

enum TrialStep {
  TInit,
  TReady,
  TGame,
  TMiss,
#ifdef NETWORK
  TNetStatus,
  TNetEnd,
#endif
  TGameover,
  TEnd
};


/*-------------------------------*/
/* struct                        */
/*-------------------------------*/

/* -------------------------------- */
/* --- ���饹���С� */
typedef struct {
  /* - �ƥ��饹 */
  TPuzzleDisp  *super;
  /* - ��������� */
  int  status;

  /* - ��������� */
  TGameScreen  *screen;
  /* - ���ơ��� */
  int  step;
  /* - �������ȥ������ */
  int  readytimer;

  /* - ����� */
  int  game_level;

} TPuzzleTrial, *PTPuzzleTrial;


/* ---------------------------------------------- */
/* --- extern                                  -- */
/* ---------------------------------------------- */

TPuzzleTrial *TPuzzleTrial_Create(TGameScreen *scr, int level);
void TPuzzleTrial_Destroy(TPuzzleTrial *class);

void TPuzzleTrial_LoadTexture(TPuzzleTrial *class);
void TPuzzleTrial_GameMain(TPuzzleTrial *class);
int TPuzzleTrial_GameStat(TPuzzleTrial *class);
void TPuzzleTrial_UserControl(TPuzzleTrial *class);


#endif //PUZ_DISP_H
