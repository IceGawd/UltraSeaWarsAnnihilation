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
	int dashframes;
	int jumpspeed;
	int fallspeed;
	int jumpSquat;
	int turnAround;
	float shorthop;
	float drag;
	float friction;
	float fastFall;
	float slowDown;
	int maxjumps;
	int landinglag;
	float hitstunModifier;

	vector<Circle> baseHitboxes; // HAS TO GO FROM BOTTOM TO TOP
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
	int invulnerability = 0;
	double percent = 0;
	Move inUse;
	int attackNumber = 0;
	int lastAttackNumHitBy = -1;

	Direction previous;
	bool previousJump;
	bool previousQuick;
	static constexpr float minMagnitude = 0.1f;
	static const int INVULNFLASHTIME = 1;
	static const int CHARGEMAX = 120;
	static constexpr float SQRT2 = (float) sqrt(2);

	virtual void startForwardCharge(vector<GameObject*>& gameobjs, Direction d);
	virtual void startBackCharge(vector<GameObject*>& gameobjs, Direction d);
	virtual void releaseForwardCharge(vector<GameObject*>& gameobjs, Direction d);
	virtual void releaseBackCharge(vector<GameObject*>& gameobjs, Direction d);
	virtual void forwardQuick(vector<GameObject*>& gameobjs, Direction d);
	virtual void downQuick(vector<GameObject*>& gameobjs, Direction d);

	Player();
	Player(Character c, Controllers p);
	void spawn();
	void zeroInitializeHitboxes();
	virtual void applyFrame(vector<GameObject*>& gameobjs, Stage* s, Inputs& input);
	Circle* collides(int x, int y);
	Circle* collides(Circle& c2);
	float angleBetween(int tempx, int tempy);
	void damage(DamageInfo& damageinfo, Player* attacker);
	virtual void attack(vector<GameObject*>& gameobjs, bool charge, Direction d);
	virtual GameObject* createObject();

	inline bool wasDown(Direction& d) {
		return d.magnitude > minMagnitude && ((d.angle > 11 * M_PI / 8) || (d.angle < -3 * M_PI / 8));
	}

	inline bool wasUp(Direction& d) {
		return d.magnitude > minMagnitude && ((5 * M_PI / 8 > d.angle) && (d.angle > 3 * M_PI / 8));
	}

	inline bool wasRight(Direction& d) {
		return d.magnitude > minMagnitude && ((3 * M_PI / 8 >= d.angle) && (d.angle >= -3 * M_PI / 8));
	}

	inline bool wasLeft(Direction& d) {
		return d.magnitude > minMagnitude && ((11 * M_PI / 8 >= d.angle) && (d.angle >= 5 * M_PI / 8));
	}
};