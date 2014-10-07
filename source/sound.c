/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintaner  Rerorero@fumi.   */
/*                                                        */
/*   sound.pp                                             */
/*     �ʰץ������(SDL_mixer����)                        */
/*                                                        */
/*--------------------------------------------------------*/
/* -- $Id: sound.pp,v 1.3 2002/08/10 03:05:14 rero2 Exp $ */

/*------------------------------------------------------------- */
/** @file
    @brief		������ɥޥ͡�����E    @author		K.Kunikane (rerofumi)
    @since		Sep.04.2005
    $Revision: 1.1.1.1 $
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
/* define                        */
/*-------------------------------*/

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/*-------------------------------*/
/* local value                   */
/*-------------------------------*/

/* ----- BGM �ȥ�å� */
Mix_Music *SoundTrack;
char *BGMPool[BGMMAX];
char *BGMPoolList[BGMMAX + 1] = {
	"",
	"sound/music/gameover.ogg",
	"sound/music/title.ogg",
	"sound/music/network.ogg",
	"sound/music/stage1.ogg",
	"sound/music/stage2.ogg",
	"sound/music/stage3.ogg",
	"sound/music/stage4.ogg",
	"sound/music/stage5.ogg",
	"sound/music/stage6.ogg",
	"sound/music/freeplay.ogg"
};

/* ----- SE �ȥ�å� */
Mix_Chunk *SEPool[SEMAX];
char *SEPoolList[SEMAX + 1] = {
	"",
	"sound/menu/init.wav",
	"sound/menu/start.wav",
	"sound/game/get_ready.wav",
	"sound/game/clean.wav",
	"sound/game/clear.wav",
	"sound/menu/enter.wav",
	"sound/game/fall.wav",
	"sound/game/null.wav",
	"sound/voice/title.wav",
	"sound/menu/cancel.wav",
	"sound/menu/decision.wav",
	"sound/menu/move.wav",
	"sound/voice/bvs.wav",
	"sound/voice/singleplayer.wav",
	"sound/voice/multiplayer.wav",
	"sound/voice/game_settings.wav",
	"sound/voice/game_rules.wav",
	"sound/voice/quit.wav",
	"sound/voice/ready.wav",
	"sound/voice/see_you_later.wav",
	"sound/voice/bye_bye.wav",
	"sound/voice/new_level.wav",
	"sound/voice/new_stage.wav",
	"sound/voice/gameover.wav",
	"sound/menu/connect.wav",
	"sound/game/talk.wav"
};

/* ----- ������ɥ����ͥ�EΥȥ�å� */
int  track;

/* ----- �������ͭ���ե饰 */
int  SoundEnable;

/* -------------------------------------------------------------- */
/* --- �������                                                   */
/* -------------------------------------------------------------- */

/* ---------------------------------------- */
/* --- ������ɤν鴁Eȥ�E����ɤ߹���       */
/* ---------------------------------------- */

void  SoundInit(void)
{
  int  i, ret;
#ifdef LIBPAK
  unsigned int size;
  byte *buffer;
#endif

  track = 0;
  SoundEnable = TRUE;

  if (sys_sound == 0)
    return;

  /* ----- SDL_mixer �Υ����ץ�E*/
  ret = Mix_OpenAudio (SND_FREQ, SND_FORMAT, SND_CHAN, SND_SAMPLE);
  if (ret != 0) {
    SoundEnable = FALSE;
    printf ("Mix_OpenAudio failed: %s\n", Mix_GetError ());
    return;
  }

  SoundVolume ();

  /* ----- ���󥯥�E� */
  for (i = 0; i < BGMMAX; i++) {
    BGMPool[i] = va ("%s/%s", BASEDIR, BGMPoolList[i]);
  }

  for (i = 0; i < SEMAX; i++) {
#ifdef LIBPAK
    buffer = extractMem(SEPoolList[i], openPak("data.pak"), &size);
    if (buffer == NULL)
      return;

    SEPool[i] = Mix_LoadWAV_RW (SDL_RWFromMem (buffer, size), 0);
#else
    SEPool[i] = Mix_LoadWAV (va ("%s/%s", BASEDIR, SEPoolList[i]));
#endif
    if (SEPool[i] == NULL)
      printf ("Mix_LoadWAV failed: Cannot open %s\n", SEPoolList[i]);
  }
}


