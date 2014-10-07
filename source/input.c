/* ---------------------------------------------------------- */
/*  input.c                                                   */
/* ---------------------------------------------------------- */

/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintaner  Rerorero@fumi.   */
/*                                                        */
/*   input.c                                              */
/*     ∆˛Œœ•ﬁ•Õ°º•∏•ÅE                                    */
/*                                                        */
/*--------------------------------------------------------*/
/* -- $Id:  $ */


/*------------------------------------------------------------- */
/** @file
    @brief		∆˛Œœ•ﬁ•Õ°º•∏•ÅE    @author		K.Kunikane (rerofumi)
    @since		Jul.27.2005
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

#ifdef CAANOO
#define GP2X_BUTTON_UP              (11)
#define GP2X_BUTTON_DOWN            (12)
#define GP2X_BUTTON_LEFT            (13)
#define GP2X_BUTTON_RIGHT           (14)
#define GP2X_BUTTON_UPLEFT          (15)
#define GP2X_BUTTON_UPRIGHT         (16)
#define GP2X_BUTTON_DOWNLEFT        (17)
#define GP2X_BUTTON_DOWNRIGHT       (18)
#define GP2X_BUTTON_CLICK           (10)
#define GP2X_BUTTON_A               (0)
#define GP2X_BUTTON_B               (2)
#define GP2X_BUTTON_X               (1)
#define GP2X_BUTTON_Y               (3)
#define GP2X_BUTTON_L               (4)
#define GP2X_BUTTON_R               (5)
#define GP2X_BUTTON_START           (8)
#define GP2X_BUTTON_SELECT          (9)
#define GP2X_BUTTON_HOME            (6)
#define GP2X_BUTTON_HOLD            (7)
#else
#define GP2X_BUTTON_UP              (0)
#define GP2X_BUTTON_DOWN            (4)
#define GP2X_BUTTON_LEFT            (2)
#define GP2X_BUTTON_RIGHT           (6)
#define GP2X_BUTTON_UPLEFT          (1)
#define GP2X_BUTTON_UPRIGHT         (7)
#define GP2X_BUTTON_DOWNLEFT        (3)
#define GP2X_BUTTON_DOWNRIGHT       (5)
#define GP2X_BUTTON_CLICK           (18)
#define GP2X_BUTTON_A               (12)
#define GP2X_BUTTON_B               (13)
#define GP2X_BUTTON_X               (14)
#define GP2X_BUTTON_Y               (15)
#define GP2X_BUTTON_L               (10)
#define GP2X_BUTTON_R               (11)
#define GP2X_BUTTON_START           (8)
#define GP2X_BUTTON_SELECT          (9)
#define GP2X_BUTTON_VOLUP           (16)
#define GP2X_BUTTON_VOLDOWN         (17)
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/*-------------------------------*/
/* local value                   */
/*-------------------------------*/

/* ----- •§•Ÿ•Û•» */
SDL_Event  event;

/* ----- •Ê°º•∂°º§Œ•≤°º•‡¡‡∫ÅE*/
SDL_Joystick  *JoyPtr[JOY_NUM_MAX];
unsigned long  JoyKey[JOY_NUM_MAX];
unsigned long  JoyStick[JOY_NUM_MAX];
unsigned long  JoyKeyPast[JOY_NUM_MAX];
unsigned long  JoyStickPast[JOY_NUM_MAX];

/* ----- •∏•Á•§•π•∆•£•√•Ø§Œæ ÅE*/
/*  §ﬁ§¢°¢∞ÅE˛æ Û§»§∑§∆  */
int JoyStickAxes[JOY_NUM_MAX];
int JoyStickButtons[JOY_NUM_MAX];

/* ----- Ω™Œª•≠°º•’•È•∞ */
int  AppExit;

#ifdef NETWORK
int net_msgstart = 0;
#endif

/* ----- GP2X •≠°º¿ﬂƒÅE≥•Û•’•£•Æ•Â•ÅEº•∑•Á•ÅE*/
int  GpKeySwap;

/* -------------------------------------------------------------- */
/* --- ∆˛Œœ¥…Õ˝                                                   */
/* -------------------------------------------------------------- */

