#include <iostream>
#include <math.h>

#include "Vanshlicer.hpp"

using namespace std;

Vanshlicer::Vanshlicer() {

}

Vanshlicer::Vanshlicer(RenderWindow& window, Controllers p) : Player(VANSHLICER, p) {
	setTexture(window.loadTexture("res/gfx/Vanshlicer.png"));
	width = 255;
	height = 250;
	show_width = 255;
	show_height = 250;
	walkspeed = 3;
	dashspeed = 35;
	dashframes = 11;
	jumpspeed = 100;
	fallspeed = 5;
	jumpSquat = 4;
	shorthop = 0.5f;
	drag = 0.85f;
	friction = 0.9f;
	fastFall = 2.0f;
	slowDown = 0.9f;
	maxjumps = 2;
	turnAround = 8;
	landinglag = 4;
	hitstunModifier = 0.5;

	animationLength[JAB] = 8;
	animationLength[SQUAT] = 4;
	animationLength[TURN] = 4;

	baseHitboxes = {
		Circle(75, 55, 40), 
		Circle(79, 113, 50), 
		Circle(20, 143, 25), 
		Circle(138, 135, 25), 
		Circle(64, 172, 20), 
		Circle(58, 205, 20), 
		Circle(55, 235, 20), 
		Circle(99, 169, 20), 
		Circle(106, 198, 20), 
		Circle(110, 232, 20)
	};

	imageDimentions = {
		{
			{160, 250}, 
			{213, 243}, 
			{254, 160}, 
			{252, 194}, 
			{162, 249}, 
			{206, 241}, 
			{255, 160}, 
			{250, 200}
		}, 
		{
			{206, 250}, 
			{202, 238}, 
			{208, 250}, 
			{202, 250}
		}, 
		{
			{160, 250}, 
			{120, 243}, 
			{160, 250}, 
			{120, 243}
		}
	};

	hitboxes = vector<Circle>(baseHitboxes.size());

	zeroInitializeHitboxes();

	for (int z = 1; z < hitboxChanges[0].size(); z++) {
		double angle = M_PI * z / 4.0;
		int numExtra = 2 - abs((hitboxChanges[0].size() / 2.0) - z);
		cout << "numex: " << numExtra << endl;
		for (int a = 0; a < hitboxChanges[0][z].size(); a++) {
			int centerx = baseHitboxes[a].basex - (imageDimentions[0][0][0] / 2);
			int centery = (height - imageDimentions[0][0][1] / 2) - baseHitboxes[a].basey;
			double radius = sqrt(pow(centerx, 2) + pow(centery, 2));
			double myangle = atan(1.0 * centery / centerx) + angle;
			if (centerx < 0) {
				myangle -= M_PI;
			}
			int newX = radius * cos(myangle);
			int newY = radius * sin(myangle);
			newX += imageDimentions[0][0][0] / 2;
			newY = (height - imageDimentions[0][0][1] / 2) - newY;

			hitboxChanges[0][z][a] = Circle(newX, newY, baseHitboxes[a].radius) - baseHitboxes[a];
			if ((a <= 6 && a >= 6 - numExtra) || (a <= 9 && a >= 9 - numExtra)) {
				int dist = 6 - a;
				if (a > 6) {
					dist += 3;
				}

				hitboxChanges[0][z][a].attack = true;
//				cout << "damage: " << (4 + numExtra * 3) / (dist + 1) << endl;
				hitboxChanges[0][z][a].onHit = DamageInfo((4 + numExtra * 3) / (dist + 1), 80, 0.5, 8 - z);
				hitboxChanges[0][z][a].onHit.angle = M_PI / 2;
//				cout << "angle: " << hitboxChanges[0][z][a].onHit.angle << endl;
			}
//			cout << "angle2: " << hitboxChanges[0][z][a].onHit.angle << endl;
		}
	}
}

void Vanshlicer::startForwardCharge(vector<GameObject*>& gameobjs, Direction d) {

}

void Vanshlicer::startBackCharge(vector<GameObject*>& gameobjs, Direction d) {

}

void Vanshlicer::startUpCharge(vector<GameObject*>& gameobjs, Direction d) {

}

void Vanshlicer::releaseForwardCharge(vector<GameObject*>& gameobjs, Direction d) {

}

void Vanshlicer::releaseBackCharge(vector<GameObject*>& gameobjs, Direction d) {

}

void Vanshlicer::releaseUpCharge(vector<GameObject*>& gameobjs, Direction d) {

}

void Vanshlicer::forwardQuick(vector<GameObject*>& gameobjs, Direction d) {

}

void Vanshlicer::downQuick(vector<GameObject*>& gameobjs, Direction d) {
	lag = 8;
	type = ATTACK;
	inUse = DownQuick;
}

void Vanshlicer::forwardAerial(vector<GameObject*>& gameobjs, Direction d) {
	
}

void Vanshlicer::secondStick(vector<GameObject*>& gameobjs, Direction d) {
	
}

GameObject* Vanshlicer::createObject() {
	return new Vanshlicer(*this);
}