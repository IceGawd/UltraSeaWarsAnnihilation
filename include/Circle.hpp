#pragma once

#include "RenderWindow.hpp"
#include "DamageInfo.hpp"

class Circle {
public:
	Circle();
	Circle(int xtemp, int ytemp, int r);
	Circle(int xtemp, int ytemp, int r, bool a);

	int basex = 0;
	int basey = 0;
	int x = 0;
	int y = 0;
	int radius = 0;
	bool attack = false;
	DamageInfo onHit;
	
	bool collides(int x, int y);
	bool collides(Circle& c);
	Circle operator+(const Circle& c);
	Circle operator-(const Circle& c);

	void draw(RenderWindow* window, int r, int g, int b, int a);
};

