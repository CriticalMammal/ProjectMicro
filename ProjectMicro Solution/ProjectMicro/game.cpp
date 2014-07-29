//File: game.cpp
//Prog: Dylan Gallardo
//Purp: handles all the main logic for the game, such as the event loop.


#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

#include "definitions.h"


using namespace std;



//variable definitions
SDL_Event evt;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

//function definitions 
bool init();
void close();



//-------------
//MAIN FUNCTION
//-------------

int main(int argc, char *args[])
{
	//----------
	//INITIALIZE
	//----------

	//variables
	bool quit = false;

	//initialize SDL
	if (!init())
	{
		cout << "SDL failed to initialize" << endl;
		return -1;
	}



	//----------
	//MAIN LOOP
	//----------
	while (!quit)
	{
		//-------
		//EVENTS
		//-------
		while (SDL_PollEvent(&evt) != 0)
		{
			if (evt.type == SDL_QUIT)
			{
				quit = true;
			}

			//KEYS DOWN
			else if (evt.type == SDL_KEYDOWN)
			{
				switch(evt.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						quit = true;
						break;
				}
			}

			//KEYS UP
			else if (evt.type == SDL_KEYUP)
			{
				switch(evt.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						quit = true;
						break;
				}
			}
		}




		//-------
		//RENDER
		//-------
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		SDL_RenderPresent(renderer);
	}


	//---------
	//EXIT GAME
	//---------
	close();
	return 0;

} //END main()




bool init()
{
	//initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL Init failed. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	//create window
	window = SDL_CreateWindow("SDL Prototype", 600, 40, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window)
	{
		printf("SDL Window failed to init. SDL_ERROR: %s\n", SDL_GetError());
		return false;
	}

	//create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
	{
		printf("SDL renderer failed to init. SDL_ERROR: %s\n", SDL_GetError());
		return false;
	}
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);


	//init TTF
	if (TTF_Init() != 0)
	{
		printf("TTF failed to init. SDL_ERROR: %s\n", SDL_GetError());
		return false;
	}

	if (SDL_Init(SDL_INIT_TIMER) < 0)
	{
		cout << "Timer failed to init. SDL_ERROR: " << SDL_GetError() << endl;
		return false;
	}

	return true;
} //END init()




void close()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;

	TTF_Quit();
	SDL_Quit();
} //END close()