/* ---------------------------------------- */
/* --- ������ɤβ�ʁE                      */
/* ---------------------------------------- */
void  SoundFree(void)
{
  int i = 0;

  if (SoundEnable == FALSE)
    return;

  /* ----- �ݻ�������ɤβ�ʁE*/
  if (SoundTrack != 0) {
    Mix_HaltMusic();
    Mix_FreeMusic(SoundTrack);
    SoundTrack = 0;
  }

  /* ----- SE�����ͥ�E���� */
  for(i=0; i<8; i++)
    Mix_HaltChannel(i);

  for (i = 0; i < BGMMAX; i++)
    BGMPool[i] = BGMPoolList[i] = 0;

  /* ----- SE chank �β�ʁE*/
  for(i=0; i<SEMAX; i++) {
    if (SEPool[i] != 0) {
      Mix_FreeChunk(SEPool[i]);
      SEPool[i] = 0;
    }
  }

  /* ----- SDL_Mixer �Υ������� */
  Mix_CloseAudio();
}


/* ---------------------------------------- */
/* --- BGM �Υ�E�������                     */
/* ---------------------------------------- */
void  SoundMusic(int req)
{
#ifdef LIBPAK
  unsigned int size;
  byte *buffer;
#endif
  if (SoundEnable == FALSE) return;

  /* --- �ϰϥ����å� */
  if (req < 0) return;
  if (req >= BGMMAX) return;
  if (BGMPool[req][0] == 0) return;

  /* ----- ����EޤǤα������ */
  if (SoundTrack != 0) {
    Mix_HaltMusic();
    Mix_FreeMusic(SoundTrack);
    SoundTrack = 0;
  }

  /* ----- BGM ���ճ��� */
#ifdef LIBPAK
  buffer = extractMem(BGMPool[req], openPak("data.pak"), &size);
  if (buffer == NULL)
    return;

#if defined(VS_WIN8)
  SoundTrack = Mix_LoadMUS_RW (SDL_RWFromMem (buffer, size));
#else
  SoundTrack = Mix_LoadMUS_RW (SDL_RWFromMem (buffer, size), 0);
#endif
#else
  SoundTrack = Mix_LoadMUS(BGMPool[req]);
#endif
  if (SoundTrack == NULL) {
    printf ("Mix_LoadMUS failed: Cannot open %s\n", BGMPool[req]);
    SoundTrack = 0;
    return;
  }

  Mix_PlayMusic(SoundTrack, -1);
}

/* ---------------------------------------- */
/* --- ����E֤��ʤ� BGM �Υ�E�������        */
/* ---------------------------------------- */
void  SoundMusicOneshot(int  req)
{
#ifdef LIBPAK
  unsigned int size;
  byte *buffer;
#endif
  if (SoundEnable == FALSE) return;

  /* --- �ϰϥ����å� */
  if (req < 0) return;
  if (req >= BGMMAX) return;
  if (BGMPool[req][0] == 0) return;

  /* ----- ����EޤǤα������ */
  if (SoundTrack != 0) {
    Mix_HaltMusic();
    Mix_FreeMusic(SoundTrack);
    SoundTrack = 0;
  }

  /* ----- BGM ���ճ��� */
#ifdef LIBPAK
  buffer = extractMem(BGMPool[req], openPak("data.pak"), &size);
  if (buffer == NULL)
    return;

#if defined(VS_WIN8)
  SoundTrack = Mix_LoadMUS_RW (SDL_RWFromMem (buffer, size));
#else
  SoundTrack = Mix_LoadMUS_RW (SDL_RWFromMem (buffer, size), 0);
#endif
#else
  SoundTrack = Mix_LoadMUS(BGMPool[req]);
#endif
  if (SoundTrack == NULL) {
    printf ("Mix_LoadMUS failed: Cannot open %s\n", BGMPool[req]);
    SoundTrack = 0;
    return;
  }

  Mix_PlayMusic(SoundTrack, 1);
}

/* ---------------------------------------- */
/* --- SE �Υ�E�������                      */
/* ---------------------------------------- */
void  SoundSE(int req)
{
  if (SoundEnable == FALSE) return;

  /* --- �ϰϥ����å� */
  if (req < 0) return;
  if (req >= BGMMAX) return;
  if (SEPool[req] == 0) return;

  /* ----- SE ȯ�� */
  Mix_PlayChannel(track, SEPool[req], 0);
  track = (track + 1) % 8;
}

/* ---------------------------------------- */
/* --- BGM �ȥ�å������                   */
/* ---------------------------------------- */
void  SoundMusicStop(void)
{
  if (SoundEnable == FALSE) return;

  Mix_HaltMusic();
}

/* ---------------------------------------- */
/* --- SE �������                          */
/* ---------------------------------------- */
void  SoundSEStop(void)
{
  int i = 0;

  if (SoundEnable == FALSE)
    return;

  /* ----- SE�����ͥ�E���� */
  for(i=0; i<8; i++)
    Mix_HaltChannel(i);
}

/* ---------------------------------------- */
/* --- Volume�ͤ���āE                     */
/* ---------------------------------------- */
void  SoundVolume (void)
{
  int i = 0;

  for(i=0; i < 8; i++)
    Mix_Volume(i, sys_se_volume);
  Mix_VolumeMusic(sys_bgm_volume);
}
