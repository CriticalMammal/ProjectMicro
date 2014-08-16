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



void TileMap::initialize(std::string fileLocation, int mapHeight, int mapWidth, double blockHeight, double blockWidth, SDL_Renderer *renderer)
{
	mapFileName = fileLocation;
	mapH = mapHeight;
	mapW = mapWidth;
	blockH = blockHeight;
	blockW = blockWidth;
	tilePad = 1;
	//renderer = &mainRenderer;

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


	// Populate tileRect vector
	float realX = x;
	float realY = y;
	float orgRealX = realX;

	for (int i=0; i<tileMap.size(); i++)
	{
		for (int w=0; w<mapWidth; w++)
		{
			SDL_Rect tempRect = {realX, realY, blockW, blockH};
			tileRects.push_back(tempRect);

			realX += blockWidth;
			i++;
		}

		realX = orgRealX;
		realY += blockHeight;
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
		SDL_Texture *tempTexture = loadTexture("emptyString", blockSurface[i], renderer);
		blocks.push_back(new Tile);
		blocks.back()->settileTexture(tempTexture);

		string traitLine;

		if (tileTraitFile.is_open())
		{
			getline(tileTraitFile, traitLine);
		}
		else cout << "Unable to open file";

		blocks.back()->setCollision(traitLine[0] - '0');
		blocks.back()->setTrait1(traitLine[1] - '0');
		blocks.back()->setTrait2(traitLine[2] - '0');
		blocks.back()->setTrait3(traitLine[3] - '0');
		blocks.back()->setTrait4(traitLine[4] - '0');

		//use this when you expand the functionality to handle longer digits than 0-9
		//cout << atoi(traitLine.c_str()) << endl << endl;
	}

	tileTraitFile.close();

	for (int i=blockSurface.size()-1; i<=0; i=blockSurface.size()-1)
	{
		SDL_FreeSurface(blockSurface[i]);
	}
}



void TileMap::updateTileRects()
{
	float realX = x;
	float realY = y;
	float orgRealX = realX;

	float adjBlockW = blockW*zoom;
	float adjBlockH = blockH*zoom;


	// Update Tile rect positions
	for (int i=0; i<tileMap.size()-1; i++)
	{
		for (int w = 0; w<mapW; w++)
		{
			tileRects[i].x = realX;
			tileRects[i].y = realY;
			tileRects[i].w = adjBlockW+tilePad;
			tileRects[i].h = adjBlockH+tilePad;

			realX += adjBlockW;
			i++;
		}

		realX = x;
		realY += adjBlockH;
		i--;
	}
}



void TileMap::drawTileMap(SDL_Rect screenRect, SDL_Renderer *renderer)
{
	updateTileRects(); // Temporary, shouldn't call this from draw()

	vector<int> tilesToDraw;
	tilesToDraw = getTilesInRect(screenRect);

	
	// Draw tile Rect grid for example
	/*
	for (int i=0; i<tileMap.size()-1; i++)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &tileRects[i]);
	}
	*/

	if (tilesToDraw.size() <= 0)
	{
		return;
	}

	// Draw elements
	for (int i=0; i<tilesToDraw.size(); i++)
	{
			if (tileMap[tilesToDraw[i]] == 1)
			{
				SDL_RenderCopy(renderer, blocks[0]->gettileTexture(), NULL, &tileRects[tilesToDraw[i]]);
			}
			else if (tileMap[tilesToDraw[i]] == 2)
			{
				SDL_RenderCopy(renderer, blocks[1]->gettileTexture(), NULL, &tileRects[tilesToDraw[i]]);
			}
			else if (tileMap[tilesToDraw[i]] == 3)
			{
				SDL_RenderCopy(renderer, blocks[2]->gettileTexture(), NULL, &tileRects[tilesToDraw[i]]);
			}
			else
			{
				SDL_RenderCopy(renderer, blocks[3]->gettileTexture(), NULL, &tileRects[tilesToDraw[i]]);
			}
	}

} // END draw()



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



int TileMap::getTileElementAt(int xIn, int yIn)
{
	double width = 1;
	double height = 1;

	SDL_Rect rect1 = {xIn, yIn, width, height};

	for (int i=0; i<tileMap.size()-1; i++)
	{
		if (collisionDetect(rect1, tileRects[i]))
		{
			return i;
		}
	}

	return 0;
}

// Very similar function to the function below
int TileMap::getTileElementTrait(int element, int trait)
{
	if (element > 0 && element < tileMap.size())
	{
		//test what trait you should be returning
		switch(trait)
		{
			case 0:
				return blocks[tileMap[element]-1]->getCollision();
				break;
			case 1:
				return blocks[tileMap[element]-1]->getTrait1();
				break;
			case 2:
				return blocks[tileMap[element]-1]->getTrait2();
				break;
			case 3:
				return blocks[tileMap[element]-1]->getTrait3();
				break;
			case 4:
				return blocks[tileMap[element]-1]->getTrait4();
				break;
			default:
				return -2;
				break;
		}
	}
	else
		return -1;
}



int TileMap::getTileTraitAt(int xIn, int yIn, int trait)
{
	int tileElement = getTileElementAt(xIn, yIn);
	//cout << ": tileElement " << tileElement << ": ";

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



vector<int> TileMap::getTilesInRect(SDL_Rect rect1)
{
	vector<int> collidingElements;

	for (int i=0; i<tileMap.size()-1; i++)
	{
		if (collisionDetect(rect1, tileRects[i]))
		{
			collidingElements.push_back(i);
		}
	}

	return collidingElements;
}



bool TileMap::checkCollision(SDL_Rect rect1)
{
	vector<int> possibleCollidingTiles = getTilesInRect(rect1);


	for (int i=0; i<possibleCollidingTiles.size(); i++)
	{
		if (getTileElementTrait(possibleCollidingTiles[i], 0) == 1)
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



SDL_Texture* TileMap::loadTexture (std::string path, SDL_Surface *currentSurface, SDL_Renderer *renderer)
{

	//final image
	SDL_Texture* newTexture = nullptr;

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
		cout << "Failed to load image " << path.c_str() << ". SDL_Error: " << SDL_GetError() << endl;
	}
	else
	{
		//convert surface to screen format
		if (!(newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface)))
		{
			cout << "Failed to create texture " << path.c_str() << ". SDL_Error: " << SDL_GetError() << endl;
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