#ifndef PLAYER_H
#define PLAYER_H

class Player : public Sprite
{
	private:
		bool inventoryOpen;
		int collisionPad;
		SDL_Rect playerRect; //used to draw the player square
		SDL_Texture* tempTexture;

		SDL_Texture* loadTexture (std::string path, SDL_Surface *currentSurface);


		//inventory variables
		int inventoryCapacity;
		int inventoryWidth, inventoryHeight;
		double inventoryX, inventoryY;

		std::vector<SDL_Rect> inventoryGrid;

	public:
		Player();
		~Player();
		void update();
		void draw();
		
		//player rect is the square drawn to represent
		//the character
		SDL_Rect getPlayerRect() {return playerRect;}
		bool getInventoryOpen() {return inventoryOpen;}

		void setInventoryOpen(bool boolIn) {inventoryOpen = boolIn;}
};

#endif