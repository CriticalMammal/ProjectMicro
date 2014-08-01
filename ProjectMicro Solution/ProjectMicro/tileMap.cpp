//tileMap.cpp - source file for the game's tile map. 

#include <SDL.h>
#include <fstream> //is a part of <stdio>?
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>

#include "definitions.h"
#include "tile.h"
#include "tileMap.h"

using namespace std;
extern double zoom;



TileMap::TileMap()
{

}



TileMap::~TileMap()
{
	for (int i=0; i<blocks.size(); i++)
	{
		blocks[i]->~Tile();
	}
}



void TileMap::initialize(std::string fileLocation, int mapHeight, int mapWidth, int blockHeight, int blockWidth, SDL_Renderer &mainRenderer)
{
	mapFileName = fileLocation;
	mapH = mapHeight;
	mapW = mapWidth;
	blockH = blockHeight;
	blockW = blockWidth;
	tilePad = 1;
	renderer = &mainRenderer;

	//first load the map file into the vector tileMap
	int mapData;
	ifstream mapFile(mapFileName);
	
	if (mapFile.is_open())
	{
		while (mapFile >> mapData)
		{
			tileMap.push_back(mapData);
		}

		mapFile.close();
	}
	else //if map not found randomly generate one
	{
		for (int c=0; c<=mapWidth*mapHeight; c++)
		{
			tileMap.push_back(randomNumber(1, 5)); //remember rand(1, 5) = values 1-4 NOT values 1-5
		}
	}


	//"tileSheet.bmp" should be a parameter in the init function
	//to make this class more versatile
	tileSheet = SDL_LoadBMP("tileSheet.bmp");

	//load traits from txt file.
	//"tileTraits.txt" should be a parameter in the init function
	ifstream tileTraitFile;
	tileTraitFile.open ("tileTraits.txt");

	vector<SDL_Surface*> blockSurface;

	//the actual pixel width/height of tiles in tilesheet image
	int importHeight = 16, importWidth = 16;

	//load the tile sheet into seperate surfaces
	for (int h=0; h<tileSheet->h; h+=importHeight)
	{
		for (int w=0; w<tileSheet->w; w+=importWidth)
		{
			//this is weird... you needed to load a temporary tile in order for
			//it to properly blit the tilesheet into parts? otherwise using
			//SDL_CreateRGBSurface() causes it to fill everything by the color of
			//the first pixel
			SDL_Surface *tempSurface = SDL_LoadBMP("tempTile.bmp");
			blockSurface.push_back(tempSurface);

			SDL_Rect clip = {w, h, importWidth, importHeight};

			if (SDL_BlitSurface(tileSheet, &clip, blockSurface.back(), NULL) != 0)
			{
				cout << "Error, SDL_Blit failed: " << SDL_GetError() << endl;
			}
		}
	}

	//make all recently read tiles into textures and free surfaces
	for (int i=0; i<blockSurface.size(); i++)
	{
		SDL_Texture *tempTexture = loadTexture("emptyString", blockSurface[i]);
		blocks.push_back(new Tile);
		blocks.back()->settileTexture(tempTexture);

		string traitLine;

		if (tileTraitFile.is_open())
		{
			getline(tileTraitFile, traitLine);
			cout << traitLine << endl;
		}
		else cout << "Unable to open file";

		blocks.back()->setCollision(traitLine[0] - '0');
		blocks.back()->setTrait1(traitLine[1] - '0');
		blocks.back()->setTrait2(traitLine[2] - '0');
		blocks.back()->setTrait3(traitLine[3] - '0');
		blocks.back()->setTrait4(traitLine[4] - '0');

		cout << traitLine[1] << endl << endl;
		//use this when you expand the functionality to handle longer digits than 0-9
		//cout << atoi(traitLine.c_str()) << endl << endl;
	}

	tileTraitFile.close();

	for (int i=blockSurface.size()-1; i<=0; i=blockSurface.size()-1)
	{
		SDL_FreeSurface(blockSurface[i]);
	}
}



