#include <SDL.h>
#include <string>
#include <vector>
#include <iostream>

#include "definitions.h"
#include "sprite.h"
#include "tile.h"
#include "tileMap.h"
#include "player.h"

extern bool keys[];
extern double xOffset, yOffset, zoom;
//extern SDL_Renderer* renderer;

Player::Player()
{
	x = 60*zoom;
	y = 60*zoom;

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

	randomColor.r = randomNumber(0, 255);
	randomColor.g = randomNumber(0, 255);
	randomColor.b = randomNumber(0, 255);
	randomColor.a = 255;



	//create motherboard
	/*
	boardWidth = 4;
	boardHeight = boardWidth;
	boardCapacity = boardWidth*boardHeight;
	boardX = x;
	boardY = y;

	double blockWidth = (double)width/(double)boardWidth;
	double blockHeight = (double)height/(double)boardHeight;

	std::cout << "blockWidth: " << blockWidth << std::endl;

	double gridX = boardX, gridY = boardY;

	for (int i=0; i<boardCapacity; i++)
	{
		for (int w=0; w<boardWidth; w++)
		{
			SDL_Rect tempRect = {gridX, gridY, blockWidth, blockHeight};
			boardGrid.push_back(tempRect);

			gridX += blockWidth;
			i++;
		}

		gridY += blockHeight;
		gridX = boardX;
		i--;
	}
	*/
	
	boardWidth = 10;
	boardHeight = boardWidth;
	boardCapacity = boardWidth*boardHeight;
	boardX = x;
	boardY = y;
	
} //END Player()

Player::~Player()
{

}


void Player::initializeBoard(SDL_Renderer &renderer)
{
	double blockWidth = (double)width/(double)boardWidth;
	double blockHeight = (double)height/(double)boardHeight;

	motherBoard.initialize("none", boardHeight, boardWidth, blockHeight, blockWidth, &renderer);
	motherBoard.setX(x);
	motherBoard.setY(y);
}


void Player::update()
{
	updateCollisionRects();
	updateMotherboard();
}


void Player::handleKeys()
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
} //END handleKeys()


void Player::updateCollisionRects()
{
	//playerRect.x = x*zoom-xOffset;
	//playerRect.y = y*zoom-yOffset;
	playerRect.w = width*zoom;
	playerRect.h = height*zoom;
	playerRect.x = x-xOffset - (width*zoom/2);
	playerRect.y = y-yOffset - (height*zoom/2);
	//playerRect.x = (x*zoom - xOffset) + (SCREEN_WIDTH/2);
	//playerRect.y = (y*zoom - yOffset) + (SCREEN_HEIGHT/2);

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
} // END updateCollisionRects()


void Player::updateMotherboard()
{
	// Old board grid implementation
	/*
	boardX = x;
	boardY = y;

	double blockWidth = (double)width/(double)boardWidth;
	double blockHeight = (double)height/(double)boardHeight;

	double gridX = boardX, gridY = boardY;

	for (int i=0; i<boardCapacity; i++)
	{
		for (int w=0; w<boardWidth; w++)
		{
			boardGrid[i].x = (gridX*zoom)-xOffset;
			boardGrid[i].y = (gridY*zoom)-yOffset;
			boardGrid[i].w = blockWidth*zoom;
			boardGrid[i].h = blockHeight*zoom;

			gridX += blockWidth;
			i++;
		}

		gridY += blockHeight;
		gridX = boardX;
		i--;
	}
	*/

	// Keep this
	motherBoard.setX(x*zoom-xOffset);
	motherBoard.setY(y*zoom-yOffset);

	//motherBoard.setX(width*zoom/2);
	//motherBoard.setY(height*zoom/2);
} // END updateMotherboard()


void Player::draw(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, randomColor.r, randomColor.g, randomColor.b, 255);
	SDL_RenderFillRect(renderer, &playerRect);



	//draw grid
	/*
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

	for (int i=0; i<boardCapacity; i++)
	{
		SDL_RenderDrawRect(renderer, &boardGrid[i]);
	}
	*/

	SDL_Rect screenRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	motherBoard.drawTileMap(screenRect, renderer);
} // END draw()