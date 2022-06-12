#include "Spore.hpp"
#include "Player.hpp"

#include <math.h>
#include <iostream>

using namespace std;

Spore::Spore() {

}

Spore::Spore(float xtemp, float ytemp, float xv, float yv, int s, int e, int g, DamageInfo di, const char* t) {
	x = xtemp;
	y = ytemp;
	xvel = xv;
	yvel = yv;
	survivalFrames = s;
	explosionLength = e;
	damageinfo = di;
	gameobjPlacement = g;
	width = 193;
	show_width = 193;

	if (t == "Spore") {
		height = 151;
		show_height = 151;
		hitbox = Circle(97, 76, 14);
	}
	else {
		height = 126;
		show_height = 126;
		y -= 126;
		hitbox = Circle(97, 126, 14);
	}
	textureDraw = false;
	type = t;
}

bool Spore::draw(vector<GameObject*>& gameobjs, Stage* stage) {
	xvel *= friction;
	yvel *= friction;

	Player* owner = static_cast<Player*>(gameobjs.at(gameobjPlacement));
	Player* toHit = static_cast<Player*>(gameobjs.at(1 - gameobjPlacement));

	GameObject::draw(gameobjs, stage);
	survivalFrames--;
	hitbox.x = hitbox.basex + x;
	hitbox.y = hitbox.basey + y;

	if (survivalFrames > 0) {
		animationFrame = ((int) sqrt(survivalFrames)) % 2;
		if (toHit->collides(hitbox)) {
			survivalFrames = 1;
		}
	}
	else if (survivalFrames == 0) {
		animationFrame = 0;
		animationtype = SQUAT;
		hitbox.radius = 75;
	}
	else {
		animationFrame = -survivalFrames / explosionLength;
		if (survivalFrames + explosionLength > 0) {
			Circle* c = toHit->collides(hitbox);
			if (c != nullptr) {
				damageinfo.setAngle(damageinfo.angle, atan((y - c->y) / (x - c->x)), toHit->previous);
				toHit->damage(damageinfo, owner);
				survivalFrames = -explosionLength;
			}
			/*
			c = owner->collides(hitbox);
			if (c != nullptr) {
				damageinfo.setAngle(damageinfo.angle, atan((y - c->y) / (x - c->x)), owner->previous);
				owner->xvel += damageinfo.baseknockback * cos(damageinfo.angle);
				owner->yvel += damageinfo.baseknockback * sin(damageinfo.angle);
				survivalFrames = -explosionLength;
			}
			*/
		}
	}

//	cout << survivalFrames << endl;

	return ((survivalFrames + explosionLength * 2) < 0);
}

void Spore::customDraw(RenderWindow* window) {
	if (!textureDraw) {
//		cout << "Texture loaded!\n";
		string give = "res/gfx/";
		give += type;
		give += ".png";
		setTexture(window->loadTexture(give.c_str()));
		textureDraw = true;
	}
	setRect();
//	hitbox.draw(window, 255, 255, 0, 100);
}

GameObject* Spore::createObject() {
	return new Spore(*this);
}