/* ---------------------------------------- */
/* --- •≠°º§Œ»Ω ÃΩËÕ˝                       */
/* ---------------------------------------- */
void  key_read_down(SDL_KeyboardEvent *key)
{
  int l;

  if (key->keysym.sym == SDLK_UP)
    JoyKey[0] = JoyKey[0] | IN_Up;
  if (key->keysym.sym == SDLK_DOWN)
    JoyKey[0] = JoyKey[0] | IN_Down;
  if (key->keysym.sym == SDLK_LEFT)
    JoyKey[0] = JoyKey[0] | IN_Left;
  if (key->keysym.sym == SDLK_RIGHT)
    JoyKey[0] = JoyKey[0] | IN_Right;
#if defined(DINGOO)
  // if (key->keysym.sym == SDLK_TAB) // L
  // if (key->keysym.sym == SDLK_BACKSPACE) // R
  if (key->keysym.sym == SDLK_ESCAPE) // Select
    JoyKey[0] = JoyKey[0] | IN_Button2;
  if (key->keysym.sym == SDLK_RETURN) // Start
    JoyKey[0] = JoyKey[0] | IN_Button1;
  if (key->keysym.sym == SDLK_LCTRL) // A
    JoyKey[0] = JoyKey[0] | IN_Button3;
  if (key->keysym.sym == SDLK_LALT) // B
    JoyKey[0] = JoyKey[0] | IN_Button4;
  if (key->keysym.sym == SDLK_SPACE) // X
    JoyKey[0] = JoyKey[0] | IN_Button6;
  if (key->keysym.sym == SDLK_LSHIFT) // Y
    JoyKey[0] = JoyKey[0] | IN_Button5;
#else
  if (key->keysym.sym == SDLK_F12)
    TGameScreen_ScreenShot ();

#ifdef NETWORK
	if ((network_game) && (net_msgstart == 1)) {
		TNetwork_ChatInput (key);
	} else {
		if (key->keysym.sym == SDLK_ESCAPE) // Select
			JoyKey[0] = JoyKey[0] | IN_Button2;
		if (key->keysym.sym == SDLK_RETURN) // Start
			JoyKey[0] = JoyKey[0] | IN_Button1;

		if ((dispnetwork == 3) || (dispnetwork == 5)) {
			switch (key->keysym.sym) { /* Hardware Keyboard for X86 */
				case SDLK_BACKSPACE:
					if (strlen (osk_buffer))
						osk_buffer[strlen (osk_buffer) - 1] = 0;
					break;

				default:
					if (key->keysym.sym < 32 || key->keysym.sym > 127)
						break;

					l = strlen (osk_buffer);
					if (l < 15) {
						osk_buffer[l + 1] = 0;
						osk_buffer[l] = key->keysym.sym;
					}
					break;
			}
		} else {
			if (key->keysym.sym == SDLK_t) {
				osk_buffer[0] = net_msgprev[0] = '\0';
				net_msgstart = 1; // OpenChat
			}
			if (key->keysym.sym == SDLK_z) // A
				JoyKey[0] = JoyKey[0] | IN_Button3;
			if (key->keysym.sym == SDLK_x) // B
				JoyKey[0] = JoyKey[0] | IN_Button4;
			if (key->keysym.sym == SDLK_c) // X
				JoyKey[0] = JoyKey[0] | IN_Button6;
			if (key->keysym.sym == SDLK_v) // Y
				JoyKey[0] = JoyKey[0] | IN_Button5;
		}
	}
#else
	if (key->keysym.sym == SDLK_ESCAPE) // Select
		JoyKey[0] = JoyKey[0] | IN_Button2;
	if (key->keysym.sym == SDLK_RETURN) // Start
		JoyKey[0] = JoyKey[0] | IN_Button1;
	if (key->keysym.sym == SDLK_z) // A
		JoyKey[0] = JoyKey[0] | IN_Button3;
	if (key->keysym.sym == SDLK_x) // B
		JoyKey[0] = JoyKey[0] | IN_Button4;
	if (key->keysym.sym == SDLK_c) // X
		JoyKey[0] = JoyKey[0] | IN_Button6;
	if (key->keysym.sym == SDLK_v) // Y
		JoyKey[0] = JoyKey[0] | IN_Button5;
#endif
#endif
}

void  key_read_up(SDL_KeyboardEvent *key)
{
  if (key->keysym.sym == SDLK_UP)
    JoyKey[0] = JoyKey[0] & (~(IN_Up));
  if (key->keysym.sym == SDLK_DOWN)
    JoyKey[0] = JoyKey[0] & (~(IN_Down));
  if (key->keysym.sym == SDLK_LEFT)
    JoyKey[0] = JoyKey[0] & (~(IN_Left));
  if (key->keysym.sym == SDLK_RIGHT)
    JoyKey[0] = JoyKey[0] & (~(IN_Right));
#if defined(DINGOO)
  // if (key->keysym.sym == SDLK_TAB) // L
  // if (key->keysym.sym == SDLK_BACKSPACE) // R
  if (key->keysym.sym == SDLK_ESCAPE) // Select
    JoyKey[0] = JoyKey[0] & (~(IN_Button2));
  if (key->keysym.sym == SDLK_RETURN) // Start
    JoyKey[0] = JoyKey[0] & (~(IN_Button1));
  if (key->keysym.sym == SDLK_LCTRL) // A
    JoyKey[0] = JoyKey[0] & (~(IN_Button3));
  if (key->keysym.sym == SDLK_LALT) // B
    JoyKey[0] = JoyKey[0] & (~(IN_Button4));
  if (key->keysym.sym == SDLK_SPACE) // X
    JoyKey[0] = JoyKey[0] & (~(IN_Button6));
  if (key->keysym.sym == SDLK_LSHIFT) // Y
    JoyKey[0] = JoyKey[0] & (~(IN_Button5));
#else
  if (key->keysym.sym == SDLK_ESCAPE) // Select
    JoyKey[0] = JoyKey[0] & (~(IN_Button2));
  if (key->keysym.sym == SDLK_RETURN) // Start
    JoyKey[0] = JoyKey[0] & (~(IN_Button1));
  if (key->keysym.sym == SDLK_z) // A
    JoyKey[0] = JoyKey[0] & (~(IN_Button3));
  if (key->keysym.sym == SDLK_x) // B
    JoyKey[0] = JoyKey[0] & (~(IN_Button4));
  if (key->keysym.sym == SDLK_c) // X
    JoyKey[0] = JoyKey[0] & (~(IN_Button6));
  if (key->keysym.sym == SDLK_v) // Y
    JoyKey[0] = JoyKey[0] & (~(IN_Button5));
#endif
}

