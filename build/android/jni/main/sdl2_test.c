#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

typedef struct Sprite {
	SDL_Texture *texture;
	Uint16 w, h;
} Sprite;

Sprite LoadSprite (const char* file, SDL_Renderer* renderer) {
	SDL_Surface* temp;
	Sprite image;

	image.texture = NULL;
	image.w = image.h = 0;

	temp = IMG_Load (file);
	if (temp == NULL) {
		fprintf (stderr, "Couldn't load %s: %s\n", file, SDL_GetError ());

		return image;
	}

	image.texture = SDL_CreateTextureFromSurface (renderer, temp);
	if (!image.texture) {
		fprintf (stderr, "Couldn't create texture: %s\n", SDL_GetError ());
		SDL_FreeSurface (temp);

		return image;
	}

	image.w = temp->w, image.h = temp->h;

	SDL_FreeSurface (temp);

	return image;
}

void draw (SDL_Window* window, SDL_Renderer* renderer, const Sprite sprite) {
	int w, h;

	SDL_GetWindowSize (window, &w, &h);
	SDL_Rect destRect = {
		w / 2 - sprite.w / 2,
		h / 2 - sprite.h / 2,
		sprite.w,
		sprite.h
	};

	SDL_RenderCopy (renderer, sprite.texture, NULL, &destRect);
}

int main(int argc, char *argv[]) {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    Uint8 done = 0;

    if (SDL_CreateWindowAndRenderer(0, 0, 0, &window, &renderer) < 0)
        exit(1);

	Sprite sprite = LoadSprite ("image.png", renderer);
    if (sprite.texture == NULL)
		exit(2);

	SDL_InitSubSystem (SDL_INIT_AUDIO);
	if (Mix_OpenAudio (22050, AUDIO_S16SYS, 2, 2048))
		exit(3);
	Mix_Music *music;
	music = Mix_LoadMUS ("demo.ogg");
	if (!Mix_PlayingMusic ())
		Mix_PlayMusic (music, -1);

	TTF_Font *font;
	SDL_Color colour = { 255, 255, 255 };
	SDL_Surface *text;

	TTF_Init ();

	font = TTF_OpenFont ("arial.ttf", 24);
	if (!font)
		exit (4);

	text = TTF_RenderText_Solid (font, "Hello, World!", colour);

    while (!done) {
        while (SDL_PollEvent (&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN || event.type == SDL_FINGERDOWN) {
				done = 1;
            }
        }

		SDL_SetRenderDrawColor (renderer, 0xA0, 0xA0, 0xA0, 0xFF);
		SDL_RenderClear (renderer);

		draw (window, renderer, sprite);
		SDL_RenderCopy (renderer, text, NULL, NULL);
	
		SDL_RenderPresent (renderer);
		
		SDL_Delay (10);
    }

    exit (0);
}


