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
	
}

Player::~Player()
{

}

void Player::initializeChip(double xIn, double yIn, double wIn, double hIn, double newSpeed)
{
	x = xIn;
	y = yIn;
	oldX = x;
	oldY = y;
	width = wIn;
	height = hIn;

	colliding = false;

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
	speed = newSpeed;;
	maxSpeed = 1;
	oldMaxSpeed = maxSpeed;

	randomColor.r = randomNumber(0, 255);
	randomColor.g = randomNumber(0, 255);
	randomColor.b = randomNumber(0, 255);
	randomColor.a = 255;
	
	boardWidth = 20;
	boardHeight = boardWidth;
	boardCapacity = boardWidth*boardHeight;
	boardX = x;
	boardY = y;
}


void Player::initializeBoard(SDL_Renderer &renderer, std::string mapTxtFile)
{
	double blockWidth = (double)width/(double)boardWidth;
	double blockHeight = (double)height/(double)boardHeight;

	motherBoard.initialize(mapTxtFile, boardHeight, boardWidth, blockHeight, blockWidth, &renderer);
	motherBoard.setX(x);
	motherBoard.setY(y);
}


void Player::update()
{
	updateCollisionRects();
	updateMotherboard();
	handleCollisions();
	updateInternalChips();
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

	//updateInternalChips();

} //END handleKeys()


void Player::updateCollisionRects()
{
	playerRect.x = x*zoom - xOffset*zoom + SCREEN_WIDTH/2;
	playerRect.y = y*zoom - yOffset*zoom + SCREEN_HEIGHT/2;
	playerRect.w = width*zoom;
	playerRect.h = height*zoom;

	collisionRect.x = x;
	collisionRect.y = y;
	collisionRect.w = width;
	collisionRect.h = height;

	collisionHorz.x = x*zoom - xOffset*zoom + SCREEN_WIDTH/2;
	collisionHorz.y = y*zoom - yOffset*zoom + SCREEN_HEIGHT/2;
	collisionHorz.w = width*zoom;
	collisionHorz.h = (height*zoom);

	collisionVert.x = x+collisionPad;
	collisionVert.y = y;
	collisionVert.w = (width)-collisionPad*2;
	collisionVert.h = height;
} // END updateCollisionRects()


void Player::handleCollisions()
{
	if (chip != NULL && parentChip != NULL)
	{
		colliding = false;

		if (parentChip->motherBoard.checkCollision(playerRect))
		{
			setX(oldX);
			setY(oldY);
			setVx(0);
			setVy(0);

			colliding = true;
		}
	}

	/*
	//check for player collision with the tile map
	if (chip != NULL && parentChip != NULL)
	{
		chip->setColliding(false);
		
		if (motherBoard.checkCollision(chip->getPlayerRect()))
		{
			chip->setX(chip->getOldX());
			chip->setY(chip->getOldY());
			chip->setVx(0);
			chip->setVy(0);

			chip->setColliding(true);
		}
	}
	*/
}


void Player::updateInternalChips()
{
	//update internal chip position (if it's here)
	if (chip != NULL && getColliding() == false)
	{
		chip->setX(chip->getX()+vx);
		chip->setY(chip->getY()+vy);
	}
}


void Player::updateMotherboard()
{
	// Keep this
	motherBoard.setX(x*zoom - xOffset*zoom + SCREEN_WIDTH/2);
	motherBoard.setY(y*zoom - yOffset*zoom + SCREEN_HEIGHT/2);

	if (motherBoard.getBlockW() == 0 || motherBoard.getBlockH() == 0)
	{
		motherBoard.setBlockW((double)width/(double)boardWidth);
		motherBoard.setBlockH((double)height/(double)boardHeight);
	}

} // END updateMotherboard()


void Player::draw(SDL_Renderer *renderer)
{
	// Draw Chip Background
	SDL_SetRenderDrawColor(renderer, randomColor.r, randomColor.g, randomColor.b, 255);
	SDL_RenderFillRect(renderer, &playerRect);

	// Draw Motherboard
	SDL_Rect screenRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	if (width*zoom > 2)
	{
		motherBoard.drawTileMap(screenRect, renderer);
	}
} // END draw()

double Player::getBlockW() 
{
	return (double)width*zoom/(double)boardWidth;
}

double Player::getBlockH()
{
	return (double)height*zoom/(double)boardHeight;
}