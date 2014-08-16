//FILE : sprite.cpp
//PROG : Dylan Gallardo
//PURP : This is primarily used so that other classes can inherit basic features
//		 a sprite would need. Though in some cases it may be used to initialize a very
//		 simple sprite.

#include <SDL.h>
#include <stdlib.h>

#include "definitions.h"
#include "sprite.h"


Sprite::Sprite() {}

Sprite::~Sprite() {}

//come up with a better solution to distribute the random
//numbers evenly. rand() doesn't evenly distribute!
double Sprite::randomNumber(double Min, double Max)
{
    return ((double(rand()) / double(RAND_MAX)) * (Max - Min)) + Min;
}