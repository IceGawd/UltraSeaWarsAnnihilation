#pragma once

#include <iostream>
#include <math.h>

#include "GameObject.hpp"
#include "RenderWindow.hpp"
#include "DamageInfo.hpp"
#include "Player.hpp"

class HitscanRay : public GameObject {
public:
	HitscanRay(float x, float y, float a, int g, DamageInfo di);
	bool draw(vector<GameObject*>& gameobjs);
	int fixIt(int i);
	void customDraw(RenderWindow* window);

	virtual GameObject createObject();

	float originx = 0;
	float originy = 0;

	float destx = 0;
	float desty = 0;

	float angle = 0;

	int frames = 0;
	int gameobjPlacement = 0;
	const int LIFESPAN = 30;
	DamageInfo damageinfo;
};