#ifdef GEKKO
void  joy_read_button_up_wii(SDL_JoyHatEvent *stick)
{
  if (stick->which < JOY_NUM_MAX) {
   switch(stick->value) {
    case SDL_HAT_LEFT:
      JoyStick[stick->which] = JoyStick[stick->which] | IN_Left;
      break;
    case SDL_HAT_RIGHT:
      JoyStick[stick->which] = JoyStick[stick->which] | IN_Right;
      break;
    case SDL_HAT_UP:
      JoyStick[stick->which] = JoyStick[stick->which] | IN_Up;
      break;
    case SDL_HAT_DOWN:
      JoyStick[stick->which] = JoyStick[stick->which] | IN_Down;
      break;

    case SDL_HAT_CENTERED:
      JoyStick[stick->which] &= (~(IN_Left|IN_Right));
      JoyStick[stick->which] &= (~(IN_Up|IN_Down));
      break;
   }
  }
}
#endif

/* ---------------------------------------- */
/* --- •∏•Á•§•π•∆•£•√•Ø§Œ»Ω ÃΩËÕ˝           */
/* ---------------------------------------- */

void  joy_read_stick(SDL_JoyAxisEvent *stick)
{
  if (stick->which < JOY_NUM_MAX) {
   switch(stick->axis) {
      /* --- ≤£ ˝∏˛ */

    case 0:
     if (GpKeySwap == FALSE) {
      JoyStick[stick->which] &= (~(IN_Left|IN_Right));
      if (stick->value < (-JOY_STICK_DITHER)) {
	   JoyStick[stick->which] = JoyStick[stick->which] | IN_Left;
      }
      if (stick->value > JOY_STICK_DITHER) {
	   JoyStick[stick->which] = JoyStick[stick->which] | IN_Right;
      }
     } else {
      JoyStick[stick->which] &= (~(IN_Button5|IN_Button3));
      if (stick->value < (-JOY_STICK_DITHER)) {
	   JoyStick[stick->which] = JoyStick[stick->which] | IN_Button5;
      }
      if (stick->value > JOY_STICK_DITHER) {
	   JoyStick[stick->which] = JoyStick[stick->which] | IN_Button3;
      }
	 }
     break;

      /* --- Ωƒ ˝∏˛ */
    case 1:
     if (GpKeySwap == FALSE) {
      JoyStick[stick->which] &= (~(IN_Up|IN_Down));
      if (stick->value < (-JOY_STICK_DITHER)) {
	   JoyStick[stick->which] = JoyStick[stick->which] | IN_Up;
      }
      if (stick->value > JOY_STICK_DITHER) {
	   JoyStick[stick->which] = JoyStick[stick->which] | IN_Down;
      }
     } else {
      JoyStick[stick->which] &= (~(IN_Button6|IN_Button4));
      if (stick->value < (-JOY_STICK_DITHER)) {
	   JoyStick[stick->which] = JoyStick[stick->which] | IN_Button6;
      }
      if (stick->value > JOY_STICK_DITHER) {
	   JoyStick[stick->which] = JoyStick[stick->which] | IN_Button4;
      }
	 }
     break;
    }
  }
}


void  joy_read_button_up_pc(SDL_JoyButtonEvent *btn)
{
  if (btn->which < JOY_NUM_MAX) {
#ifdef GEKKO
	// Remote
    if ((btn->button == 0) && (btn->state == SDL_RELEASED)) // A
	  JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button1));
    if ((btn->button == 1) && (btn->state == SDL_RELEASED)) // B
	  JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button2));
    if ((btn->button == 2) && (btn->state == SDL_RELEASED)) // 1
	  JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button3));
    if ((btn->button == 3) && (btn->state == SDL_RELEASED)) // 2
	  JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button4));
    if ((btn->button == 4) && (btn->state == SDL_RELEASED)) // -
	  JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button8)); // Menu Settings
    // if ((btn->button == 5) && (btn->state == SDL_RELEASED)) // +
    // if ((btn->button == 6) && (btn->state == SDL_RELEASED)) // Home

	// Nunchuck support
    if ((btn->button == 7) && (btn->state == SDL_RELEASED)) // Z
	  JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button5));
    if ((btn->button == 8) && (btn->state == SDL_RELEASED)) // C
	  JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button6));

	// Classic Controller support
    if (GpKeySwap == FALSE) {
      if ((btn->button == 9) && (btn->state == SDL_RELEASED)) // A
	    JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button3));
      if ((btn->button == 10) && (btn->state == SDL_RELEASED)) // B
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button4));
      if ((btn->button == 11) && (btn->state == SDL_RELEASED)) // X
	    JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button6));
      if ((btn->button == 12) && (btn->state == SDL_RELEASED)) // Y
	    JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button5));
	} else {
      if ((btn->button == 9) && (btn->state == SDL_RELEASED)) // A
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Right));
      if ((btn->button == 10) && (btn->state == SDL_RELEASED)) // B
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Down));
      if ((btn->button == 11) && (btn->state == SDL_RELEASED)) // X
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Up));
      if ((btn->button == 12) && (btn->state == SDL_RELEASED)) // Y
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Left));
	}
    // if ((btn->button == 13) && (btn->state == SDL_RELEASED)) // L
    // if ((btn->button == 14) && (btn->state == SDL_RELEASED)) // R
    // if ((btn->button == 15) && (btn->state == SDL_RELEASED)) // Zl
    // if ((btn->button == 16) && (btn->state == SDL_RELEASED)) // Zr
    if ((btn->button == 17) && (btn->state == SDL_RELEASED)) // -
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button2));
    if ((btn->button == 18) && (btn->state == SDL_RELEASED)) // +
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button1));
    // if ((btn->button == 19) && (btn->state == SDL_RELEASED)) // Home
