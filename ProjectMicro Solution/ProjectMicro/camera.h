//FILE : camera.h
//PROG : Dylan Gallardo
//PURP : header for the camera source file. Many functions/variables are present to
//		 handle the movement of the camera and determine when and how to move.

#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
	private:
		double cameraPause, shakeDelayRange, shakeDelay, zoomToPoint, moveToPointX, moveToPointY, 
			lerpAmt, zoomLerp, xScroll, yScroll, zoomScroll, 
			additionalMotionX, additionalMotionY;
		int motion, framesWaited, movementTimer, shakeTimer;
		Sprite *followedObject;

		void updateZoom();
		void scrollScreen();
		void updateTimer();
		void newMoveToPoint(Sprite *sprite);
		void changeMotion();
		double lerp(double A, double t, double B);
		double randomNumber(double Min, double Max);

	public:
		Camera();
		~Camera();
		void updateCamera();
		void newZoom(double newZoom);

		double getCameraPause() {return cameraPause;}
		int   getMotion() {return motion;}
		int   getMoveToPointX() {return moveToPointX;}
		int   getMoveToPointY() {return moveToPointY;}

		void setCameraPause(double newPause) {cameraPause = newPause*FPS;}
		void setMotion(int newMotion) {motion = newMotion;}
		void setfollowedObject (Sprite* object) {followedObject = object;}
};

#endif