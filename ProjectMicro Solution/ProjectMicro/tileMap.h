#ifndef TILEMAP_H
#define TILEMAP_H


class TileMap
{
	private:
		SDL_Surface *tileSheet;
		std::vector<Tile*> blocks;

		std::vector<int> tileMap;
		std::string mapFileName;
		int mapH, mapW, blockH, blockW, tilePad;
		double x, y;
		//SDL_Renderer *renderer;

		SDL_Texture* loadTexture(std::string, SDL_Surface*, SDL_Renderer *renderer);
		float randomNumber(float, float);
		bool collisionDetect(SDL_Rect, SDL_Rect);
		int getTileElementAt(int x, int y);
		

	public:
		TileMap();
		~TileMap();

		void initialize(std::string fileLocation, int mapHeight, int mapWidth, int blockHeight, int blockWidth, SDL_Renderer *renderer);
		void drawTileMap(SDL_Rect, SDL_Renderer *renderer);

		//finds tile at x/y coordinates and then returns
		//the trait value for specified trait
		int getTileTraitAt(int x, int y, int trait);
		SDL_Rect getTileRectAt(int x, int y);
		bool checkCollision(SDL_Rect);
		void changeTileAt(int x, int y, int blockType);
		std::vector<pathCoord> pathFind(double startX, double startY, double endX, double endY);
		void saveMapFile();


		void setX(int newX) {x = newX;}
		void setY(int newY) {y = newY;}

		std::vector<Tile*> getBlocks() {return blocks;}
};

#endif