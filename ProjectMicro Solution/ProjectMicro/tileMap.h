#ifndef TILEMAP_H
#define TILEMAP_H


class TileMap
{
	private:
		SDL_Surface *tileSheet;
		std::vector<Tile*> blocks;

		std::vector<int> tileMap;
		std::vector<SDL_Rect> tileRects;
		std::string mapFileName;
		int mapH, mapW, tilePad;
		double x, y, blockH, blockW;
		//SDL_Renderer *renderer;

		SDL_Texture* loadTexture(std::string, SDL_Surface*, SDL_Renderer *renderer);
		float randomNumber(float, float);
		bool collisionDetect(SDL_Rect, SDL_Rect);
		int getTileElementAt(int x, int y);
		std::vector<int> getTilesInRect(SDL_Rect rect1);
		void updateTileRects();
		int getTileElementTrait(int element, int trait);

	public:
		TileMap();
		~TileMap();

		void initialize(std::string fileLocation, int mapHeight, int mapWidth, double blockHeight, double blockWidth, SDL_Renderer *renderer);
		void drawTileMap(SDL_Rect, SDL_Renderer *renderer);

		//finds tile at x/y coordinates and then returns
		//the trait value for specified trait
		int getTileTraitAt(int x, int y, int trait);
		SDL_Rect getTileRectAt(int x, int y);
		bool checkCollision(SDL_Rect rect1);
		void changeTileAt(int x, int y, int blockType);
		std::vector<pathCoord> pathFind(double startX, double startY, double endX, double endY);
		void saveMapFile();

		double getBlockW() {return blockW;}
		double getBlockH() {return blockH;}

		void setX(int newX) {x = newX;}
		void setY(int newY) {y = newY;}
		void setBlockW(double newW) {blockW = newW;}
		void setBlockH(double newH) {blockH = newH;}

		std::vector<Tile*> getBlocks() {return blocks;}
};

#endif