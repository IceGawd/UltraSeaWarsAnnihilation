#include "HookShot.hpp"

HookShot::HookShot() {

}

HookShot::HookShot(float xtemp, float ytemp, float xv, float yv, int g, DamageInfo di) {
	x = xtemp;
	y = ytemp;
	xvel = xv;
	yvel = yv;
	magnitude = getMagnitude();
	cout << "magnitude: " << magnitude << endl;
	gameobjPlacement = g;
	damageinfo = di;
	textureDraw = false;
	hitbox = Circle(16 * SIZECHANGE, 16 * SIZECHANGE, 16 * SIZECHANGE);
}

bool HookShot::draw(vector<GameObject*>& gameobjs, Stage* stage) {
	GameObject::draw(gameobjs, stage);
	frames++;

	Player* owner = static_cast<Player*>(gameobjs.at(gameobjPlacement));
	Player* toHit = static_cast<Player*>(gameobjs.at(1 - gameobjPlacement));

	playerx = owner->x;
	playery = owner->y;

	int frameLag = ENDLAG + frames - LIFESPAN;
	if (frameLag < 0) {
		xvel *= 0.99;
		yvel *= 0.99;
		yvel += 4;
	}
	else {
		xvel = (owner->x - x) * frameLag / ENDLAG;
		yvel = (owner->y - y) * frameLag / ENDLAG;
	}

	// NO MORE X/Y MOVEMENT
	hitbox.x = hitbox.basex + x;
	hitbox.y = hitbox.basey + y;
	Circle* c = toHit->collides(hitbox);
	if (c != nullptr) {
		if (xvel != 0) {
			damageinfo.setAngle(acos(xvel / abs(xvel)), atan(yvel / xvel), toHit->previous);
		}
		else {			
			if (yvel > 0) {
				damageinfo.setAngle(M_PI / 2, M_PI / 2, toHit->previous);
			}
			else {
				damageinfo.setAngle(M_PI / -2, M_PI / -2, toHit->previous);
			}
		}
		// float magni2 = getMagnitude() / magnitude;
		// cout << xvel << ", " << yvel << endl;
		// cout << "magni2: " << magni2 << endl;
		// damageinfo.knockback *= magni2;
		// damageinfo.modifier *= magni2;
		// damageinfo.damage *= magni2;
		toHit->damage(damageinfo, owner);
		frames = LIFESPAN - ENDLAG;
	}

	return frames > LIFESPAN;
}

void HookShot::customDraw(RenderWindow* window) {
	if (!textureDraw) {
		cout << "Texture loaded!\n";
		setTexture(window->loadTexture("res/gfx/grapple.png"));
		chain = Entity(x, y, window->loadTexture("res/gfx/chain.png"));
		chain.show_width *= SIZECHANGE;
		chain.show_height *= SIZECHANGE;
		fullPicSize();
		show_width *= SIZECHANGE;
		show_height *= SIZECHANGE;
		textureDraw = true;
		setRect();
	}

	particleTrails(playerx, playery, x, y, chain, window);

	// hitbox.draw(window, 255, 255, 0, 100);
}

GameObject* HookShot::createObject() {
	return new HookShot(*this);
}