#else
    if (GpKeySwap == FALSE) {
      if ((btn->button == 1) && (btn->state == SDL_RELEASED)) // A
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button3));
      if ((btn->button == 0) && (btn->state == SDL_RELEASED)) // B
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button4));
      if ((btn->button == 3) && (btn->state == SDL_RELEASED)) // X
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button5));
      if ((btn->button == 2) && (btn->state == SDL_RELEASED)) // Y
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button6));
    } else {
      if ((btn->button == 1) && (btn->state == SDL_RELEASED)) // A
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Right));
      if ((btn->button == 0) && (btn->state == SDL_RELEASED)) // B
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Down));
      if ((btn->button == 3) && (btn->state == SDL_RELEASED)) // X
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Up));
      if ((btn->button == 2) && (btn->state == SDL_RELEASED)) // Y
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Left));
	}
    // if ((btn->button == 4) && (btn->state == SDL_RELEASED)) // L
    // if ((btn->button == 5) && (btn->state == SDL_RELEASED)) // R
    if ((btn->button == 6) && (btn->state == SDL_RELEASED)) // Select
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button2));
    if ((btn->button == 7) && (btn->state == SDL_RELEASED)) // Start
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button1));
#endif
  }
}

void  joy_read_button_up_gp2x(SDL_JoyButtonEvent *btn)
{
  if (btn->which < JOY_NUM_MAX) {
#ifdef __GP2X__
    if (GpKeySwap == FALSE) {
      if ((btn->button == GP2X_BUTTON_RIGHT) && (btn->state == SDL_RELEASED))
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Right));
      if ((btn->button == GP2X_BUTTON_DOWN) && (btn->state == SDL_RELEASED))
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Down));
      if ((btn->button == GP2X_BUTTON_UP) && (btn->state == SDL_RELEASED))
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Up));
      if ((btn->button == GP2X_BUTTON_LEFT) && (btn->state == SDL_RELEASED))
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Left));
      if ((btn->button == GP2X_BUTTON_A) && (btn->state == SDL_RELEASED)) // A
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button3));
      if ((btn->button == GP2X_BUTTON_B) && (btn->state == SDL_RELEASED)) // B
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button4));
      if ((btn->button == GP2X_BUTTON_X) && (btn->state == SDL_RELEASED)) // X
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button5));
      if ((btn->button == GP2X_BUTTON_Y) && (btn->state == SDL_RELEASED)) // Y
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button6));
    } else {
      if ((btn->button == GP2X_BUTTON_RIGHT) && (btn->state == SDL_RELEASED))
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button3));
      if ((btn->button == GP2X_BUTTON_DOWN) && (btn->state == SDL_RELEASED))
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button4));
      if ((btn->button == GP2X_BUTTON_UP) && (btn->state == SDL_RELEASED))
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button5));
      if ((btn->button == GP2X_BUTTON_LEFT) && (btn->state == SDL_RELEASED))
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button6));
      if ((btn->button == GP2X_BUTTON_A) && (btn->state == SDL_RELEASED)) // A
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Left));
      if ((btn->button == GP2X_BUTTON_B) && (btn->state == SDL_RELEASED)) // B
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Right));
      if ((btn->button == GP2X_BUTTON_X) && (btn->state == SDL_RELEASED)) // X
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Down));
      if ((btn->button == GP2X_BUTTON_Y) && (btn->state == SDL_RELEASED)) // Y
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Up));
	}
    // if ((btn->button == GP2X_BUTTON_L) && (btn->state == SDL_RELEASED))
    // if ((btn->button == GP2X_BUTTON_R) && (btn->state == SDL_RELEASED))
#ifdef CAANOO
    // if ((btn->button == GP2X_BUTTON_HOME) && (btn->state == SDL_RELEASED))
    if ((btn->button == GP2X_BUTTON_START) && (btn->state == SDL_RELEASED)) // Help 1
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button1));
    if ((btn->button == GP2X_BUTTON_SELECT) && (btn->state == SDL_RELEASED)) // Help 2
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button2));
    // if ((btn->button == GP2X_BUTTON_VOLUP) && (btn->state == SDL_RELEASED))
    // if ((btn->button == GP2X_BUTTON_VOLDOWN) && (btn->state == SDL_RELEASED))
#else
    if ((btn->button == GP2X_BUTTON_START) && (btn->state == SDL_RELEASED)) // Start
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button1));
    if ((btn->button == GP2X_BUTTON_SELECT) && (btn->state == SDL_RELEASED)) // Select
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button2));
#endif
    // if ((btn->button == GP2X_BUTTON_CLICK) && (btn->state == SDL_RELEASED))
#endif
  }
}

