#ifndef PLAYER_H
#define PLAYER_H

//#include "sprite.h"
//#include "player.h"

class Player : public Sprite
{
	private:
		int collisionPad;
		SDL_Rect playerRect; //used to draw the player square
		SDL_Texture* tempTexture;
		Player *parentChip;

		//motherboard variables (tileMap inside this chip)
		int boardCapacity;
		int boardWidth, boardHeight;
		double boardX, boardY;
		bool chipPowered;

		std::vector<SDL_Rect> boardGrid;

		void updateCollisionRects();
		void handleCollisions();
		void updateMotherboard();
		void updateInternalChips();

	public:
		TileMap motherBoard;
		Player *chip;

		Player();
		~Player();
		void initializeChip(double xIn, double yIn, double wIn, double hIn, double newSpeed);
		void initializeBoard(SDL_Renderer &renderer, std::string mapTxtFile);
		void update();
		void handleKeys();
		void draw(SDL_Renderer *renderer);
		
		//player rect is the square drawn to represent
		//the character
		SDL_Rect getPlayerRect() {return playerRect;}
		double getBlockW();
		double getBlockH();
		Player* getParentChip() {return parentChip;}
		bool getChipPowerState() {return chipPowered;}

		void setParentChip(Player *newParent) {parentChip = newParent;}
		void setChipPoweredState(bool newState) {chipPowered = newState;}
};

#endif