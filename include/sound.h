/* ---------------------------------------------------------- */
/*  sound.h                                                   */
/* ---------------------------------------------------------- */

/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintaner  Rerorero@fumi.   */
/*                                                        */
/*   sound.h                                              */
/*     音声マネージャ                                     */
/*                                                        */
/*--------------------------------------------------------*/
/* -- $Id:  $ */


/*------------------------------------------------------------- */
/** @file
    @brief		音声マネージャ
    @author		K.Kunikane (rerofumi)
    @since		Sep.04.2005
    $Revision: 1.1.1.1 $
*/
/*-----------------------------------------------------
 Copyright (C) 2002,2005 rerofumi <rero2@yuumu.org>
 All Rights Reserved.
 ------------------------------------------------------*/


#ifndef SOUND_H
#define SOUND_H

/*-------------------------------*/
/* include                       */
/*-------------------------------*/

/*-------------------------------*/
/* define                        */
/*-------------------------------*/

/* --- BGM の管理最大数 */
#define BGMMAX  32

/* --- SE の管理最大数 */
#define SEMAX   64

/* --- VOUME の最大値 */
#ifdef __GP2X__
#define VOLUMEDEFAULT  96
#else
#define VOLUMEDEFAULT  MIX_MAX_VOLUME
#endif

#define SND_FREQ	22050 // Wii use 32khz or 48khz
#if defined(ANDROID)
#define SND_FORMAT	AUDIO_S16SYS
#else
#define SND_FORMAT	MIX_DEFAULT_FORMAT
#endif
#define SND_CHAN	2
#if defined(ANDROID)
#define SND_SAMPLE	2048
#else
#define SND_SAMPLE	4096
#endif

/*-------------------------------*/
/* struct                        */
/*-------------------------------*/



/* ---------------------------------------------- */
/* --- extern                                  -- */
/* ---------------------------------------------- */

void  SoundInit(void);
void  SoundFree(void);
void  SoundMusic(int req);
void  SoundMusicOneshot(int req);
void  SoundSE(int req);
void  SoundMusicStop(void);
void  SoundSEStop(void);
void  SoundVolume(void);

#endif //SOUND_H

