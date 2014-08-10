//FILE : camera.cpp
//PROG : Dylan Gallardo
//PURP : camera.cpp scrolls the tile-based background to positions based on
//		 the player's location. The background itself is handled by mappy_A5.c
//		 tile background created by Mappy, (which is an open source tile mapping program)

#include <SDL.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include "definitions.h"
#include "sprite.h"
#include "camera.h"

extern bool keys[];
extern double xOffset, yOffset, zoom;



Camera::Camera()
{
	followedObject = NULL;
	
	moveToPointX = 0;
	moveToPointY = 0;
	xOffset = moveToPointX;
	yOffset = moveToPointY;
	cameraPause = 0.05*FPS; //frames to wait to renew moveTo points

	motion = 2;				//amount of camera motion/drift
	additionalMotionX = 0;
	additionalMotionY = 0;
	shakeDelayRange = 2;
	shakeDelay = randomNumber(0, shakeDelayRange)*FPS;

	zoomToPoint = 1;

	framesWaited = 0;
	movementTimer = 0;
	shakeTimer = 0;

	lerpAmt = 0.0;
	zoomLerp = 0.0;

	xScroll = xOffset;
	yScroll = yOffset;

	zoomScroll = zoom;
}



Camera::~Camera()
{

}



void Camera::updateCamera()
{
	updateZoom();
	scrollScreen();
	updateTimer();
}


void Camera::updateZoom()
{
	if (zoom != zoomToPoint)
	{
		zoomLerp = 0.0;
	}

	if (zoomLerp < 1.0)
	{
		zoomLerp += 0.05;
	}

	zoomScroll = lerp(zoom, zoomLerp, zoomToPoint);

	zoom = zoomScroll;
}


//scroll camera/screen towards the defined moveTo coordinates
void Camera::scrollScreen()
{
	//xOffset += zoomVelocity*followedObject->getMidX();
	//yOffset += zoomVelocity*followedObject->getMidY();

	if (xOffset != moveToPointX && yOffset != moveToPointY)
	{
		lerpAmt = 0;
	}

	if (lerpAmt < 1.0)
	{
		lerpAmt += 0.1;
	}

	// Linear Interpolation between points
	xScroll = lerp(xOffset, lerpAmt, moveToPointX);
	yScroll = lerp(yOffset, lerpAmt, moveToPointY);

	// Move screen
	xOffset = xScroll;
	yOffset = yScroll;
}



void Camera::updateTimer()
{
	//framesWaited ++;
	movementTimer ++;
	shakeTimer ++;

	if (movementTimer >= cameraPause)
	{
		newMoveToPoint(followedObject);
		movementTimer = 0;
	}

	if (shakeTimer >= shakeDelay)
	{
		shakeDelay = randomNumber(0, shakeDelayRange)*FPS;
		changeMotion();
		shakeTimer = 0;
	}
}


void Camera::changeMotion()
{
	additionalMotionX = randomNumber(-motion, motion);
	additionalMotionY = randomNumber(-motion, motion);
}


// Called every (cameraPause) seconds
void Camera::newMoveToPoint(Sprite *sprite)
{
	double zoomDiff = zoomToPoint + ((zoom-zoomToPoint)/2);
	followedObject = sprite;

	moveToPointX = ((sprite->getMidX()*zoom)-0.5*SCREEN_WIDTH) + additionalMotionX;
	moveToPointY = ((sprite->getMidY()*zoom)-0.5*SCREEN_HEIGHT) + additionalMotionY;
}



double Camera::lerp(double x, double t, double y)
{
	return x * (1-t) + y*t;
}


void Camera::newZoom(double newZoom)
{
	zoomToPoint = newZoom;
}



//come up with a better solution to distribute the random
//numbers evenly. rand() doesn't evenly distribute!
double Camera::randomNumber(double Min, double Max)
{
    return ((double(rand()) / double(RAND_MAX)) * (Max - Min)) + Min;
}