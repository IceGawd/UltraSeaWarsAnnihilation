#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <list>
#include <math.h>

#include "GameObject.hpp"
#include "Characters.hpp"
#include "Moves.hpp"
#include "Input.hpp"
#include "RenderWindow.hpp"
#include "Circle.hpp"
#include "DamageInfo.hpp"
#include "Stage.hpp"

using namespace std;

class Player : public GameObject {
public:
	Character character;
	list<Move> stale = {};
	Controllers playerNum;
	InputMethod inputs;
	list<Inputs> inputQueue = {{}};
	list<Inputs> rollback = {};
	int walkspeed;
	int dashspeed;
	int jumpspeed;
	int fallspeed;
	int jumpSquat;
	int dashlag;
	int turnAround;
	float shorthop;
	float drag;
	float friction;
	float fastFall;
	float slowDown;
	int maxjumps;
	int landinglag;
	int airDodgeTime;
	int airDodgeLag;
	int airDodgeSpeed;
	float hitstunModifier;

	vector<Circle> baseHitboxes;
	vector<Circle> hitboxes;	
	vector<vector<vector<Circle>>> hitboxChanges = {};
	vector<vector<vector<int>>> imageDimentions;	

	int animationLength[ANIMATIONS];

	int invulnFlash = 0;
	int jumpsUsed = 0;
	int charge = 0;
	int lag = 0;
	Lag type = NONE;
	bool onGround = false;
	bool wasOnGround = false;
	bool facingRight = true;
	bool fastFalling = false;
	int framesNeutral = 0;
	int invulnerability = 0;
	double percent = 0;
	Move inUse;
	int attackNumber = 0;
	int lastAttackNumHitBy = -1;

	Direction previous = NEUTRAL;
	static const int INVULNFLASHTIME = 1;
	static const int CHARGEMAX = 120;
	static constexpr float SQRT2 = (float) sqrt(2);

	Player();
	Player(Character c, Controllers p);
	void spawn();
	void zeroInitializeHitboxes();
	float getAngle(Direction d);
	virtual void applyFrame(vector<GameObject*>& gameobjs, Stage* s, Inputs& input);
	Circle* collides(int x, int y);
	Circle* collides(Circle& c2);
	float angleBetween(int tempx, int tempy);
	float getAngle();
	void damage(DamageInfo& damageinfo, Player* attacker);
	virtual void attack(vector<GameObject*>& gameobjs, bool charge, Direction d);
	virtual GameObject createObject();

	inline bool wasDown(Direction d) {
		return (d == DOWN_LEFT || d == DOWN || d == DOWN_RIGHT);
	}

	inline bool wasUp(Direction d) {
		return (d == UP_LEFT || d == UP || d == UP_RIGHT);
	}

	inline bool wasRight(Direction d) {
		return (d == UP_RIGHT || d == RIGHT || d == DOWN_RIGHT);
	}

	inline bool wasLeft(Direction d) {
		return (d == UP_LEFT || d == LEFT || d == DOWN_LEFT);
	}
};