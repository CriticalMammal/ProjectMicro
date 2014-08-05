//FILE : camera.h
//PROG : Dylan Gallardo
//PURP : header for the camera source file. Many functions/variables are present to
//		 handle the movement of the camera and determine when and how to move.

#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
	private:
		double vx, vy,speed, maxSpeed, friction, cameraPause,
			zoomVelocity, zoomSpeed, zoomMaxSpeed, zoomAccuracy, zoomToPoint;
		int width, height, moveToPointX, moveToPointY,
			accuracy, motion;
		int framesWaited;
		Sprite *followedObject;

	public:
		Camera();
		~Camera();
		void handleKeys(double playerSpeed);
		void scrollScreen();
		void updateTimer(Sprite *sprite);
		void newMoveToPoint(Sprite *sprite, double xAdj, double yAdj);
		void newZoom(double newZoom, double speed, double maxSpeed, double accuracy);
		double randomNumber(double Min, double Max);

		//quick inline functions
		double getSpeed() {return speed;}
		double getCameraPause() {return cameraPause;}
		int   getAccuracy() {return accuracy;}
		int   getMotion() {return motion;}
		int   getMoveToPointX() {return moveToPointX;}
		int   getMoveToPointY() {return moveToPointY;}

		void setSpeed(double newSpeed) {speed = newSpeed;}
		void setMaxSpeed(double newMax) {maxSpeed = newMax;}
		void setCameraPause(double newPause) {cameraPause = newPause*FPS;}
		void setAccuracy(int newAccuracy) {accuracy = newAccuracy;}
		void setMotion(int newMotion) {motion = newMotion;}
		void setZoomVelocity(double newVel) {zoomVelocity = newVel;}
		void setFriction (double newFric) {friction = newFric;}
		void setfollowedObject (Sprite* object) {followedObject = object;}
};

#endif