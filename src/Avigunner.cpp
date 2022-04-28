#include "Avigunner.hpp"

using namespace std;

Avigunner::Avigunner() {
	
}

Avigunner::Avigunner(RenderWindow& window, Controllers p) : Player(AVIGUNNER, p) {
	texture = window.loadTexture("res/gfx/Avigunner.png");
	width = 224;
	height = 280;
	show_width = 224;
	show_height = 280;
	walkspeed = 4;
	jumpspeed = 120;
	fallspeed = 6;
	jumpSquat = 4;
	shorthop = 0.4f;
	drag = 0.8f;
	friction = 0.85f;
	fastFall = 3.0f;
	slowDown = 0.95f;
	maxjumps = 2;
	turnAround = 4;
	landinglag = 4;
	hitstunModifier = 0.6;

	animationLength[JAB] = 8;
	animationLength[SQUAT] = 8;
	animationLength[TURN] = 4;

	baseHitboxes = {
		Circle(63, 73, 40), 
		Circle(60, 148, 45), 
		Circle(14, 180, 15), 
		Circle(116, 179, 25), 
		Circle(45, 207, 20), 
		Circle(36, 239, 20), 
		Circle(22, 269, 20), 
		Circle(76, 206, 20), 
		Circle(78, 239, 20), 
		Circle(86, 269, 20)
	};

	imageDimentions = {
		{
			{136, 274}, 
			{152, 274}, 
			{172, 274}, 
			{224, 274}, 
			{205, 279}, 
			{178, 274}, 
			{176, 274}, 
			{130, 274}
		}, 
		{
			{136, 244}, 
			{142, 240}, 
			{136, 231}, 
			{148, 230}, 
			{136, 226}, 
			{146, 244}, 
			{144, 246}, 
			{136, 244}
		}, 
		{
			{136, 274}, 
			{96, 274}, 
			{136, 274}, 
			{96, 274}
		}
	};

	hitboxes = vector<Circle>(baseHitboxes.size());

	zeroInitializeHitboxes();
	
	
	for (int z = 1; z < hitboxChanges[0].size(); z++) {
		hitboxChanges[0][z][3] = Circle(10, -40, 3);
	}
	for (int z = 3; z < 4; z++) {
		hitboxChanges[0][z][5] = Circle(5, -5, 0);
		hitboxChanges[0][z][8] = Circle(0, -10, 0);
		hitboxChanges[0][z][6] = Circle(10, -10, 0);
		hitboxChanges[0][z][9] = Circle(-15, -10, 0);
	}
}

void Avigunner::startForwardCharge(vector<GameObject*>& gameobjs, Direction d) {
	lag = 3;
	type = ATTACK;
}

void Avigunner::startBackCharge(vector<GameObject*>& gameobjs, Direction d) {
	lag = 3;
	type = ATTACK;
}

void Avigunner::releaseForwardCharge(vector<GameObject*>& gameobjs, Direction d) {
	lag = 5;
	type = ATTACK;

	float angle = d.angle;

	if (facingRight) { 
		if (!wasRight(d)) {
			angle = 0;
		}
	}
	else {
		if (!wasLeft(d)) {
			angle = M_PI;
		}
	}
	float maxanglemod = (M_PI * this->charge) / (2.0 * CHARGEMAX);

	float xtemp = x;
	if (facingRight) {xtemp += 170;} else {}
	float shotsFired = this->charge / 5;

	for (int z = 0; z < shotsFired; z++) {
		// cout << "arg: " << (angle + maxanglemod * (0.5 - (z / shotsFired))) << endl;
		gameobjs.push_back(new HitscanRay(xtemp, y + 150, (angle + maxanglemod * (0.5 - (z / shotsFired))), playerNum, DamageInfo(1, 30, 0.6, 0)));
	}
}

void Avigunner::releaseBackCharge(vector<GameObject*>& gameobjs, Direction d) {
	float angle = d.angle;

	if (facingRight) { 
		if (!wasLeft(d)) {
			angle = M_PI;
		}
	}
	else {
		if (!wasRight(d)) {
			angle = 0;
		}
	}
	float maxanglemod = (M_PI * this->charge) / (1.5 * CHARGEMAX);

	float xtemp = x;
	if (facingRight) {} else {xtemp += 170;}
	const float spores = this->charge / 10;
	float velocity = (CHARGEMAX + this->charge) / 6;

	for (int z = 14; z > 14 - spores; z--) {
		DamageInfo di = DamageInfo(5, 120, 0.5, 3);
//					cout << "mod: " << this->charge % z << endl;
//					cout << "div: " << z / 2.0 << endl;
//					cout << "tot: " << ((z / 2.0 - (this->charge % z)) / z) << endl;
		float mod = ((z / 2.0 - (this->charge % z)) / z);
		di.angle = angle + maxanglemod * mod;
		velocity *= 1.2 + 0.1 * (this->charge / CHARGEMAX);
//					cout << "arg: " << di.angle << endl;
		gameobjs.push_back(new Spore(xtemp, y + 150, velocity * cos(di.angle), -velocity * sin(di.angle), 300, 10, playerNum, di, "Spore"));
	}
}

void Avigunner::forwardQuick(vector<GameObject*>& gameobjs, Direction d) {
	lag = 8;
	type = ATTACK;
}

void Avigunner::downQuick(vector<GameObject*>& gameobjs, Direction d) {
	lag = 8;
	type = ATTACK;
}

void Avigunner::applyFrame(vector<GameObject*>& gameobjs, Stage* s, Inputs& input) {
	if (type == ATTACK) {
		if (inUse == ForwardQuick && lag == 5) {
			float angle = input.direction.angle;
			float xtemp = x;
			if (facingRight) {xtemp += 170;} else {}
			gameobjs.push_back(new HitscanRay(xtemp, y + 150, angle, playerNum, DamageInfo(1, 0, 0, 0)));
		}
		if (inUse == DownQuick && lag == 5) {
			DamageInfo di = DamageInfo(3, 60, 0.4, 3);
			di.angle = M_PI / 2;
			gameobjs.push_back(new Spore(x, y + imageDimentions[animationtype][animationFrame][1], 0, 0, 100, 10, playerNum, di, "Mine"));			
		}
	}
	
	Player::applyFrame(gameobjs, s, input);
}

GameObject* Avigunner::createObject() {
	return new Avigunner();
}