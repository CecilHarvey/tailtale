/* TailTale - Theora Movie Player */

#include "default.h"

#ifdef MOVIE
extern TGameScreen  *scr;

static Uint32 baseticks = 0;

typedef struct AudioQueue {
    const THEORAPLAY_AudioPacket *audio;
    int offset;
    struct AudioQueue *next;
} AudioQueue;

static volatile AudioQueue *audio_queue = NULL;
static volatile AudioQueue *audio_queue_tail = NULL;

const THEORAPLAY_VideoFrame *video = NULL;
const THEORAPLAY_AudioPacket *audio = NULL;

static void SDLCALL audio_callback (void *userdata, Uint8 *stream, int len) {
	Sint16 *dst = (Sint16 *) stream;

	while (audio_queue && (len > 0)) {
		volatile AudioQueue *item = audio_queue;
		AudioQueue *next = item->next;
		const int channels = item->audio->channels;

		const float *src = item->audio->samples + (item->offset * channels);
		int cpy = (item->audio->frames - item->offset) * channels;
		int i;

		if (cpy > (len / sizeof (Sint16)))
			cpy = len / sizeof (Sint16);

		for (i = 0; i < cpy; i++) {
			const float val = *(src++);
			if (val < -1.0f)
				*(dst++) = -32768;
			else if (val > 1.0f)
				*(dst++) = 32767;
			else
				*(dst++) = (Sint16) (val * 32767.0f);
		}

		item->offset += (cpy / channels);
		len -= cpy * sizeof (Sint16);

		if (item->offset >= item->audio->frames) {
			THEORAPLAY_freeAudio(item->audio);
			free((void *) item);
			audio_queue = next;
		}
	}

	if (!audio_queue)
		audio_queue_tail = NULL;

	if (len > 0)
		memset(dst, '\0', len);
}

static void queue_audio(const THEORAPLAY_AudioPacket *audio) {
	AudioQueue *item = (AudioQueue *) malloc(sizeof (AudioQueue));
	if (!item) {
		THEORAPLAY_freeAudio(audio);
		return;
	}

	item->audio = audio;
	item->offset = 0;
	item->next = NULL;

	SDL_LockAudio();
	if (audio_queue_tail)
		audio_queue_tail->next = item;
	else
		audio_queue = item;
	audio_queue_tail = item;
	SDL_UnlockAudio();
}

int theora_audio_init () {
	SDL_AudioSpec spec;

	Mix_CloseAudio (); // Close Mix_OpenAudio
	memset (&spec, '\0', sizeof (SDL_AudioSpec));
#ifdef GEKKO // Wii use 32khz or 48khz
	spec.freq = 44100;
#else
	spec.freq = audio->freq;
#endif
	spec.format = AUDIO_S16SYS;
	spec.channels = audio->channels;
	spec.samples = 2048;
	spec.callback = audio_callback;
	if (SDL_OpenAudio(&spec, NULL) < 0) {
		printf ("SDL_OpenAudio failed: %s\n", SDL_GetError ());
		return 1;
	}

	return 0;
}

int theora_audio_deinit () {
	SDL_CloseAudio (); // Close SDL_OpenAudio
	if (Mix_OpenAudio (SND_FREQ, SND_FORMAT, SND_CHAN, 1024) != 0) {
		printf ("Mix_OpenAudio failed: %s\n", Mix_GetError ());
		return 1;
	}

	return 0;
}

void YUV420ToRGB (unsigned char *pYUVBuf[3], unsigned long width, unsigned long height, SDL_Surface *pRGBBuf) {
	long U[256], V[256], Y1[256], Y2[256];
	unsigned char *pY, *pU, *pV, *pUbase, *pVbase;
	unsigned long i, j, RGB_SIZE;
	unsigned char *pR, *pG, *pB;
	unsigned char *buf;

	for (i = 0; i < 256; i++)
		Y1[i] = 11644 * i, Y2[i] = 19837 * i -311710, V[i] = 15938 * i -2221300, U[i] = 20238 * i -2771300;

	pY = *pYUVBuf;
	pU = pUbase = *pYUVBuf + width * height;
	pV = pVbase = pUbase + width * height / 4;

	RGB_SIZE = width * height * 3L;

	for (i = 0; i < height; i++) {
		buf = (Uint8 *) pRGBBuf->pixels + i * pRGBBuf->pitch; 

		pB = buf + RGB_SIZE - 3 * width * height;
		pG = buf + RGB_SIZE - 3 * width * height + 1;
		pR = buf + RGB_SIZE - 3 * width * height + 2;

		for (j = 0; j < width; j += 2) {
			*pR = max (0, min (255, (V[*pV] + Y1[*pY]) / 10000));
			*pB = max (0, min (255, (U[*pU] + Y1[*pY]) / 10000));
			*pG = max (0, min (255, (Y2[*pY] - 5094 * (*pR) - 1942 * (*pB)) / 10000));

			pR += 3, pB += 3, pG += 3;

			pY++;

			*pR = max (0, min (255, (V[*pV] + Y1[*pY]) / 10000));
			*pB = max (0, min (255, (U[*pU] + Y1[*pY]) / 10000));
			*pG = max (0, min (255, (Y2[*pY] - 5094 * (*pR) - 1942 * (*pB)) / 10000));

			pR += 3, pB += 3, pG += 3;

			pY++, pU++, pV++;
		}

		if ((i % 2 == 0))
			pU = pU - (width >> 1), pV = pV - (width >> 1);
	}
}

