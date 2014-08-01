#include <SDL.h>

#include "tile.h"

using namespace std;

Tile::Tile()
{
	collision = 0;
	trait1 = 0;
	trait2 = 0;
	trait3 = 0;
	trait4 = 0;
	tileTexture = NULL;
}

Tile::~Tile()
{
	//SDL_DestroyTexture(tileTexture);
	//tileTexture = NULL;
}