void  joy_read_button_up_psp(SDL_JoyButtonEvent *btn)
{
  if (btn->which < JOY_NUM_MAX) {
    // if ((btn->button == 4) && (btn->state == SDL_RELEASED)) // L
    // if ((btn->button == 5) && (btn->state == SDL_RELEASED)) // R
    if (GpKeySwap == FALSE) {
      if ((btn->button == 9) && (btn->state == SDL_RELEASED))
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Right));
      if ((btn->button == 6) && (btn->state == SDL_RELEASED))
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Down));
      if ((btn->button == 8) && (btn->state == SDL_RELEASED))
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Up));
      if ((btn->button == 7) && (btn->state == SDL_RELEASED))
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Left));
      if ((btn->button == 1) && (btn->state == SDL_RELEASED)) // Circle (A)
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button3));
      if ((btn->button == 2) && (btn->state == SDL_RELEASED)) // Cross (B)
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button4));
      if ((btn->button == 0) && (btn->state == SDL_RELEASED)) // Triangle (X)
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button5));
      if ((btn->button == 3) && (btn->state == SDL_RELEASED)) // Square (Y)
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button6));
    } else {
      if ((btn->button == 9) && (btn->state == SDL_RELEASED))
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button3));
      if ((btn->button == 6) && (btn->state == SDL_RELEASED))
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button4));
      if ((btn->button == 8) && (btn->state == SDL_RELEASED))
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button5));
      if ((btn->button == 7) && (btn->state == SDL_RELEASED))
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button6));
      if ((btn->button == 1) && (btn->state == SDL_RELEASED)) // Circle (A)
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Right));
      if ((btn->button == 2) && (btn->state == SDL_RELEASED)) // Cross (B)
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Down));
      if ((btn->button == 0) && (btn->state == SDL_RELEASED)) // Triangle (X)
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Up));
      if ((btn->button == 3) && (btn->state == SDL_RELEASED)) // Square (Y)
        JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Left));
	}
    if ((btn->button == 10) && (btn->state == SDL_RELEASED)) // Select
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button2));
    if ((btn->button == 11) && (btn->state == SDL_RELEASED)) // Start
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button1));
  }
}

void  joy_read_button_down_pc(SDL_JoyButtonEvent *btn)
{
  if (btn->which < JOY_NUM_MAX) {
#ifdef GEKKO
	// Remote
    if ((btn->button == 0) && (btn->state == SDL_PRESSED)) // A
	  JoyKey[btn->which] = JoyKey[btn->which] | IN_Button1;
    if ((btn->button == 1) && (btn->state == SDL_PRESSED)) // B
	  JoyKey[btn->which] = JoyKey[btn->which] | IN_Button2;
    if ((btn->button == 2) && (btn->state == SDL_PRESSED)) // 1
	  JoyKey[btn->which] = JoyKey[btn->which] | IN_Button3;
    if ((btn->button == 3) && (btn->state == SDL_PRESSED)) // 2
	  JoyKey[btn->which] = JoyKey[btn->which] | IN_Button4;
    if ((btn->button == 4) && (btn->state == SDL_PRESSED)) // -
	  JoyKey[btn->which] = JoyKey[btn->which] | IN_Button8; // Menu Settings
    // if ((btn->button == 5) && (btn->state == SDL_PRESSED)) // +
    // if ((btn->button == 6) && (btn->state == SDL_PRESSED)) // Home

	// Nunchuck support
    if ((btn->button == 7) && (btn->state == SDL_PRESSED)) // Z
	  JoyKey[btn->which] = JoyKey[btn->which] | IN_Button5;
    if ((btn->button == 8) && (btn->state == SDL_PRESSED)) // C
	  JoyKey[btn->which] = JoyKey[btn->which] | IN_Button6;

	// Classic Controller support
    if (GpKeySwap == FALSE) {
      if ((btn->button == 9) && (btn->state == SDL_PRESSED)) // A
	    JoyKey[btn->which] = JoyKey[btn->which] | IN_Button3;
      if ((btn->button == 10) && (btn->state == SDL_PRESSED)) // B
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button4;
      if ((btn->button == 11) && (btn->state == SDL_PRESSED)) // X
	    JoyKey[btn->which] = JoyKey[btn->which] | IN_Button6;
      if ((btn->button == 12) && (btn->state == SDL_PRESSED)) // Y
	    JoyKey[btn->which] = JoyKey[btn->which] | IN_Button5;
	} else {
      if ((btn->button == 9) && (btn->state == SDL_PRESSED)) // A
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Right;
      if ((btn->button == 10) && (btn->state == SDL_PRESSED)) // B
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Down;
      if ((btn->button == 11) && (btn->state == SDL_PRESSED)) // X
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Up;
      if ((btn->button == 12) && (btn->state == SDL_PRESSED)) // Y
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Left;
	}
    // if ((btn->button == 13) && (btn->state == SDL_PRESSED)) // L
    // if ((btn->button == 14) && (btn->state == SDL_PRESSED)) // R
    // if ((btn->button == 15) && (btn->state == SDL_PRESSED)) // Zl
    // if ((btn->button == 16) && (btn->state == SDL_PRESSED)) // Zr
    if ((btn->button == 17) && (btn->state == SDL_PRESSED)) // -
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button2;
    if ((btn->button == 18) && (btn->state == SDL_PRESSED)) // +
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button1;
    // if ((btn->button == 19) && (btn->state == SDL_PRESSED)) // Home
#else
    if (GpKeySwap == FALSE) {
      if ((btn->button == 1) && (btn->state == SDL_PRESSED)) // A
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button3;
      if ((btn->button == 0) && (btn->state == SDL_PRESSED)) // B
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button4;
      if ((btn->button == 3) && (btn->state == SDL_PRESSED)) // X
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button5;
      if ((btn->button == 2) && (btn->state == SDL_PRESSED)) // Y
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button6;
    } else {
      if ((btn->button == 1) && (btn->state == SDL_PRESSED)) // A
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Right;
      if ((btn->button == 0) && (btn->state == SDL_PRESSED)) // B
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Down;
      if ((btn->button == 3) && (btn->state == SDL_PRESSED)) // X
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Up;
      if ((btn->button == 2) && (btn->state == SDL_PRESSED)) // Y
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Left;
	}
    // if ((btn->button == 4) && (btn->state == SDL_PRESSED)) // L
    // if ((btn->button == 5) && (btn->state == SDL_PRESSED)) // R
    if ((btn->button == 6) && (btn->state == SDL_PRESSED)) // Select
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button2;
    if ((btn->button == 7) && (btn->state == SDL_PRESSED)) // Start
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button1;
#endif
  }
}

