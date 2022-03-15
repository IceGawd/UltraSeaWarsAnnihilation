#pragma once

#include "GameObject.hpp"
#include "DamageInfo.hpp"
#include "RenderWindow.hpp"
#include "Circle.hpp"

class Spore : public GameObject {
public:
	Spore();
	Spore(float xtemp, float ytemp, float xv, float yv, int s, int e, int g, DamageInfo di, const char* t);

	int survivalFrames = 0;
	int explosionLength = 0;
	int gameobjPlacement = 0;
	DamageInfo damageinfo;
	const char* type;
	Circle hitbox;

	const float friction = 0.7;

	bool draw(vector<GameObject*>& gameobjs);
	void customDraw(RenderWindow* window);
	virtual GameObject* createObject();
};