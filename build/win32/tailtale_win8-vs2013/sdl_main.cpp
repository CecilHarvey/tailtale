/*
	SDL Main for Windows 8 and Windows Phone 8
*/

#include "pch.h"

int CALLBACK WinMain (HINSTANCE, HINSTANCE, LPSTR, int) {
    if (FAILED (Windows::Foundation::Initialize (RO_INIT_MULTITHREADED)))
        return 1;

	if (SDL_WinRTRunApp (SDL_main, NULL))
		return 1;

	return 0;
}
