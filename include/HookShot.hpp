#pragma once

#include <iostream>
#include "GameObject.hpp"
#include "DamageInfo.hpp"
#include "RenderWindow.hpp"
#include "Circle.hpp"
#include "Player.hpp"
#include "utils.hpp"

class HookShot : public GameObject {
public:
	int playerx;
	int playery;

	int gameobjPlacement;
	Circle hitbox;
	DamageInfo damageinfo;
	Entity chain;
	int frames = 0;
	float magnitude;
	const int LIFESPAN = 30;
	const int ENDLAG = 10;
	const float SIZECHANGE = 2;

	HookShot();
	HookShot(float xtemp, float ytemp, float xv, float yv, int g, DamageInfo di);

	bool draw(vector<GameObject*>& gameobjs, Stage* stage);
	void customDraw(RenderWindow* window);
	inline float getMagnitude() {return xvel * xvel + yvel * yvel;} 
	virtual GameObject* createObject();
};