void  joy_read_button_down_gp2x(SDL_JoyButtonEvent *btn) {
  if (btn->which < JOY_NUM_MAX) {
#ifdef __GP2X__
    if (GpKeySwap == FALSE) {
      if ((btn->button == GP2X_BUTTON_RIGHT) && (btn->state == SDL_PRESSED))
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Right;
      if ((btn->button == GP2X_BUTTON_DOWN) && (btn->state == SDL_PRESSED))
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Down;
      if ((btn->button == GP2X_BUTTON_UP) && (btn->state == SDL_PRESSED))
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Up;
      if ((btn->button == GP2X_BUTTON_LEFT) && (btn->state == SDL_PRESSED))
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Left;
      if ((btn->button == GP2X_BUTTON_A) && (btn->state == SDL_PRESSED)) // A
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button3;
      if ((btn->button == GP2X_BUTTON_B) && (btn->state == SDL_PRESSED)) // B
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button4;
      if ((btn->button == GP2X_BUTTON_X) && (btn->state == SDL_PRESSED)) // X
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button5;
      if ((btn->button == GP2X_BUTTON_Y) && (btn->state == SDL_PRESSED)) // Y
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button6;
    } else {
      if ((btn->button == GP2X_BUTTON_RIGHT) && (btn->state == SDL_PRESSED))
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button3;
      if ((btn->button == GP2X_BUTTON_DOWN) && (btn->state == SDL_PRESSED))
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button4;
      if ((btn->button == GP2X_BUTTON_UP) && (btn->state == SDL_PRESSED))
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button5;
      if ((btn->button == GP2X_BUTTON_LEFT) && (btn->state == SDL_PRESSED))
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button6;
      if ((btn->button == GP2X_BUTTON_A) && (btn->state == SDL_PRESSED)) // A
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Left;
      if ((btn->button == GP2X_BUTTON_B) && (btn->state == SDL_PRESSED)) // B
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Right;
      if ((btn->button == GP2X_BUTTON_X) && (btn->state == SDL_PRESSED)) // X
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Down;
      if ((btn->button == GP2X_BUTTON_Y) && (btn->state == SDL_PRESSED)) // Y
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Up;
	}
    // if ((btn->button == GP2X_BUTTON_L) && (btn->state == SDL_PRESSED))
    // if ((btn->button == GP2X_BUTTON_R) && (btn->state == SDL_PRESSED))
#ifdef CAANOO
    // if ((btn->button == GP2X_BUTTON_HOME) && (btn->state == SDL_PRESSED))
    if ((btn->button == GP2X_BUTTON_START) && (btn->state == SDL_PRESSED)) // Help 1
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button1;
    if ((btn->button == GP2X_BUTTON_SELECT) && (btn->state == SDL_PRESSED)) // Help 2
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button2;
    // if ((btn->button == GP2X_BUTTON_VOLUP) && (btn->state == SDL_PRESSED))
    // if ((btn->button == GP2X_BUTTON_VOLDOWN) && (btn->state == SDL_PRESSED))
#else
    if ((btn->button == GP2X_BUTTON_START) && (btn->state == SDL_PRESSED)) // Start
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button1;
    if ((btn->button == GP2X_BUTTON_SELECT) && (btn->state == SDL_PRESSED)) // Select
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button2;
#endif
    // if ((btn->button == GP2X_BUTTON_CLICK) && (btn->state == SDL_PRESSED))
#endif
  }
}

void  joy_read_button_down_psp(SDL_JoyButtonEvent *btn)
{
  if (btn->which < JOY_NUM_MAX) {
    // if ((btn->button == 4) && (btn->state == SDL_PRESSED)) // L
    // if ((btn->button == 5) && (btn->state == SDL_PRESSED)) // R
    if (GpKeySwap == FALSE) {
      if ((btn->button == 9) && (btn->state == SDL_PRESSED))
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Right;
      if ((btn->button == 6) && (btn->state == SDL_PRESSED))
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Down;
      if ((btn->button == 8) && (btn->state == SDL_PRESSED))
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Up;
      if ((btn->button == 7) && (btn->state == SDL_PRESSED))
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Left;
      if ((btn->button == 1) && (btn->state == SDL_PRESSED)) // Circle (A)
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button3;
      if ((btn->button == 2) && (btn->state == SDL_PRESSED)) // Cross (B)
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button4;
      if ((btn->button == 0) && (btn->state == SDL_PRESSED)) // Triangle (X)
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button5;
      if ((btn->button == 3) && (btn->state == SDL_PRESSED)) // Square (Y)
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button6;
    } else {
      if ((btn->button == 9) && (btn->state == SDL_PRESSED))
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button3;
      if ((btn->button == 6) && (btn->state == SDL_PRESSED))
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button4;
      if ((btn->button == 8) && (btn->state == SDL_PRESSED))
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button5;
      if ((btn->button == 7) && (btn->state == SDL_PRESSED))
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Button6;
      if ((btn->button == 1) && (btn->state == SDL_PRESSED)) // Circle (A)
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Right;
      if ((btn->button == 2) && (btn->state == SDL_PRESSED)) // Cross (B)
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Down;
      if ((btn->button == 0) && (btn->state == SDL_PRESSED)) // Triangle (X)
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Up;
      if ((btn->button == 3) && (btn->state == SDL_PRESSED)) // Square (Y)
        JoyKey[btn->which] = JoyKey[btn->which] | IN_Left;
	}
    if ((btn->button == 10) && (btn->state == SDL_PRESSED)) // Select
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button2;
    if ((btn->button == 11) && (btn->state == SDL_PRESSED)) // Start
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button1;
  }
}

