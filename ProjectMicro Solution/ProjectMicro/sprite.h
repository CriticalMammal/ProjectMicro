//FILE : sprite.h
//PROG : Dylan Gallardo
//PURP : header file for sprite.cpp. This is the base class for many
//		 other objects in the program, such as the Player and Bush class.

#ifndef SPRITE_H
#define SPRITE_H

class Sprite
{
	protected:
		double x, y, midX, midY, oldX, oldY, vx, vy,
			  speed, friction, maxSpeed,
			  oldMaxSpeed;
		int width, height, orgWidth, orgHeight;
		SDL_Rect collisionRect;
		SDL_Rect collisionVert;
		SDL_Rect collisionHorz;

	public:
		Sprite();
		~Sprite();

		double getX() {return x;}
		double getY() {return y;}
		double getOldX() {return oldX;}
		double getOldY() {return oldY;}
		double getMidX() {return x + 0.5f*width;}
		double getMidY() {return y + 0.5f*height;}
		double getSpeed() {return speed;}
		int getWidth() {return width;}
		int getHeight() {return height;}
		double getvx() {return vx;}
		double getvy() {return vy;}
		SDL_Rect getCollisionRect() {return collisionRect;}
		SDL_Rect getCollisionRectVert() {return collisionVert;}
		SDL_Rect getCollisionRectHorz() {return collisionHorz;}

		void setX(double newX) {x = newX;}
		void setY(double newY) {y = newY;}
		void setVx(double newVx){vx = newVx;}
		void setVy(double newVy){vy = newVy;}
		void setWidth(int newW){width = newW;}
		void setHeight(int newH){height = newH;}

		void updatePos();
		double randomNumber(double, double);
};

#endif