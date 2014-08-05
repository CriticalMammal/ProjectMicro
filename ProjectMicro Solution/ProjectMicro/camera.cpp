//FILE : camera.cpp
//PROG : Dylan Gallardo
//PURP : camera.cpp scrolls the tile-based background to positions based on
//		 the player's location. The background itself is handled by mappy_A5.c
//		 tile background created by Mappy, (which is an open source tile mapping program)

#include <SDL.h>
#include <stdlib.h>
#include<iostream>
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
	speed = 0.01;
	maxSpeed = .2;
	friction = 0.1;
	vx = 0;
	vy = 0;
	accuracy = 5;			//how close the camera moves to the points
	motion = 0;				//amount of camera motion/drift
	cameraPause = 0.05*FPS; //frames to wait to renew moveTo points

	zoomSpeed = 0.00001;
	zoomAccuracy = 0;
	zoomMaxSpeed = maxSpeed/1000;
	zoomToPoint = 1;

	framesWaited = 0;
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
	double zoomDist = abs(zoom-zoomToPoint);

	zoomSpeed = zoomDist / 200;
	zoomMaxSpeed = zoomDist / 5;

	// Handle zooming
	if (zoom < zoomToPoint - zoomAccuracy)
		zoomVelocity += zoomSpeed;
	else if (zoom > zoomToPoint + zoomAccuracy)
		zoomVelocity -= zoomSpeed;
	else
		zoomVelocity *= friction;

	
	if (zoomVelocity > zoomMaxSpeed)
		zoomVelocity = zoomMaxSpeed;
	else if (zoomVelocity < -zoomMaxSpeed)
		zoomVelocity = -zoomMaxSpeed;

	zoom += zoomVelocity;

	if (zoom <= 0.2)
		zoom = 0.2;
	else if (zoom >= 50)
		zoom = 50;
}


//scroll camera/screen towards the defined moveTo coordinates
void Camera::scrollScreen()
{
	// Sqrt could be substituted with Manhattan Dist
	double distance = sqrt(pow(double(moveToPointX-xOffset), 2) + pow(double(moveToPointY-yOffset), 2));

	speed = distance / 60;
	maxSpeed = distance / 5;

	// Test what directions to scroll screen
	if (xOffset < moveToPointX - accuracy)		// Too far left
		vx += speed;
	else if (xOffset > moveToPointX + accuracy)	// Too far right
		vx -= speed;
	else
		vx *= friction;

	if (yOffset < moveToPointY - accuracy)		// Too far left
		vy += speed;
	else if (yOffset > moveToPointY + accuracy) // Too far right
		vy -= speed;
	else
		vy *= friction;


	// Speed limits
	if (vx > maxSpeed)
		vx = maxSpeed;
	else if (vx < -maxSpeed)
		vx = -maxSpeed;
	if (vy > maxSpeed)
		vy = maxSpeed;
	else if (vy < -maxSpeed)
		vy = -maxSpeed;
	

	xOffset += vx;
	yOffset += vy;

	xOffset += zoomVelocity*followedObject->getMidX();
	yOffset += zoomVelocity*followedObject->getMidY();
}



void Camera::updateTimer()
{
	framesWaited ++;

	if (framesWaited >= cameraPause)
	{
		newMoveToPoint(followedObject);
		framesWaited = 0;
	}
}


// Called every (cameraPause) seconds
void Camera::newMoveToPoint(Sprite *sprite)
{
	double zoomDiff = zoomToPoint + ((zoom-zoomToPoint)/2);
	followedObject = sprite;

	moveToPointX = ((sprite->getMidX()*zoom)-0.5*SCREEN_WIDTH) + randomNumber(-motion, motion);
	moveToPointY = ((sprite->getMidY()*zoom)-0.5*SCREEN_HEIGHT) + randomNumber(-motion, motion);
}



void Camera::newZoom(double newZoom, double speed, double maxSpeed, double accuracy)
{
	zoomToPoint = newZoom;

	zoomSpeed = speed;
	zoomMaxSpeed = maxSpeed;
	zoomAccuracy = accuracy;
}



//come up with a better solution to distribute the random
//numbers evenly. rand() doesn't evenly distribute!
double Camera::randomNumber(double Min, double Max)
{
    return ((double(rand()) / double(RAND_MAX)) * (Max - Min)) + Min;
}