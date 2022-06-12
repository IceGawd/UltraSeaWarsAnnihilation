#include "utils.hpp"

void particleTrails(int startx, int starty, int endx, int endy, Entity& particle, RenderWindow* window) {
	float xdiff = startx - endx;
	float ydiff = starty - endy;

	// cout << "xdiff: " << xdiff << endl;
	// cout << "ydiff: " << ydiff << endl;

	if (xdiff != 0) {
		int times = sqrt(pow(xdiff, 2) + pow(ydiff, 2)) / particle.width;
		particle.angle = atan(ydiff / xdiff);
		// cout << "particle angle: " << particle.angle << endl;
		for (int z = 0; z < times; z++) {
			particle.x = (startx * (times - z) + z * endx) / times;
			particle.y = (starty * (times - z) + z * endy) / times;
			window->render(&particle);
		}
	}
}