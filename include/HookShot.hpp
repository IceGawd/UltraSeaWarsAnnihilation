#pragma once

#include <iostream>
#include "GameObject.hpp"
#include "DamageInfo.hpp"
#include "RenderWindow.hpp"
#include "Circle.hpp"
#include "Player.hpp"

class HookShot : public GameObject {
public:
	int gameobjPlacement;
	Circle hitbox;
	SDL_Texture* chain; // MAKE A SHARED POINTER + DESTRUCTOR CALL I THINK IDK
	int frames = 0;
	const int LIFESPAN = 60;

	HookShot();
	HookShot(float xtemp, float ytemp, float xv, float yv, int g);

	bool draw(vector<GameObject*>& gameobjs);
	void customDraw(RenderWindow* window);
	virtual GameObject* createObject();
};