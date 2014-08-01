#ifndef TILE_H
#define TILE_H

class Tile
{
	private:
		uint16_t collision, trait1, trait2, trait3, trait4;
		SDL_Texture *tileTexture;

	public:
		Tile();
		~Tile();

		//set functions
		void setCollision(uint16_t newColl) {collision = newColl;}
		void setTrait1(uint16_t newTrait) {trait1 = newTrait;}
		void setTrait2(uint16_t newTrait) {trait2 = newTrait;}
		void setTrait3(uint16_t newTrait) {trait3 = newTrait;}
		void setTrait4(uint16_t newTrait) {trait4 = newTrait;}

		void settileTexture(SDL_Texture *newTexture) {tileTexture = newTexture;}


		//get functions
		uint16_t getCollision() {return collision;}
		uint16_t getTrait1() {return trait1;}
		uint16_t getTrait2() {return trait2;}
		uint16_t getTrait3() {return trait3;}
		uint16_t getTrait4() {return trait4;}

		SDL_Texture *gettileTexture() {return tileTexture;}

};

#endif