void TileMap::drawTileMap(SDL_Rect rect1)
{
	int tile = 0;
	float tempX = -rect1.x;
	float tempY = -rect1.y;
	int orgX = tempX;
	float adjBlockW = blockW*zoom;
	float adjBlockH = blockH*zoom;

	SDL_Rect screenRect = rect1;
	screenRect.x += 0;
	screenRect.w -= 0;
	screenRect.y += 0;
	screenRect.h -= 0;
	
	//figure out the elements you should be drawing
	int tilesWide = screenRect.x/adjBlockW;
	int tilesHigh = screenRect.y/adjBlockH;

	int elementWidth = (screenRect.x+screenRect.w)/adjBlockW;
	int elementHeight = (screenRect.y+screenRect.h)/adjBlockH;


	int startingElement = tilesHigh*mapW + tilesWide;
	int rowLength = tilesHigh*mapW + elementWidth+1;
	int endElement = (elementHeight)*mapW + (elementWidth);

	// Checks to ensure that it doesn't attempt to draw more than what exists
	if (endElement >= tileMap.size())
	{
		endElement = tileMap.size()-1;
	}
	if (rowLength >= mapW)
	{
		rowLength = mapW;
	}
	
	tempX += tilesWide * adjBlockW;
	orgX = tempX;

	tempY += tilesHigh * adjBlockH;

	
	//draw elements
	for (int i=startingElement; i<endElement; i+=mapW)
	{

		for (int w = i; w<rowLength; w++) 
		{
			tile = w;

			SDL_Rect blockRect = {tempX, tempY, adjBlockW+tilePad, adjBlockH+tilePad};

			if (tileMap[tile] == 1)
			{
				SDL_RenderCopy(renderer, blocks[0]->gettileTexture(), NULL, &blockRect);
			}
			else if (tileMap[tile] == 2)
			{
				SDL_RenderCopy(renderer, blocks[1]->gettileTexture(), NULL, &blockRect);
			}
			else if (tileMap[tile] == 3)
			{
				SDL_RenderCopy(renderer, blocks[2]->gettileTexture(), NULL, &blockRect);
			}
			else
			{
				SDL_RenderCopy(renderer, blocks[3]->gettileTexture(), NULL, &blockRect);
			}

			tempX += adjBlockW;
		}

		tempX = orgX;
		tempY += adjBlockH;
		rowLength += mapW;

		if (i>= 300 && rect1.y>=140)
		{
			i =i;
		}
	}

} //END draw()



void TileMap::saveMapFile()
{
	ofstream externMapFile;
	externMapFile.open("mapFile.txt");

	for (int i=0; i<tileMap.size(); i++)
	{
		//produces a non-human readable mess... add in a way to read 
		//tileMap[i]+' '; later so that the output file is readable by a person
		externMapFile << tileMap[i] << ' ';
	}
	
	externMapFile.close();
}



int TileMap::getTileElementAt(int x, int y)
{
	//If you divide the coordinates by the width/height of each tile...
	//you should be able to find out the element it would be in the vector
	int tilesWide = x/blockW;
	int tilesHigh = y/blockH;

	//each tilesHigh will be an entire row of tiles, aka the mapW
	int tileElement = tilesHigh*mapW + tilesWide;

	return tileElement;
}



int TileMap::getTileTraitAt(int x, int y, int trait)
{
	int tileElement = getTileElementAt(x, y);

	if (tileElement > 0 && tileElement < tileMap.size())
	{
		//test what trait you should be returning
		switch(trait)
		{
			case 0:
				return blocks[tileMap[tileElement]-1]->getCollision();
				break;
			case 1:
				return blocks[tileMap[tileElement]-1]->getTrait1();
				break;
			case 2:
				return blocks[tileMap[tileElement]-1]->getTrait2();
				break;
			case 3:
				return blocks[tileMap[tileElement]-1]->getTrait3();
				break;
			case 4:
				return blocks[tileMap[tileElement]-1]->getTrait4();
				break;
			default:
				return -1;
				break;
		}
	}
	else
		return -1;
}