#ifdef TOUCH
int ingame;

int touch_x, touch_y;

void gettouch () {
	SDL_Finger *finger1, *finger2;
	float f1x, f1y, f2x, f2y;

	while (SDL_PollEvent (&event)) {
		finger1 = SDL_GetTouchFinger (event.tfinger.touchId, 0);
		if (finger1) {
			f1x = SCREEN_WIDTH * finger1->x;
			f1y = SCREEN_HEIGHT * finger1->y;
		}
		finger2 = SDL_GetTouchFinger (event.tfinger.touchId, 1);
		if (finger2) {
			f2x = SCREEN_WIDTH * finger2->x;
			f2y = SCREEN_HEIGHT * finger2->y;
		} else
			touch_x = touch_y = 0;

		if ((f1x + (0) > 32 && f1x < 32 + (64) && f1y + (0) > SCREEN_HEIGHT - 128 && f1y < SCREEN_HEIGHT - 128 + (64)) ||
			(f2x + (0) > 32 && f2x < 32 + (64) && f2y + (0) > SCREEN_HEIGHT - 128 && f2y < SCREEN_HEIGHT - 128 + (64))) // Up
			JoyKey[0] = JoyKey[0] | IN_Up, touch_y = -32;
		else
			JoyKey[0] = JoyKey[0] & (~(IN_Up));

		if ((f1x + (0) > 0 && f1x < 0 + (64) && f1y + (0) > SCREEN_HEIGHT - 96 && f1y < SCREEN_HEIGHT - 96 + (64)) ||
			(f2x + (0) > 0 && f2x < 0 + (64) && f2y + (0) > SCREEN_HEIGHT - 96 && f2y < SCREEN_HEIGHT - 96 + (64))) // Left
			JoyKey[0] = JoyKey[0] | IN_Left, touch_x = -32;
		else
			JoyKey[0] = JoyKey[0] & (~(IN_Left));

		if ((f1x + (0) > 32 && f1x < 32 + (64) && f1y + (0) > SCREEN_HEIGHT - 64 && f1y < SCREEN_HEIGHT - 64 + (64)) ||
			(f2x + (0) > 32 && f2x < 32 + (64) && f2y + (0) > SCREEN_HEIGHT - 64 && f2y < SCREEN_HEIGHT - 64 + (64))) // Down
			JoyKey[0] = JoyKey[0] | IN_Down, touch_y = 32;
		else
			JoyKey[0] = JoyKey[0] & (~(IN_Down));

		if ((f1x + (0) > 64 && f1x < 64 + (64) && f1y + (0) > SCREEN_HEIGHT - 96 && f1y < SCREEN_HEIGHT - 96 + (64)) ||
			(f2x + (0) > 64 && f2x < 64 + (64) && f2y + (0) > SCREEN_HEIGHT - 96 && f2y < SCREEN_HEIGHT - 96 + (64))) // Right
			JoyKey[0] = JoyKey[0] | IN_Right, touch_x = 32;
		else
			JoyKey[0] = JoyKey[0] & (~(IN_Right));

		if (f1x + (0) > SCREEN_WIDTH / 2 - 72 && f1x < SCREEN_WIDTH / 2 - 72 + (64) && f1y + (0) > SCREEN_HEIGHT - 80 && f1y < SCREEN_HEIGHT - 80 + (64)) // Start
			JoyKey[0] = JoyKey[0] | IN_Button1;
		else
			JoyKey[0] = JoyKey[0] & (~(IN_Button1));
		if (f1x + (0) > SCREEN_WIDTH / 2 + 8 && f1x < SCREEN_WIDTH / 2 + 8 + (64) && f1y + (0) > SCREEN_HEIGHT - 80 && f1y < SCREEN_HEIGHT - 80 + (64)) // Select
			JoyKey[0] = JoyKey[0] | IN_Button2;
		else

		JoyKey[0] = JoyKey[0] & (~(IN_Button2));
		if (f1x + (0) > SCREEN_WIDTH - 136 && f1x < SCREEN_WIDTH - 136 + (64) && f1y + (0) > SCREEN_HEIGHT - 136 && f1y < SCREEN_HEIGHT - 136 + (64)) // Key1
			JoyKey[0] = JoyKey[0] | IN_Button3;
		else
			JoyKey[0] = JoyKey[0] & (~(IN_Button3));
		if (f1x + (0) > SCREEN_WIDTH - 72 && f1x < SCREEN_WIDTH - 72 + (64) && f1y + (0) > SCREEN_HEIGHT - 136 && f1y < SCREEN_HEIGHT - 136 + (64)) // Key1
			JoyKey[0] = JoyKey[0] | IN_Button4;
		else
			JoyKey[0] = JoyKey[0] & (~(IN_Button4));
		if (f1x + (0) > SCREEN_WIDTH - 136 && f1x < SCREEN_WIDTH - 136 + (64) && f1y + (0) > SCREEN_HEIGHT - 72 && f1y < SCREEN_HEIGHT - 72 + (64)) // Key2
			JoyKey[0] = JoyKey[0] | IN_Button5;
		else
			JoyKey[0] = JoyKey[0] & (~(IN_Button5));
		if (f1x + (0) > SCREEN_WIDTH - 72 && f1x < SCREEN_WIDTH - 72 + (64) && f1y + (0) > SCREEN_HEIGHT - 72 && f1y < SCREEN_HEIGHT - 72 + (64)) // Key3
			JoyKey[0] = JoyKey[0] | IN_Button6;
		else
			JoyKey[0] = JoyKey[0] & (~(IN_Button6));
	}
}
#endif

