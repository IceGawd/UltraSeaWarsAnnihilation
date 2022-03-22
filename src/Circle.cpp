#include <iostream>

#include "Circle.hpp"

using namespace std;

Circle::Circle() {}
Circle::Circle(int xtemp, int ytemp, int r) : x(xtemp), basex(xtemp), y(ytemp), basey(ytemp), radius(r) {}
Circle::Circle(int xtemp, int ytemp, int r, bool a) : x(xtemp), basex(xtemp), y(ytemp), basey(ytemp), radius(r), attack(a) {}

bool Circle::collides(int pointx, int pointy) {
	return sqrt(pow(pointx - x, 2) + pow(pointy - y, 2)) < radius;
}

bool Circle::collides(Circle& c) {
	return sqrt(pow(c.x - x, 2) + pow(c.y - y, 2)) < (radius + c.radius);
}

// The arbitrary bullshit that + and - are might lead to future headaches.
Circle Circle::operator+(const Circle& c) {
	return Circle(basex + c.basex, basey + c.basey, radius + c.radius, c.attack);
}

Circle Circle::operator-(const Circle& c) {
	return Circle(basex - c.basex, basey - c.basey, radius - c.radius, attack);
}

void Circle::draw(RenderWindow* window, int r, int g, int b, int a) {
//	cout << "CIRLCE X: " << x << " Y: " << y << endl;
//	cout << "BASEX: " << basex << " BASEY: " << basey << endl;
	window->setColor(r, g, b, a);
	window->drawCircle((x - window->x) * window->zoom, (y - window->y) * window->zoom, radius * window->zoom);
}