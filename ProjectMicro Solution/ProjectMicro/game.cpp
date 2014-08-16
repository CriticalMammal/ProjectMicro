//File: game.cpp
//Prog: Dylan Gallardo
//Purp: Handles all the main logic for the game, such as the event loop.


#include <SDL.h>
#include <SDL_mixer.h>
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
SDL_Event evt;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

double zoom = 1;	// Tracks current game magnification
double xOffset = 0, yOffset = 0;	// Screen position relative to objects
bool keys[] = {false, false, false, false, false, false}; // Array to track key press booleans


// Function Definitions 
bool init();
void close();	// Destroys allocated memory and closes the game





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
	int chipLayers = 5;		// How many chips there will be
	vector<Player*> chips;	// Vector to contain player chips

	// Load Audio
	Mix_Chunk *bgMusic = NULL;
	Mix_Chunk *whooshSound = NULL;

	bgMusic = Mix_LoadWAV("audio/corsica_crussles.wav");
	whooshSound = Mix_LoadWAV("audio/bigWhoosh (amplified).wav");

	if (bgMusic == NULL || whooshSound == NULL)
	{
		cout << "Unable to load a sound. Mixer_ERROR: " << Mix_GetError() << endl;
	}
	int channel;
	channel = Mix_FadeInChannel(-1, bgMusic, -1, 3000);

	// Temp player chip implementation
	int controlledChip = 0;
	double chipStartX = 400*zoom, chipStartY = 800*zoom, 
		chipStartW = 100, chipStartH = 100;
	double oldX, oldY, oldW, oldH, newW, newH, newSpeed;
	oldX = chipStartX*20;
	oldY = chipStartY*20;
	oldW = chipStartW;
	oldH = chipStartH;	

	double shrinkRate = 12;
	double shrinkSizeModifier = 8.4;

	for (int i=0; i<chipLayers; i++)
	{
		if (i == 0)
		{
			newW = oldW;
			newH = oldH;
			newSpeed = 0.05;
			chips.push_back(new Player);
		}
		else
		{
			newW = chips.back()->getBlockW();
			newH = chips.back()->getBlockH();
			newSpeed = chips.back()->getSpeed()/shrinkRate;
			chips.back()->chip = new Player;
			chips.push_back(chips.back()->chip);
			chips.back()->setParentChip(chips[chips.size()-2]);
		}
		
		chips.back()->initializeChip(oldX/20, oldY/20, newW, newH, newSpeed);
		chips.back()->initializeBoard(*renderer, "mapFile.txt");
		oldX /= 20;
		oldY /= 20;
		oldW = newW;
		oldH = newH;
	}

	// Set player control to specific chip
	for (int i=0; i<chipLayers; i++)
	{
		if (chips[i]->getWidth()*zoom >= 20)
		{
			controlledChip = i;
		}
	}

	Camera camera;
	camera.setfollowedObject(chips[controlledChip]);
	zoom = 0.01;
	camera.newZoom(zoom);

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
					case SDLK_w:
					case SDLK_UP:
						keys[UP] = true;
						break;
					case SDLK_s:
					case SDLK_DOWN:
						keys[DOWN] = true;
						break;
					case SDLK_a:
					case SDLK_LEFT:
						keys[LEFT] = true;
						break;
					case SDLK_RIGHT:
					case SDLK_d:
						keys[RIGHT] = true;
						break;
					case SDLK_LEFTBRACKET:
						camera.newZoom(zoom-(zoom/2));
						cout << "zoom = " << zoom-(zoom/2) << endl;
						break;
					case SDLK_RIGHTBRACKET:
						camera.newZoom(zoom+(zoom/2));
						cout << "zoom = " << zoom+(zoom/2) << endl;
				}
			}
			else if (evt.type == SDL_KEYUP)		// Key Up events
			{
				switch(evt.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						quit = true;
						break;
					case SDLK_w:
					case SDLK_UP:
						keys[UP] = false;
						break;
					case SDLK_s:
					case SDLK_DOWN:
						keys[DOWN] = false;
						break;
					case SDLK_a:
					case SDLK_LEFT:
						keys[LEFT] = false;
						break;
					case SDLK_d:
					case SDLK_RIGHT:
						keys[RIGHT] = false;
						break;
				}
			}
			else if (evt.type == SDL_MOUSEWHEEL)	// MouseWheel event
			{
				camera.newZoom(zoom+(evt.wheel.y*(zoom/2)));
			}
		}




		// ==================================
		// GAME LOGIC
		// ==================================

		camera.setfollowedObject(chips[controlledChip]);
		camera.updateCamera();

		// Set player control to specific chip
		for (int i=chipLayers-1; i>-1; i--)
		{
			if (chips[i]->getWidth()*zoom >= 20)
			{
				if (i != controlledChip)
				{
					channel = Mix_PlayChannel(-1, whooshSound, 0);
					controlledChip = i;
				}

				i = 0;
			}
		}

		for (int i=0; i<chipLayers; i++)
		{
			if (i == controlledChip)
			{
				chips[i]->handleKeys();
			}

			chips[i]->update();
		}

		theMap.setX(-xOffset*zoom+SCREEN_WIDTH/2);
		theMap.setY(-yOffset*zoom + SCREEN_HEIGHT/2);

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


	Mix_FadeOutChannel(channel, 1000);
	while (Mix_Playing(channel) != 0);
	Mix_FreeChunk(bgMusic);
	Mix_FreeChunk(whooshSound);
	close();	//cleans up allocated memory
	return 0;
} // END main()





bool init()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
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

	// Init Audio Mixer
	int audio_rate = 22050;
	uint16_t audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 3000;

	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
	{
		cout << "Audio failed to initialize. SDL_ERROR: " << Mix_GetError() << endl;
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

	Mix_CloseAudio();

	TTF_Quit();
	SDL_Quit();
} // END close()