/* ---------------------------------------- */
/* --- ∆˛Œœ¡ı√÷§ŒΩÈ¥ÅEﬂƒÅE                  */
/* ---------------------------------------- */
void  InputInit(void)
{
  int i;
  int JoyNum;

  GpKeySwap = sys_joystick_swap;

  for(i=0; i<JOY_NUM_MAX; i++) {
    JoyPtr[i] = 0;
    JoyStickAxes[i] = 0;
    JoyStickButtons[i] = 0;
    JoyKey[i] = 0;
    JoyStick[i] = 0;
  }
  /* ----- •∏•Á•§•π•∆•£•√•Ø§Œ¬∏∫ﬂ≥Œ«ß */
  JoyNum = SDL_NumJoysticks();
  if (JoyNum > JOY_NUM_MAX) {
    JoyNum = JOY_NUM_MAX;
  }
  for (i = 0; i < (JoyNum + 1); i++) {
    JoyPtr[i] = SDL_JoystickOpen(i);
    if (JoyPtr[i] != NULL) {
      printf ("SDL_JoystickOpen failed: %s\n", SDL_GetError ());
      /* --- •∏•Á•§•π•∆•£•√•Ø§ŒºÅE‡§Ú•¡•ß•√•Ø */
      JoyStickAxes[i] = SDL_JoystickNumAxes(JoyPtr[i]);
      JoyStickButtons[i] = SDL_JoystickNumButtons(JoyPtr[i]);
    }
  }

  GpKeySwap = sys_joystick_swap;
}


/* ---------------------------------------- */
/* --- ∆˛Œœ¡ı√÷§Œ≤Ú ÅE                      */
/* ---------------------------------------- */
void  InputFree(void)
{
  int i;

  /* ----- •∏•Á•§•π•∆•£•√•Ø§Œ≤Ú ÅE*/
  for(i=0; i<JOY_NUM_MAX; i++) {
    if (JoyPtr[i] != 0) {
      SDL_JoystickClose(JoyPtr[i]);
    }
  }
}

/* ---------------------------------------- */
/* --- •≠°º§ŒƒÅEÅE™∆…§ﬂ§»§ÅE                */
/* ---------------------------------------- */
void  InputPoll(void)
{
  int  i;
  AppExit = 0;

  /* ----- for Triger */
  for(i=0; i<JOY_NUM_MAX; i++) {
    JoyKeyPast[i] = JoyKey[i];
    JoyStickPast[i] = JoyStick[i];
  }

#ifdef TOUCH
  gettouch ();
#else
  /* ----- Key Event Read */
  while(SDL_PollEvent(&event) != 0) {
    switch(event.type) {
    case SDL_KEYUP:
      key_read_up(&event.key);
      break;

    case SDL_KEYDOWN:
      key_read_down(&event.key);
      break;

	case SDL_JOYHATMOTION:
#ifdef GEKKO
	  joy_read_button_up_wii(&event.jhat);
#endif
      break;

    case SDL_JOYBUTTONUP:
#ifndef PSP
#ifdef __GP2X__
      joy_read_button_up_gp2x(&event.jbutton);
#else
      joy_read_button_up_pc(&event.jbutton);
#endif
#else
      joy_read_button_up_psp(&event.jbutton);
#endif
      break;

    case SDL_JOYBUTTONDOWN:
#ifndef PSP
#ifdef __GP2X__
      joy_read_button_down_gp2x(&event.jbutton);
#else
      joy_read_button_down_pc(&event.jbutton);
#endif
#else
      joy_read_button_down_psp(&event.jbutton);
#endif
      break;

    case SDL_JOYAXISMOTION:
      joy_read_stick(&event.jaxis);
      break;

    case SDL_QUIT:
	  AppExit = 1;
      break;
    }
  }
#endif
}


/* ---------------------------------------- */
/* --- •¢•◊•ÅE±°º•∑•Á•ÛΩ™Œª•≠°º§Œ»Ω Ã       */
/* ---------------------------------------- */
int  InputExit(void) {
  return (AppExit);  
}


/* ---------------------------------------- */
/* --- •Ê°º•∂°º∆˛Œœ§Œ∆…§ﬂ§»§ÅE              */
/* ---------------------------------------- */
int InputJoyKey(int side)
{
  int r1, r2;

  r1 = r2 = 0;
  if (side < JOY_NUM_MAX) {
    r1 = JoyKey[side];
    r2 = JoyStick[side];
    r1 |= r2;
  }

  return(r1);
}


/* ---------------------------------------- */
/* --- •Ê°º•∂°º∆˛Œœ§Œ•»•ÅE¨°º∆…§ﬂ§»§ÅE      */
/* ---------------------------------------- */
int  InputJoyKeyTriger(int side)
{
  int  r1, r2;

  r1 = r2 = 0;
  if (side < JOY_NUM_MAX) {
    r1 = (JoyKey[side] ^ JoyKeyPast[side]) & JoyKey[side];
    r2 = (JoyStick[side] ^ JoyStickPast[side]) & JoyStick[side];
    r1 |= r2;
  }
  return(r1);
}
