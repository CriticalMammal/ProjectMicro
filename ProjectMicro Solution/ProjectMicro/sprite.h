//FILE : sprite.h
//PROG : Dylan Gallardo
//PURP : header file for sprite.cpp. This is the base class for many
//		 other objects in the program, such as the Player and Bush class.

#ifndef SPRITE_H
#define SPRITE_H

class Sprite
{
	protected:
		double baseX, baseY, oldBaseX, oldBaseY; // Local xy values
		double x, y, midX, midY, oldX, oldY, vx, vy,
			  speed, friction, maxSpeed, oldMaxSpeed;
		double width, height, orgWidth, orgHeight;
		SDL_Rect collisionRect;

	public:
		Sprite();
		~Sprite();

		double getX() {return x;}
		double getY() {return y;}
		double getOldX() {return oldX;}
		double getOldY() {return oldY;}
		double getMidX() {return x + 0.5f*width;}
		double getMidY() {return y + 0.5f*height;}
		double getBaseX() {return baseX;}
		double getBaseY() {return baseY;}
		double getOldBaseX() {return oldBaseX;}
		double getOldBaseY() {return oldBaseY;}

		double getSpeed() {return speed;}
		double getMaxSpeed() {return maxSpeed;}
		double getWidth() {return width;}
		double getHeight() {return height;}
		double getvx() {return vx;}
		double getvy() {return vy;}
		void setBaseX(double baseXIn) {baseX = baseXIn;}
		void setBaseY(double baseYIn) {baseY = baseYIn;}
		void setOldBaseX(double baseXIn) {oldBaseX = baseXIn;}
		void setOldBaseY(double baseYIn) {oldBaseY = baseYIn;}
		SDL_Rect getCollisionRect() {return collisionRect;}

		void setX(double newX) {x = newX;}
		void setY(double newY) {y = newY;}
		void setOldX(double newX) {oldX = newX;}
		void setOldY(double newY) {oldY = newY;}
		void setVx(double newVx){vx = newVx;}
		void setVy(double newVy){vy = newVy;}
		void setWidth(double newW){width = newW;}
		void setHeight(double newH){height = newH;}
		void setMaxSpeed (double newMax) {maxSpeed = newMax;}
		
		double randomNumber(double, double);
};

#endif