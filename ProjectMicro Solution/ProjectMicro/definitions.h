//File: definitions.h
//Prog: Dylan Gallardo
//Purp: defines some common constants to use


#ifndef DEFINITIONS_H
#define DEFINITIONS_H


// Stores a coordinate and a distance value
struct pathCoord
{
	double x, y;
	int distance;

	pathCoord(double paramX, double paramY, int dist) : x(paramX), y(paramY), distance(dist) {};
};

// Stores a tile # reference and distance/heuristic dist
struct pathTile
{
	int tileElement, distance;
	double heuristic;

	pathTile(int element, int dist, double inHeuristic) : tileElement(element), distance(dist), heuristic(inHeuristic) {};
};

// Used to check if pathTile heuristic is less than another pathTile heuristic
// Returns a bool value
struct lessThanHeuristic
{
	bool operator() (const pathTile& tile1, const pathTile& tile2)
	{
		return (tile1.heuristic < tile2.heuristic);
	}
};


#define SCREEN_WIDTH 640	// Screen width in pixels
#define SCREEN_HEIGHT 480	// Screen height in pixels
#define FPS 60				// Frames Per Second lock

enum {LEFT, RIGHT, UP, DOWN, c, TAB};

#endif