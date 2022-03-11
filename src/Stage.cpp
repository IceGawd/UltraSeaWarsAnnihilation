#include <iostream>

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

float Stage::angleFrom(int tempx, int tempy) {
//	return M_PI / 2;

	if (tempy < collide.y) {
		return M_PI / 2.0;
	}
	else {
//		cout << "tempy: " << tempy << endl;
//		cout << "collide.y: " << collide.y << endl;
		if (collide.x == tempx) {
			return M_PI / -2.0;
		}
		else {
			if (collide.x - tempx < 0) {
				return atan((collide.y - tempy) / (collide.x - tempx));				
			}
			else {
				return M_PI + atan((collide.y - tempy) / (collide.x - tempx));
			}
		}
	}

}