//pretty much useless with the new collisions implemented. Tile instances could store rects so that
//you can easily obtain x,y,w,h info
SDL_Rect TileMap::getTileRectAt(int x, int y)
{
	//If you divide the coordinates by the width/height of each tile...
	//you should be able to find out the element it would be in the vector
	int tilesWide = x/blockW;
	int tilesHigh = y/blockH;

	int tileX = blockW*tilesWide;
	int tileY = blockH*tilesHigh;

	//each tilesHigh will be an entire row of tiles, aka the mapW
	int tileElement = tilesHigh*mapW + tilesWide;

	if (tileElement > 0 && tileElement < tileMap.size())
	{
		//return rect
		SDL_Rect tileRect = {tileX, tileY, tileX+blockW, tileY+blockH};
		return tileRect;
	}
	else
	{
		SDL_Rect nullRect = {NULL, NULL, NULL, NULL};
		return nullRect;
	}
}



bool TileMap::checkCollision(SDL_Rect rect1)
{
	//some of these probably need renaming to be clear such as
	//elementWidth... since it's not clear what that is exactly
	int tilesWide = rect1.x/blockW;
	int tilesHigh = rect1.y/blockH;

	int elementWidth = (rect1.x+rect1.w)/blockW;
	int elementHeight = (rect1.y+rect1.h)/blockH;

	int startingElement = tilesHigh*mapW + tilesWide;
	int rowLength = tilesHigh*mapW + elementWidth+1;
	int endElement = (elementHeight)*mapW + (elementWidth);


	vector<int> collidingElements;

	for (int i=startingElement; i<=endElement; i+=mapW)
	{
		for (int w = i; w<rowLength; w++) 
		{
			collidingElements.push_back(w);
		}

		rowLength += mapW;
	}

	//weird approach, you needed to find the x,y of an element in the tile array to
	//get the collision rect. But it should be easier than this. The x/y etc. should
	//probably be stored in each tile instance for convenience sake.
	for (int c=0; c<collidingElements.size(); c++)
	{
		int tileElement = collidingElements[c];
		int tileX = (tileElement%mapW) * blockW;
		int tileY = tileElement/mapW * blockH;
		int tileW = tileX + blockW;
		int tileH = tileY + blockH;
		SDL_Rect tileRect = {tileX, tileY, tileW, tileH};

		if (getTileTraitAt(tileX, tileY, 0) == 1 &&
			collisionDetect(rect1, tileRect))
		{
			return true;
		}
	}

	return false;
}



bool TileMap::collisionDetect(SDL_Rect rect1, SDL_Rect rect2)
{
	int left1, right1, top1, bottom1,
		left2, right2, top2, bottom2;

	//get values for rect1
	left1 = rect1.x;
	right1 = left1+rect1.w;
	top1 = rect1.y;
	bottom1 = top1+rect1.h;
	//get values for rect2
	left2 = rect2.x;
	right2 = left2+rect2.w;
	top2 = rect2.y;
	bottom2 = top2+rect2.h;

	//test it
	if (left1 < right2 && right1 > left2 && top1 < bottom2 && bottom1 > top2)
		return true;
	else 
		return false;

} //END collisionDetect



void TileMap::changeTileAt(int x, int y, int blockType)
{
	//check if the block type is valid
	if (blockType+1 > blocks.size())
	{
		return;
	}


	int tileElement = getTileElementAt(x, y);

	if (tileElement > 0 && tileElement < tileMap.size())
	{
		tileMap[tileElement] = blockType+1;
	}
}



