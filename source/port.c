#include "default.h"

#ifdef SDL2
static Uint32 video_flags = 0;

#if !defined(ANDROID) || !defined(WINPHONE)
static int SDLCALL SCREEN_EventWatch (void *userdata, SDL_Event * event);
#endif

SDL_bool SCREEN_IsVideoModeSet () {
    return (video_surface != NULL) ? SDL_TRUE : SDL_FALSE;
}

static void SCREEN_ShutdownVideo() {
    if (video_texture) {
        SDL_DestroyTexture (video_texture);
        video_texture = NULL;
    }

    if (video_renderer) {
        SDL_DestroyRenderer (video_renderer);
        video_renderer = NULL;
    }

    if (video_surface) {
#ifndef VS_WIN8
		SDL_FreeSurface(video_surface);
#endif
		video_surface = NULL;
    }

    if (video_window) {
        SDL_DestroyWindow (video_window);
        video_window = NULL;
    }

#if !defined(ANDROID) || !defined(WINPHONE)
    SDL_DelEventWatch (SCREEN_EventWatch, NULL);
#endif

    video_flags = 0;
}

SDL_Surface * SCREEN_SetVideoMode (int width, int height, Uint32 flags) {
	Uint32 window_flags;

	if (SCREEN_IsVideoModeSet ()) {
        SCREEN_ShutdownVideo ();
    }

#if !defined(ANDROID) || !defined(WINPHONE)
    SDL_DisplayMode display_mode;
    if (SDL_GetDesktopDisplayMode (0, &display_mode) != 0) {
        SCREEN_ShutdownVideo ();
        return NULL;
    }
#endif

    window_flags = 0;
    if (flags & SCREEN_FULLSCREEN) {
        window_flags |= SDL_WINDOW_FULLSCREEN;
    }
    if (flags & SCREEN_RESIZABLE) {
        window_flags |= SDL_WINDOW_RESIZABLE;
    }

#ifdef ANDROID
    video_window = SDL_CreateWindow ("TailTale", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 480, 272, window_flags);
#elif WINPHONE
    video_window = SDL_CreateWindow ("TailTale", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 640, window_flags);
#else
    video_window = SDL_CreateWindow ("TailTale", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, display_mode.w, display_mode.h, window_flags);
#endif
    if (!video_window) {
        SCREEN_ShutdownVideo ();
        return NULL;
    }

    video_renderer = SDL_CreateRenderer (video_window, -1, SDL_RENDERER_ACCELERATED);
    if (!video_renderer) {
        SCREEN_ShutdownVideo ();
        return NULL;
    }

    video_texture = SDL_CreateTexture (video_renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!video_texture) {
        SCREEN_ShutdownVideo ();
        return NULL;
    }

    video_surface = SDL_CreateRGBSurface (0, width, height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if (!video_surface) {
        SCREEN_ShutdownVideo ();
        return NULL;
    }

    video_flags = flags;

#if !defined(ANDROID) || !defined(WINPHONE)
    SDL_AddEventWatch (SCREEN_EventWatch, NULL);
#endif

    return video_surface;
}

Uint32 SCREEN_GetVideoModeFlags () {
    return video_flags;
}

SDL_Surface * SCREEN_GetVideoSurface () {
    return video_surface;
}

SDL_Window * SCREEN_GetVideoWindow() {
    return video_window;
}

SDL_Renderer * SCREEN_GetVideoRenderer() {
    return video_renderer;
}

int SCREEN_Flip() {
	SDL_Rect src_rect = {0, 0, video_surface->w, video_surface->h};
	
	if (!video_renderer || !video_texture || !video_surface) {
        SDL_SetError ("SCREEN: A video mode does not appear to have been set.");
        return -1;
    }

    if (SDL_UpdateTexture (video_texture, NULL, video_surface->pixels, video_surface->pitch) != 0) {
        return -1;
    }

    if (SDL_RenderCopy (video_renderer, video_texture, &src_rect, NULL) != 0) {
        return -1;
    }

    SDL_RenderPresent (video_renderer);

    return 0;
}

#if !defined(ANDROID) || !defined(WINPHONE)
static int SDLCALL SCREEN_EventWatch (void *userdata, SDL_Event * event) {
    if (!video_surface || !video_window) {
        return -1;
    }

    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize (video_window, &window_width, &window_height);
    if (window_width == 0 || window_height == 0) {
        return -1;
    }

    float scale_x = (float)video_surface->w / (float)window_width;
    float scale_y = (float)video_surface->h / (float)window_height;

    switch (event->type) {
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            event->button.x = (int)(event->button.x * scale_x);
            event->button.y = (int)(event->button.y * scale_y);
            break;

        case SDL_MOUSEMOTION:
            event->motion.x = (int)(event->motion.x * scale_x);
            event->motion.y = (int)(event->motion.y * scale_y);
            event->motion.xrel = (int)(event->motion.xrel * scale_x);
            event->motion.yrel = (int)(event->motion.yrel * scale_y);
            break;
    }
    return 1;
}
#endif

#endif
