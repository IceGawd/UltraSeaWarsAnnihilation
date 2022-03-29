#include <iostream>
#include <math.h>

#include "Stage.hpp"

using namespace std;

Stage::Stage(Circle c, SDL_Texture* t) : Entity(c.basex - c.radius, c.basey, t, c.radius * 2, c.radius), collide(c) {
	width = 720;
	height = 360;
	setRect();

//	cout << collide.basex << endl;
//	cout << collide.basey << endl;
//	cout << collide.radius << endl;
}

bool Stage::collidesWith(Circle& c) {
//	cout << "c.x: " << c.x << " c.y: " << c.y << endl; 
	return (c.y > collide.y) && (collide.collides(c));
}

float Stage::angleFrom(int tempx, int tempy, int prevx, int prevy) {
	// cout << "tempx: " << tempx << " tempy: " << tempy << " prevx: " << prevx << " prevy: " << prevy << endl;
	// cout << "x: " << x << " y: " << y << " collide.x: " << collide.x << " collide.y: " << collide.y << endl;
	int a = tempx - collide.x;
	int b = tempy - collide.y;
	if (b < collide.radius - sqrt(a*a + b*b)) {
		return M_PI / 2.0;
	}
	else {
		// cout << "a: " << a << endl;
		// cout << "b: " << b << endl;
		// cout << "collide.radius - sqrt(a*a + b*b): " << collide.radius - sqrt(a*a + b*b) << endl;
		if (a == 0) {
			if (b >= 0) {
				// cout << "up" << endl;
				return M_PI / 2.0;
			}
			else {
				// cout << "down" << endl;
				return M_PI / -2.0;			
			}
		}
		else if (a > 0) {
			return atan(b / a);
		}
		else {
			return M_PI + atan(b / a);
		}
	}

	// ATTEMPT 1
	/*
	// return M_PI / 2;
	//cout << "Collide y: " << collide.y << endl;
	if (prevy < collide.y) {
		return M_PI / 2.0;
	}
	else {
		// cout << "tempy: " << tempy << endl;
		// cout << "collide.y: " << collide.y << endl;
		if (collide.x == tempx) {
			if (collide.y - tempy <= 0) {
				cout << "up" << endl;
				return M_PI / 2.0;
			}
			else {
				cout << "down" << endl;
				return M_PI / -2.0;			
			}
		}
		else if (collide.x - tempx < 0) {
			return atan((collide.y - tempy) / (collide.x - tempx));
		}
		else {
			return M_PI + atan((collide.y - tempy) / (collide.x - tempx));
		}
	}
	*/

	// ATTEMPT 2
	/*
	if (prevx == tempx) {
		if (prevy - tempy <= 0) {
			// cout << "up" << endl;
			return M_PI / 2.0;
		}
		else {
			// cout << "down" << endl;
			return M_PI / -2.0;			
		}
	}
	else if (prevx - tempx < 0) {
		// cout << "right" << endl;
		return atan((prevy - tempy) / (prevx - tempx));
	}
	else {
		// cout << "left" << endl;
		return M_PI + atan((prevy - tempy) / (prevx - tempx));
	}
	*/
}