#include "HookShot.hpp"

HookShot::HookShot() {

}

HookShot::HookShot(float xtemp, float ytemp, float xv, float yv, int g) {
	x = xtemp;
	y = ytemp;
	xvel = xv;
	yvel = yv;
	gameobjPlacement = g;
	textureDraw = false;
}

bool HookShot::draw(vector<GameObject*>& gameobjs) {
	GameObject::draw(gameobjs);
	xvel *= 0.9;
	yvel *= 0.9;
	yvel += 5;

	frames++;

	Player* owner = static_cast<Player*>(gameobjs.at(gameobjPlacement));
	Player* toHit = static_cast<Player*>(gameobjs.at(1 - gameobjPlacement));

	return frames > LIFESPAN;
}

void HookShot::customDraw(RenderWindow* window) {
	if (!textureDraw) {
		cout << "Texture loaded!\n";
		texture = window->loadTexture("res/gfx/grapple.png");
		chain = window->loadTexture("res/gfx/chain.png");
		fullPicSize();
		show_width *= 2;
		show_height *= 2;
		textureDraw = true;
		setRect();
	}
//	hitbox.draw(window, 255, 255, 0, 100);
}

GameObject* HookShot::createObject() {
	return new HookShot();
}