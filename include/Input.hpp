#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

enum Controllers {
	PLAYER1, 
	PLAYER2, 
	PLAYER3, 
	PLAYER4
};

struct Direction {
	float angle = 0;
	float magnitude = 0;

	Direction operator-(const Direction& d);
};

struct Inputs {
	Direction direction;
	Direction secondStick;
	bool quick = false;
	bool charge = false;
	bool jump = false;
	bool shield = false;
	bool tech = false;
	// int frame; // For testing later
};

enum Method {
	ONLINE, 
	KEYBOARD,
	CONTROLLER
};

struct InputMethod {
	Method method = KEYBOARD;
	int controllerNum = 0;
};

enum Lag {
	NONE, 
	HIT, 
	JUMP,
	TURNAROUND,
	ATTACK,
	LAND,
	CHARGE, 
	DASH, 
	FREEFALL
};

enum Animation {
	JAB,
	SQUAT,
	TURN, 
	ANIMATIONS
};

struct sdl_deleter
{
	void operator()(SDL_Texture *p) const {
		SDL_DestroyTexture(p);
	}
};

#endif