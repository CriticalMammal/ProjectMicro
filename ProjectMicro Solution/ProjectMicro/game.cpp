//File: game.cpp
//Prog: Dylan Gallardo
//Purp: Handles all the main logic for the game, such as the event loop.


#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <time.h>

#include "definitions.h"
#include "tile.h"
#include "tileMap.h"
#include "sprite.h"
#include "camera.h"
#include "player.h"


using namespace std;




// Variable Definitions
SDL_Event evt;					// Event variable
SDL_Window *window = NULL;		// The game window to render images to
SDL_Renderer *renderer = NULL;	// Used to render all the game objects
double zoom = 5;				// Tracks current game magnification
double xOffset = 0,				// Screen position relative to objects
	   yOffset = 0;
int mapWidthInPixels,			// Not useful here, remove this from here and camera.cpp later
	mapHeightInPixels;
bool keys[] = {false, false, false, false, false, false}; // Array to track key press booleans


// Function Definitions 
bool init();			// Initializes SDL parts
void close();			// Destroys allocated memory and closes the game





int main(int argc, char *args[])
{
	srand((unsigned)time(NULL)); //seed random numbers

	// Initialize SDL
	if (!init())
	{
		cout << "SDL failed to initialize" << endl;
		return -1;
	}

	// Initialize Variables
	bool quit = false;		// Track when to quit the game
	int chipLayers = 2;		// How many chips there will be
	vector<Player*> chips;	// Vector to contain player chips

	// Temp player chip implementation
	int controlledChip = 0;
	double chipStartX = 60*zoom, chipStartY = 60*zoom, 
		chipStartW = 600, chipStartH = 600;
	double oldX, oldY, oldW, oldH;
	oldX = chipStartX;
	oldY = chipStartY;
	oldW = chipStartW;
	oldH = chipStartH;

	for (int i=0; i<chipLayers; i++)
	{
		chips.push_back(new Player);
		chips.back()->setX(oldX+(oldW/15));
		chips.back()->setY(oldY+(oldH/15));
		chips.back()->setWidth(oldW/15);
		chips.back()->setHeight(oldH/15);
		chips.back()->initializeBoard(*renderer);

		oldX = oldX+(oldW/15);
		oldY = oldY+(oldH/15);
		oldW = oldW/15;
		oldH = oldH/15;
	}

	if (zoom <= 1)
		controlledChip = 0;
	else if (zoom > 1 && zoom < 10)
		controlledChip = 1;

	Camera camera;
	camera.setfollowedObject(chips[controlledChip]);
	camera.newZoom(5, 0.01, 0.5, 0.1);

	// Create a temporary tile map test
	TileMap theMap;
	theMap.initialize("no location", 20, 20, 20, 20, renderer);
	theMap.setX(xOffset);
	theMap.setY(yOffset);
	



	// ==================================
	// MAIN LOOP
	// ==================================
	while (!quit)
	{

		// Check for any events
		while (SDL_PollEvent(&evt) != 0)
		{
			if (evt.type == SDL_QUIT)		    // Quit event
			{
				quit = true;
			}
			else if (evt.type == SDL_KEYDOWN)	// Key Down events
			{
				switch(evt.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						quit = true;
						break;
					case SDLK_UP:
						keys[UP] = true;
						break;
					case SDLK_DOWN:
						keys[DOWN] = true;
						break;
					case SDLK_LEFT:
						keys[LEFT] = true;
						break;
					case SDLK_RIGHT:
						keys[RIGHT] = true;
						break;
					case SDLK_LEFTBRACKET:
						camera.newZoom(zoom+(zoom/2), 0.01, 0.5, 0.1);
						break;
					case SDLK_RIGHTBRACKET:
						camera.newZoom(zoom+(zoom/2), 0.01, 0.5, 0.1);
				}
			}
			else if (evt.type == SDL_KEYUP)		// Key Up events
			{
				switch(evt.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						quit = true;
						break;
					case SDLK_UP:
						keys[UP] = false;
						break;
					case SDLK_DOWN:
						keys[DOWN] = false;
						break;
					case SDLK_LEFT:
						keys[LEFT] = false;
						break;
					case SDLK_RIGHT:
						keys[RIGHT] = false;
						break;
				}
			}
			else if (evt.type == SDL_MOUSEWHEEL)	// MouseWheel event
			{
				camera.newZoom(zoom+(evt.wheel.y*(zoom/2)), 0.01, 0.5, 0.1);
			}
		}




		// ==================================
		// GAME LOGIC
		// ==================================

		camera.setfollowedObject(chips[controlledChip]);
		camera.updateCamera();


		if (zoom <= 1)
			controlledChip = 0;
		else if (zoom > 1 && zoom < 10)
			controlledChip = 1;


		for (int i=0; i<chipLayers; i++)
		{
			if (i >= controlledChip)
			{
				chips[i]->handleKeys();
			}

			chips[i]->update();
		}

		theMap.setX(-xOffset);
		theMap.setY(-yOffset);

		SDL_Rect screenRect = {xOffset, yOffset, SCREEN_WIDTH, SCREEN_HEIGHT};



		// ==================================
		// RENDER
		// ==================================
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);	// Set render color to black
		SDL_RenderClear(renderer);						// Clear screen graphics


		//theMap.drawTileMap(screenRect, renderer);	// Draw temporary map

		for (int i=0; i<chipLayers; i++)	// Draw chips
		{
			chips[i]->draw(renderer);
		}


		SDL_RenderPresent(renderer);	// Display renderer to the screen
	} // END while(!quit)



	close();	//cleans up allocated memory
	return 0;
} // END main()





bool init()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL Init failed. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Create window
	window = SDL_CreateWindow("SDL Prototype", 600, 40, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window)
	{
		printf("SDL Window failed to init. SDL_ERROR: %s\n", SDL_GetError());
		return false;
	}

	// Create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
	{
		printf("SDL renderer failed to init. SDL_ERROR: %s\n", SDL_GetError());
		return false;
	}
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);				// Init render draw color to black
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);	// Allow for renderer transparency


	// Init TTF
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
} // END init()





void close()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;

	TTF_Quit();
	SDL_Quit();
} // END close()