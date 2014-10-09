/*
	SDL puzzle project
*/

#ifndef _DEFAULT_H_
#define _DEFAULT_H_

/*-------------------------------*/
/* define                        */
/*-------------------------------*/

#define VERSION "v1.0.1.0"

#if defined(ANDROID) || defined(X86) || defined(PSP) || defined(GEKKO) || defined(WINPHONE)
	#define TAILTALE_HD 1
#endif

#ifndef X86 /* We have Keyboard support... */
#define OSK 1
#endif

#if defined(ANDROID)
#define BASEDIR "/sdcard/tailtale/"
#elif defined (MACOSX)
#define BASEDIR "./data"
#elif defined (__IOS__)
#define BASEDIR "./data"
#else
#define BASEDIR "."
#endif

/*-------------------------------*/
/* include                       */
/*-------------------------------*/

// System
#include <math.h>
#if defined(__APPLE__)
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#ifdef NETWORK
#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#elif WINPHONE
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#ifdef ANDROID
#include <signal.h>
#else
#include <sys/signal.h>
#endif
#include <sys/select.h>
#include <netdb.h>
#endif
#endif
#ifdef PSP
#include <pspkernel.h>
#include <pspsdk.h>
#include <psputility_netmodules.h>
#include <psputility_netparam.h>
#include <pspwlan.h>
#include <pspnet.h>
#include <pspnet_apctl.h>
#endif

// SDL
#ifdef WINPHONE
#include <SDL/SDL.h>
#include <SDL/SDL_endian.h>
#include "port.h" // SDL2 Link for SDL 1.2
#include <SDL/SDL_audio.h>
#include <SDL/SDL_image.h> // LibSDL_image
#include <SDL/SDL_main.h>
#include <SDL/SDL_mixer.h> // LibSDL_mixer
#include <SDL/SDL_ttf.h> // LibSDL_ttf
#else
#include <SDL.h>
#include <SDL_endian.h>
#if defined(SDL2)
#include "port.h" // SDL2 Link for SDL 1.2
#endif
#include <SDL_audio.h>
#include <SDL_image.h> // LibSDL_image
#include <SDL_main.h>
#include <SDL_mixer.h> // LibSDL_mixer
#include <SDL_ttf.h> // LibSDL_ttf
#endif
#ifdef MOVIE
#include "theoraplay.h"
#endif

// Nintendo DS(i) & Gamecube/WII
#if defined(GEKKO)
#include <fat.h>
#endif

#ifdef LIBPAK
#include "pak.h" // LibPAK v0.3
#endif

// Video
#include "texture.h"
#include "sprite.h"
#include "screen.h"

// Engine
#include "base.h"
#include "disp.h"
#include "trial.h"

// Main
#include "main.h"

// Menu
#include "menu.h"

// Misc
#include "common.h"
#include "font.h"
#include "input.h"
#ifdef NETWORK
#include "network.h"
#endif
#include "sound.h"

#endif