vector<pathCoord> TileMap::pathFind(double startX, double startY, double endX, double endY)
{
	//first let's figure out the tile array elements for the start and end
	//that way you can create a path based on the tile positions
	int startTileElement = getTileElementAt(startX, startY);

	int endTileElement = getTileElementAt(endX, endY);


	//create lists/vectors to store the path data
	//pathTile and pathCoord are structs created in definitions.h
	vector<pathTile> openCoordinates;
	vector<pathTile> closedCoordinates;
	vector<pathCoord> pathCoordinates;

	//if the start/end coordinates would result in an invalid path
	if (getTileTraitAt(startX, startY, 0) == 1 ||
		getTileTraitAt(endX, endY, 0) == 1)
	{
		pathCoordinates.push_back(pathCoord(startX, startY, 0));
		return pathCoordinates;
	}
	else if (startTileElement == endTileElement)
	{
		pathCoordinates.push_back(pathCoord(endX, endY, 0));
		return pathCoordinates;
	}

	//start the path finding process
	bool pathFound = false;
	pathTile endReference = pathTile(0, 0, NULL);
	openCoordinates.push_back(pathTile(startTileElement, 0, NULL));

	while (!pathFound)
	{
		int openListSize = openCoordinates.size();

		//if you remove/add elements openCoordinates.size will change?
		for (int t=0; t<openListSize; t++)
		{
			//check if tile is the end tile
			if (openCoordinates.front().tileElement == endTileElement)
			{
				endReference = openCoordinates.front();
				pathFound = true;
				break;
			}

			//check if tile is collidabe
			int tileElement = openCoordinates.front().tileElement;
			int tileX = (tileElement%mapW) * blockW;
			int tileY = tileElement/mapW * blockH;

			if (getTileTraitAt(tileX, tileY, 0) != 1)
			{
				//add this into the closed list
				closedCoordinates.push_back(openCoordinates.front());

				//remove openCoordinates[i] from openCoordinates list
				openCoordinates.erase(openCoordinates.begin());

				//ADD SURROUNDING TILES
				//---------------------
				int lastCoordinate = closedCoordinates.back().tileElement;
				int newDistance = closedCoordinates.back().distance+1;
				
				//tile to the left
				int nextTileX = (tileX+blockW/2)-blockW;
				int nextTileY = tileY+blockH/2;
				int nextTileDistX = abs(endX-nextTileX);
				int nextTileDistY = abs(endY-nextTileY);
				int heuristic = nextTileDistX+nextTileDistY;

				openCoordinates.push_back(pathTile(lastCoordinate-1, newDistance, heuristic));

				//tile to the right
				nextTileX = (tileX+blockW/2)+blockW;
				nextTileY = tileY+blockH/2;
				nextTileDistX = abs(endX-nextTileX);
				nextTileDistY = abs(endY-nextTileY);
				heuristic = nextTileDistX+nextTileDistY;

				openCoordinates.push_back(pathTile(lastCoordinate+1, newDistance, heuristic));

				//tile to the top
				nextTileX = tileX+blockW/2;
				nextTileY = (tileY+blockH/2)-blockH;
				nextTileDistX = abs(endX-nextTileX);
				nextTileDistY = abs(endY-nextTileY);
				heuristic = nextTileDistX+nextTileDistY;
				
				openCoordinates.push_back(pathTile(lastCoordinate-mapW, newDistance, heuristic));

				//tile to the bottom
				nextTileX = tileX+blockW/2;
				nextTileY = (tileY+blockH/2)+blockH;
				nextTileDistX = abs(endX-nextTileX);
				nextTileDistY = abs(endY-nextTileY);
				heuristic = nextTileDistX+nextTileDistY;

				openCoordinates.push_back(pathTile(lastCoordinate+mapW, newDistance, heuristic));
			}
			else
			{
				openCoordinates.erase(openCoordinates.begin());
				t--;
			}

			//remove coordinates from closed/open if they are the
			//same tile and have a larger distance value
			for (int i=0; i<openCoordinates.size(); i++)
			{
				for (int c=0; c<closedCoordinates.size(); c++)
				{
					if (openCoordinates[i].tileElement == closedCoordinates[c].tileElement)
					{
						if (openCoordinates[i].distance >= closedCoordinates[c].distance)
						{
							openCoordinates.erase(openCoordinates.begin()+i);
							i--;
						}
						else
						{
							closedCoordinates.erase(closedCoordinates.begin()+c);
							c--;
						}
					}
				}
			}

			//put the tiles with the lowest heuristic first
			std::sort(openCoordinates.begin(), openCoordinates.end(), lessThanHeuristic());
		}
	}

	//backtrack and find the best path back to the start
	pathFound = false;
	int goodTile = endReference.tileElement;
	int tileDistance = endReference.distance;

	int endTileX = (goodTile%mapW) * blockW;
	int endTileY = goodTile/mapW * blockH;

	//pathCoordinates.push_back(pathCoord(endTileX + blockW/2, endTileY + blockH/2, tileDistance));
	pathCoordinates.push_back(pathCoord(endX, endY, tileDistance));

	//TEMPORARY FIX: set a maximum coordinate limit to catch the infinite loop bug
	int maxCoordinates = 300;

	while (!pathFound)
	{
		int leftTile = goodTile-1;
		int rightTile = goodTile+1;
		int topTile = goodTile-mapW;
		int bottomTile = goodTile+mapW;

		int leftDist = INT_MAX;
		int rightDist = INT_MAX;
		int topDist = INT_MAX;
		int bottomDist = INT_MAX;

		//find the lowest distance from each tile element
		for (int i=0; i<closedCoordinates.size(); i++)
		{
			if (closedCoordinates[i].tileElement == leftTile)
			{
				leftDist = closedCoordinates[i].distance;
			}
			else if (closedCoordinates[i].tileElement == rightTile)
			{
				rightDist = closedCoordinates[i].distance;
			}
			else if (closedCoordinates[i].tileElement == topTile)
			{
				topDist = closedCoordinates[i].distance;
			}
			else if (closedCoordinates[i].tileElement == bottomTile)
			{
				bottomDist = closedCoordinates[i].distance;
			}
		}

		//find out which tile is the best move
		int distArray[4] = {leftDist, rightDist, topDist, bottomDist};
		int tempMinDist = distArray[0];
		goodTile = leftTile;

		for (int i=0; i<4; i++)
		{
			if (tempMinDist > distArray[i])
			{
				tempMinDist = distArray[i];

				switch (i)
				{
					case 0:
						goodTile = leftTile;
						break;
					case 1:
						goodTile = rightTile;
						break;
					case 2:
						goodTile = topTile;
						break;
					case 3:
						goodTile = bottomTile;
						break;
				}
			}
		}

		int tileX = (goodTile%mapW) * blockW;
		int tileY = goodTile/mapW * blockH;
		tileDistance = tempMinDist;

		if (goodTile == startTileElement)
		{
			tileX = startX;
			tileY = startY;
			pathCoordinates.push_back(pathCoord(tileX, tileY, tileDistance));
			pathFound = true;
			break;
		}
		else
		{
			pathCoordinates.push_back(pathCoord(tileX + blockW/2, tileY + blockH/2, tileDistance));

			if (pathCoordinates.size() > maxCoordinates)
			{
				vector<pathCoord> failedPath;
				failedPath.push_back(pathCoord(startX, startY, 0));
				return failedPath;
			}
		}
	}
	
	//quickly flip the vector elements since they are listed
	//backwards. std::reverse is included in <algorithm>
	reverse(pathCoordinates.begin(), pathCoordinates.end());

	return pathCoordinates;

}//END pathFind()



SDL_Texture* TileMap::loadTexture (std::string path, SDL_Surface *currentSurface)
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



float TileMap::randomNumber(float Min, float Max)
{
    return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}