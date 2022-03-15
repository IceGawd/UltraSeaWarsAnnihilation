#include "HitscanRay.hpp"

using namespace std;

HitscanRay::HitscanRay(float x, float y, float a, int g, DamageInfo di) : originx(x), destx(x), originy(y), desty(y), angle(a), gameobjPlacement(g), damageinfo(di) {}

int HitscanRay::fixIt(int i) {
	return (int) (510 * ((atan((i / 30.0) - 4) / M_PI) + 0.5));
}

bool HitscanRay::draw(vector<GameObject*>& gameobjs) {
	if (frames == 0) {
		textureDraw = false;
//		cout << "goteem\n";

		float xchange = 20 * cos(angle);
		float ychange = 20 * sin(angle);

//		cout << xchange << endl;
//		cout << ychange << endl;

		Player* owner = static_cast<Player*>(gameobjs.at(gameobjPlacement));
		Player* toHit = static_cast<Player*>(gameobjs.at(1 - gameobjPlacement));

		while (destx > -3000 && destx < 3000 && desty > -3000 && desty < 3000) {
//			cout << "goteem2\n";
			destx += xchange;
			desty += ychange;
			Circle* c = toHit->collides(destx, desty);
			if (c != nullptr) {
				damageinfo.setAngle(angle, atan((originy - c->y) / (originx - c->x)), toHit->getAngle());
				toHit->damage(damageinfo, owner);
				break;
			}
		}
//		cout << "histcent\n";
	}

	frames++;
	return frames > LIFESPAN;
}

void HitscanRay::customDraw(RenderWindow* window) {
//	cout << "StartCustomDraw\n";
	float x = 1.0 * frames / LIFESPAN;
	float x2 = x * x;
	float x3 = x2 * x;

	int r = fixIt(255 - 255 * x + 1147.5 * x2 - 1147.5 * x3);
	int g = fixIt(255 - 2550 * x + 6885 * x2 - 4590 * x3);
	int b = fixIt(255 - 2295 * x + 1402.5 * x2 - 1147.5 * x3);
	int a = fixIt(255 - 255 * x + 1147.5 * x2 - 1147.5 * x3);

	window->setColor(r, g, b, a);
	window->drawLine((int) ((originx - window->x) * window->zoom), (int) ((originy - window->y) * window->zoom), (int) ((destx - window->x) * window->zoom), (int) ((desty - window->y) * window->zoom));
//	cout << "EndCustomDraw\n";
}

virtual GameObject HitscanRay::createObject() {
	return new HitscanRay();
}