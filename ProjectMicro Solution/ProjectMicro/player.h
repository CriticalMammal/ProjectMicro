#ifndef PLAYER_H
#define PLAYER_H

class Player : public Sprite
{
	private:
		int collisionPad;
		SDL_Rect playerRect; //used to draw the player square
		SDL_Texture* tempTexture;
		SDL_Color randomColor;

		//motherboard variables (tileMap inside this chip)
		int boardCapacity;
		int boardWidth, boardHeight;
		double boardX, boardY;

		std::vector<SDL_Rect> boardGrid;

		void updateCollisionRects();
		void updateMotherboard();

	public:
		TileMap motherBoard;

		Player();
		~Player();
		void initializeChip(double xIn, double yIn, double wIn, double hIn, double newSpeed);
		void initializeBoard(SDL_Renderer &renderer);
		void update();
		void handleKeys();
		void draw(SDL_Renderer *renderer);
		
		//player rect is the square drawn to represent
		//the character
		SDL_Rect getPlayerRect() {return playerRect;}
		double getBlockW();
		double getBlockH();
};

#endif