#include <SDL.h>
#include <string>
#include <vector>
#include <iostream>

#include "definitions.h"
#include "sprite.h"
#include "player.h"

extern bool keys[];
extern double xOffset, yOffset, zoom;
extern SDL_Renderer* renderer;

Player::Player()
{
	x = 60*zoom;
	y = 60*zoom;

	//x = 210;
	//y = 330;

	width = 10;
	height = 10;

	playerRect.x = (x*zoom-xOffset);
	playerRect.y = (y*zoom-yOffset);
	playerRect.w = width*zoom;
	playerRect.h = height*zoom;

	collisionRect.x = x;
	collisionRect.y = y;
	collisionRect.w = width;
	collisionRect.h = height;

	collisionPad = 1;

	collisionHorz.x = x;
	collisionHorz.y = y+collisionPad;
	collisionHorz.w = width;
	collisionHorz.h = height-collisionPad*2;

	collisionVert.x = x+collisionPad;
	collisionVert.y = y;
	collisionVert.w = width-collisionPad*2;
	collisionVert.h = height;

	vx = 0;
	vy = 0;
	friction = 0.8;
	speed = 0.1;
	maxSpeed = 1;
	oldMaxSpeed = maxSpeed;



	//load images
	SDL_Surface* tempSurf = SDL_LoadBMP("playerCircuit.bmp");
	tempTexture = loadTexture("playerCircuit.bmp", tempSurf);
	//tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurf);

	tempSurf = NULL;
	SDL_FreeSurface(tempSurf);




	//create inventory
	inventoryOpen = false;

	inventoryWidth = 4;
	inventoryHeight = inventoryWidth;
	inventoryCapacity = inventoryWidth*inventoryHeight;
	inventoryX = x;
	inventoryY = y;

	double blockWidth = (double)width/(double)inventoryWidth;
	double blockHeight = (double)height/(double)inventoryHeight;

	std::cout << "blockWidth: " << blockWidth << std::endl;

	double gridX = inventoryX, gridY = inventoryY;

	for (int i=0; i<inventoryCapacity; i++)
	{
		for (int w=0; w<inventoryWidth; w++)
		{
			SDL_Rect tempRect = {gridX, gridY, blockWidth, blockHeight};
			inventoryGrid.push_back(tempRect);

			gridX += blockWidth;
		}

		gridY += blockHeight;
		gridX = inventoryX;
	}

} //END Player()

Player::~Player()
{

}

void Player::update()
{
	//check left/right key presses
	if (keys[LEFT])
	{
		vx -= speed;
	}
	else if (keys[RIGHT])
	{
		vx += speed;
	}
	else
	{
		vx *= friction;
	}

	//check up/down key presses
	if (keys[UP])
	{
		vy -= speed;
	}
	else if (keys[DOWN])
	{
		vy += speed;
	}
	else
	{
		vy *= friction;
	}

	//if holding c
	if (keys[c])
	{
		maxSpeed = oldMaxSpeed/2;
	}
	else
	{
		maxSpeed = oldMaxSpeed;
	}


	//don't go over speed limits
	if (vx >= maxSpeed)
		vx = maxSpeed;
	else if (vx <= -maxSpeed)
		vx = -maxSpeed;

	if (vy >= maxSpeed)
		vy = maxSpeed;
	else if (vy <= -maxSpeed)
		vy = -maxSpeed;

	//update position
	oldX = x;
	oldY = y;
	x += vx;
	y += vy;

	//update collision rects etc
	playerRect.x = (x*zoom-xOffset);
	playerRect.y = (y*zoom-yOffset);
	playerRect.w = width*zoom;
	playerRect.h = height*zoom;

	collisionRect.x = x;
	collisionRect.y = y;
	collisionRect.w = width;
	collisionRect.h = height;

	collisionHorz.x = x;
	collisionHorz.y = y+collisionPad;
	collisionHorz.w = width;
	collisionHorz.h = height-collisionPad*2;

	collisionVert.x = x+collisionPad;
	collisionVert.y = y;
	collisionVert.w = width-collisionPad*2;
	collisionVert.h = height;



	//update inventory grid
	inventoryX = x;
	inventoryY = y;

	double blockWidth = (double)width/(double)inventoryWidth;
	double blockHeight = (double)height/(double)inventoryHeight;

	double gridX = inventoryX, gridY = inventoryY;

	for (int i=0; i<inventoryCapacity; i++)
	{
		for (int w=0; w<inventoryWidth; w++)
		{
			inventoryGrid[i].x = (gridX*zoom)-xOffset;
			inventoryGrid[i].y = (gridY*zoom)-yOffset;
			inventoryGrid[i].w = blockWidth*zoom;
			inventoryGrid[i].h = blockHeight*zoom;

			gridX += blockWidth;
			i++;
		}

		gridY += blockHeight;
		gridX = inventoryX;
		i--;
	}
}

void Player::draw()
{
	//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	//SDL_RenderFillRect(renderer, &playerRect);

	SDL_RenderCopy(renderer, tempTexture, NULL, &playerRect);




	//draw grid
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

	for (int i=0; i<inventoryCapacity; i++)
	{
		SDL_RenderDrawRect(renderer, &inventoryGrid[i]);
	}




	/*
	//renders the collision rects
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
	SDL_RenderDrawRect(renderer, &collisionHorz);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
	SDL_RenderDrawRect(renderer, &collisionVert);
	*/
}




SDL_Texture* Player::loadTexture (std::string path, SDL_Surface *currentSurface)
{
	//final image
	SDL_Texture* newTexture = NULL;

	//Load Image at specified path OR use currentSurface if available
	SDL_Surface* loadedSurface;

	if (!currentSurface)
	{
		loadedSurface = SDL_LoadBMP(path.c_str());
	}
	else
	{
		loadedSurface = currentSurface;
	}
	

	if (!loadedSurface)
	{
		printf("Failed to load image %s. SDL_Error: %s\n", path.c_str(), SDL_GetError());
	}
	else
	{
		//convert surface to screen format
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (!newTexture)
		{
			printf("Failed to create texture %s. SDL_Error: %s\n", path.c_str(), SDL_GetError());
		}

		//free old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
} //END loadSurface()
