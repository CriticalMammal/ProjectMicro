#ifndef PLAYER_H
#define PLAYER_H

//#include "sprite.h"
//#include "player.h"

class Player : public Sprite
{
	private:
		double baseX, baseY, oldBaseX, oldBaseY; // Local xy values
		int collisionPad;
		SDL_Rect playerRect; //used to draw the player square
		SDL_Texture* tempTexture;
		SDL_Color randomColor;
		Player *parentChip;

		//motherboard variables (tileMap inside this chip)
		int boardCapacity;
		int boardWidth, boardHeight;
		double boardX, boardY;
		bool colliding;

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
		double getBaseX() {return baseX;}
		double getBaseY() {return baseY;}
		double getOldBaseX() {return oldBaseX;}
		double getOldBaseY() {return oldBaseY;}
		bool getColliding() {return colliding;}
		Player* getParentChip() {return parentChip;}

		void setBaseX(double baseXIn) {baseX = baseXIn;}
		void setBaseY(double baseYIn) {baseY = baseYIn;}
		void setOldBaseX(double baseXIn) {oldBaseX = baseXIn;}
		void setOldBaseY(double baseYIn) {oldBaseY = baseYIn;}
		void setParentChip(Player *newParent) {parentChip = newParent;}
		void setColliding(bool newBool) {colliding = newBool;}
};

#endif