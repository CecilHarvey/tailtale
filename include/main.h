/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintainer  Rerorero@fumi.  */
/*      C-SDL convert     2nd Maintainer  rerofumi.       */
/*                                                        */
/*   main.h                                           */
/*     ¬Á∏µ§Œ•·•§•Û•ÅEº•¡•Û§»§Ω§ŒµØ∆∞ΩËÕ˝                 */
/*                                                        */
/*--------------------------------------------------------*/
/* -- $Id: main.pp,v 1.6 2002/08/10 03:05:14 rero2 Exp $ */

#ifndef MAIN_H
#define MAIN_H

/* ------------------------------------------------ */
/* --- define                                       */
/* ------------------------------------------------ */

/* ----- ≤ËÃÃ•µ•§•∫¿ﬂƒÅEÕ */
#if defined(ANDROID) || defined(X86) || defined(PSP) || defined(GEKKO) || defined(WINPHONE)
#ifdef TAILTALE_HD
  #define  SCREEN_WIDTH   480
  #define  SCREEN_HEIGHT  272
#elif GEKKO // Rikku2000: WII Have prblems with 480x272...
  #define  SCREEN_WIDTH   640
  #define  SCREEN_HEIGHT  480
#else
  #define  SCREEN_WIDTH   320
  #define  SCREEN_HEIGHT  240
#endif
#if defined(ANDROID)
  #define  SCREEN_DEPTH   16
#else
  #define  SCREEN_DEPTH   32
#endif
#else
  #define  SCREEN_WIDTH   320
  #define  SCREEN_HEIGHT  240
  #define  SCREEN_DEPTH   16
#endif

/* ----- •’•ÅEº•‡•ÅEº•»§Œ¿ﬂƒÅE*/
#define FRAME_RATE      72
#define FRAME_SKIP_MAX  6

#endif //MAIN_H