int play_movie (char *filename) {
	THEORAPLAY_Decoder *decoder = NULL;
#ifdef SDL2
#if !defined(ANDROID) || !defined(WINPHONE)
	SDL_Window *wnd = NULL;
	SDL_Renderer *ren = NULL;
	SDL_Texture *tex = NULL;
#endif
#endif
	SDL_Surface *overlay = NULL;
	unsigned char *src[3];
	Uint32 framems = 0;
	int initfailed = 0;
	int quit = 0;
	int inp;
#ifdef TAILTALE_HD
	SDL_Surface *bg = NULL, *frame = NULL;
#ifdef LIBPAK
	unsigned int size;
	byte *buffer;
#endif
#endif

	decoder = THEORAPLAY_startDecodeFile(va ("%s/%s", BASEDIR, filename), 30, THEORAPLAY_VIDFMT_IYUV);
	if (!decoder) {
		printf("Failed to start decoding '%s'!\n", filename);
		return 1;
	}

	while (!audio || !video) {
		if (!audio)
			audio = THEORAPLAY_getAudio(decoder);

		if (!video)
			video = THEORAPLAY_getVideo(decoder);
	}

	if (scr->Screen) {
		SDL_FillRect(scr->Screen, NULL, SDL_MapRGB(scr->Screen->format, 0, 0, 0));
#ifdef SDL2
#if !defined(ANDROID) || !defined(WINPHONE)
		wnd = SDL_CreateWindow ("TailTale", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, video->width, video->height, SDL_WINDOW_FULLSCREEN);
		if (!wnd)
			printf ("Couldn't set create window: %s\n", SDL_GetError ());

		ren = SDL_CreateRenderer (wnd, -1, SDL_RENDERER_ACCELERATED);
		if (!ren)
			printf ("Couldn't set create renderer: %s\n", SDL_GetError ());
#endif
#endif

		overlay = SDL_CreateRGBSurface (SDL_SWSURFACE, video->width, video->height, 24, 0xFF0000, 0x00FF00, 0x0000FF, 0);
		if (!overlay)
			printf("RGB Surface failed: %s\n", SDL_GetError ());

#ifdef TAILTALE_HD
#ifdef LIBPAK
		buffer = extractMem ("graphic/background/title.tga", openPak("data.pak"), &size);
		if (buffer == NULL)
			return;

		bg = IMG_LoadTGA_RW (SDL_RWFromMem (buffer, size+1));
#else
		bg = IMG_Load (va ("%s/graphic/background/title.tga", BASEDIR));
#endif
		if (bg == NULL) {
			printf ("%s\n", SDL_GetError ()); 
			return 1;
		}
#endif

#ifdef TAILTALE_HD
#ifdef LIBPAK
		buffer = extractMem ("graphic/background/movie.tga", openPak("data.pak"), &size);
		if (buffer == NULL)
			return;

		frame = IMG_LoadTGA_RW (SDL_RWFromMem (buffer, size+1));
#else
		frame = IMG_Load (va ("%s/graphic/background/movie.tga", BASEDIR));
#endif
		if (frame == NULL) {
			printf ("%s\n", SDL_GetError ()); 
			return 1;
		}
#endif
	}

	framems = (video->fps == 0.0) ? 0 : ((Uint32) (1000.0 / video->fps));

#ifdef SDL2
#if defined(ANDROID) || defined(WINPHONE)
	initfailed = quit = (!scr->Screen || !overlay);
#else
	initfailed = quit = (!scr->Screen || !wnd || !ren || !overlay);
#endif
#else
	initfailed = quit = (!scr->Screen || !overlay);
#endif
	initfailed = quit = (initfailed || (theora_audio_init() < 0));

	while (audio) {
		queue_audio(audio);
		audio = THEORAPLAY_getAudio(decoder);
	}

	baseticks = SDL_GetTicks();

	if (!quit)
		SDL_PauseAudio(0);

	while (!quit && THEORAPLAY_isDecoding(decoder)) {
		const Uint32 now = SDL_GetTicks() - baseticks;
		InputPoll ();

		if (!video)
			video = THEORAPLAY_getVideo(decoder);

		if (video && (video->playms <= now)) {
			if (framems && ((now - video->playms) >= framems)) {
				const THEORAPLAY_VideoFrame *last = video;
				while ((video = THEORAPLAY_getVideo(decoder)) != NULL) {
					THEORAPLAY_freeVideo(last);
					last = video;
					if ((now - video->playms) < framems)
						break;
				}

				if (!video)
					video = last;
			}

			if (!video) {
				static int warned = 0;
				if (!warned) {
					warned = 1;
					printf("WARNING: Playback can't keep up!\n");
				}
			} else if (SDL_LockSurface (overlay) == -1) {
				static int warned = 0;
				if (!warned) {
					warned = 1;
					printf("Couldn't lock RGB Surface: %s\n", SDL_GetError());
				}
			} else {
#ifdef TAILTALE_HD
				SDL_Rect dstrect = { scr->Screen->w / 2 - video->width / 2, scr->Screen->h / 2 - video->height / 2, scr->Screen->w, scr->Screen->h };
#else
				SDL_Rect dstrect = { 0, 0, scr->Screen->w, scr->Screen->h };
#endif
				const int w = video->width, h = video->height;
				const Uint8 *y = (const Uint8 *) video->pixels;
				const Uint8 *u = y + (w * h);
				const Uint8 *v = u + ((w / 2) * (h / 2));

				src[0] = (unsigned char *)y, src[1] = (unsigned char *)u, src[2] = (unsigned char *)v;
				YUV420ToRGB (src, video->width, video->height, overlay);

				SDL_UnlockSurface (overlay);

#ifdef SDL2
#if defined(ANDROID) || defined(WINPHONE)
#ifdef TAILTALE_HD
				SDL_Rect movrect = { 67, 3, 346, 266 };

				SDL_BlitSurface (bg, NULL, scr->Screen, NULL);
				SDL_BlitSurface (frame, NULL, scr->Screen, &movrect);
#endif
				SDL_BlitSurface (overlay, NULL, scr->Screen, &dstrect);
				TGameScreen_RefreshMovieScreen ();
#else
				tex = SDL_CreateTextureFromSurface (ren, overlay);
				if (!tex)
					printf ("CreateTextureFromSurface failed: %s\n", SDL_GetError ());

				SDL_RenderClear (ren);
				SDL_RenderCopy (ren, tex, &dstrect, NULL);
				SDL_RenderPresent (ren);
#endif
#else
#ifdef TAILTALE_HD
				SDL_Rect movrect = { 67, 3, 346, 266 };

				SDL_BlitSurface (bg, NULL, scr->Screen, NULL);
				SDL_BlitSurface (frame, NULL, scr->Screen, &movrect);
#endif
				SDL_BlitSurface (overlay, NULL, scr->Screen, &dstrect);
				TGameScreen_RefreshMovieScreen ();
#endif
			}

			inp = InputJoyKey(0);
			if ((inp & (IN_Button1|IN_Button2|IN_Button3|IN_Button4|IN_Button5|IN_Button6)) != 0) {
				SDL_FillRect (scr->Screen, NULL, SDL_MapRGB (scr->Screen->format, 0, 0, 0));
				quit = 1;
			}

			THEORAPLAY_freeVideo(video);
			video = NULL;
		} else {
			SDL_Delay (10);
		}

		while ((audio = THEORAPLAY_getAudio (decoder)) != NULL)
			queue_audio (audio);
	}

	while (!quit) {
		SDL_LockAudio();
		quit = (audio_queue == NULL);
		SDL_UnlockAudio();
		if (!quit)
			SDL_Delay(100);
	}

	if (initfailed)
		printf("Initialization failed!\n");
	else if (THEORAPLAY_decodingError(decoder))
		printf("There was an error decoding this file!\n");
	else
		printf("done with this file!\n");

	if (overlay)
		SDL_FreeSurface (overlay);
	overlay = NULL;


#ifdef TAILTALE_HD
	if (bg)
		SDL_FreeSurface (bg);
	bg = NULL;

	if (frame)
		SDL_FreeSurface (frame);
	frame = NULL;
#endif

#ifdef SDL2
#if !defined(ANDROID) || !defined(WINPHONE)
	if (ren)
		SDL_DestroyRenderer (ren);

	if (wnd)
		SDL_DestroyWindow (wnd);
#endif
#endif

	if (video)
		THEORAPLAY_freeVideo(video);
	video = NULL;

	if (audio)
		THEORAPLAY_freeAudio(audio);
	audio_queue = NULL;
	audio = NULL;

	if (decoder)
		THEORAPLAY_stopDecode(decoder);

	theora_audio_deinit ();

	return 0;
}
#endif
