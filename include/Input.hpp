#pragma once

enum Controllers {
	PLAYER1, 
	PLAYER2, 
	PLAYER3, 
	PLAYER4
};

enum Direction {
	NEUTRAL, 
	UP,
	UP_RIGHT,
	RIGHT,
	DOWN_RIGHT,
	DOWN,
	DOWN_LEFT,
	LEFT, 
	UP_LEFT
};

struct Inputs {
	bool moveright = false;
	bool moveleft = false;
	Direction direction = NEUTRAL;
	bool quick = false;
	bool charge = false;
	bool jump = false;

	bool jumpPressed = false;
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
	DASH,
	LAND,
	AIRDODGE, 
	SPOTDODGE, 
	CHARGE, 
};

enum Animation {
	JAB,
	SQUAT,
	TURN, 
	ANIMATIONS
};