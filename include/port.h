typedef enum {
	SCREEN_FULLSCREEN = 0x00000001,
	SCREEN_RESIZABLE = 0x00000002
} SCREEN_VideoFlags;

SDL_Surface *video_surface;
SDL_Window *video_window;
SDL_Renderer *video_renderer;
SDL_Texture *video_texture;

SDL_bool SCREEN_IsVideoModeSet ();

SDL_Surface * SCREEN_SetVideoMode (int width, int height, Uint32 flags);

Uint32 SCREEN_GetVideoModeFlags ();

SDL_Surface * SCREEN_GetVideoSurface ();
SDL_Window * SCREEN_GetVideoWindow ();
SDL_Renderer * SCREEN_GetVideoRenderer ();

int SCREEN_Flip ();
