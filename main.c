/*
  
MIT License

Copyright (c) 2026 Wisdurm

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

/// SETTINGS
// Whether to only use Left, Right and Down for everything
static bool tkey = false;
// Whether to show help for tkey mode
static bool thelp = true;

// Size of the screen
static int screenWidth = 500;
static int screenHeight = 500;
static int screenX = 0;
static int screenY = 0;
// box, relative to window
static int bx = 0;
static int by = 0;
static int bw = 0;
static int bh = 0;
// Amount of splits
static short splits = 0;

enum Direction {
	LEFT,
	RIGHT,
	UP,
	DOWN
};
// Split the screen
void splitScreen(enum Direction direction);

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    /* Create the window */
    if (!SDL_CreateWindowAndRenderer("wisdomkey", 500, 500,
									 SDL_WINDOW_MAXIMIZED | SDL_WINDOW_TRANSPARENT |
									 SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_UTILITY |
									 SDL_WINDOW_BORDERLESS | SDL_WINDOW_INPUT_FOCUS,
									 &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
	// Get display
	SDL_DisplayID display;
	display = SDL_GetDisplayForWindow(window);
	// Get screen size
	SDL_Rect* area = SDL_malloc(sizeof(SDL_Rect));
	SDL_GetDisplayBounds(display, area);
	// Save size
	screenWidth = area->w;
	screenHeight = area->h;
	screenX = area->x;
	screenY = area->y;
	// Start
	bw = screenWidth;
	bh = screenHeight;
	// Cleanup
	SDL_free(area);
	// Resize window
	SDL_SetWindowSize(window, bw, bh);
	SDL_SetWindowPosition(window, screenX, screenY);
	// Command line arguments
	if (argc > 1 ) {
		// Initial split direction
		if (argv[1][0] == 'l')
			splitScreen(LEFT);
		else if (argv[1][0] == 'r')
			splitScreen(RIGHT);
		else if (argv[1][0] == 'u')
			splitScreen(UP);
		else if (argv[1][0] == 'd')
			splitScreen(DOWN);
	}
	
    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
	else if (event->type == SDL_EVENT_KEY_DOWN) {
		if (tkey) {
			// Three key setup
			if (splits % 2 == 0) {
				if (event->key.key == SDLK_LEFT)
					splitScreen(LEFT);
				else if (event->key.key == SDLK_RIGHT)
					splitScreen(RIGHT);
			} else {
				if (event->key.key == SDLK_LEFT)
					splitScreen(UP);
				else if (event->key.key == SDLK_RIGHT)
					splitScreen(DOWN);				
			}
		} else {
			// Normal setup
			if (event->key.key == SDLK_LEFT)
				splitScreen(LEFT);
			else if (event->key.key == SDLK_RIGHT)
				splitScreen(RIGHT);	
			if (event->key.key == SDLK_UP)
				splitScreen(UP);
			else if (event->key.key == SDLK_DOWN)
			splitScreen(DOWN);
		}
		// Exit
		if (event->key.key == SDLK_ESCAPE ||
			event->key.key == SDLK_RETURN ||
			(event->key.key == SDLK_DOWN && tkey)) {
			return SDL_APP_SUCCESS;
		}
	}
    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
	// Fill
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
	const SDL_FRect rect = {
		.x = (float)bx,
		.y = (float)by,
		.w = (float)bw,
		.h = (float)bh
	};
	SDL_RenderFillRect(renderer, &rect);
	// Border
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 200);
	SDL_RenderRect(renderer, &rect);
	// Render three key mode helpers
	if (tkey && thelp && splits < 10) {
		const int scale = 10 - splits;
		const int cw = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;
		SDL_SetRenderScale(renderer, scale, scale);
		if (splits % 2 == 0) {
			SDL_RenderDebugText(renderer,
								(bx+cw) / scale,
								(by + (bh/2)) / scale, "L");
			SDL_RenderDebugText(renderer,
								(bx+bw-(cw*scale)) / scale,
								(by + (bh/2)) / scale, "R");
		} else {
			SDL_RenderDebugText(renderer,
								(bx + (bw/2)) / scale,
								(by + cw) / scale, "L");
			SDL_RenderDebugText(renderer,
								(bx + (bw/2)) / scale,
								(by+bh-(cw*scale)) / scale, "R");
		}
		SDL_SetRenderScale(renderer, 1, 1);
	}
	// Move mouse to center of box
	SDL_WarpMouseInWindow(window, (bw/2) + bx, (bh/2) + by);
	//
    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}

void splitScreen(enum Direction direction)
{
	if (direction == LEFT) {
		bw /= 2;
		splits++;
	} else if (direction == RIGHT) {
		bw /= 2;
		bx += bw;
		splits++;
	}
	else if (direction == UP) {
		bh /= 2;
		splits++;
	} else if (direction == DOWN) {
		bh /= 2;
		by += bh;
		splits++;
	}
}
