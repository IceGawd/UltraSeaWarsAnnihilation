
#include "GameObject.hpp"
#include "DamageInfo.hpp"
#include "RenderWindow.hpp"
#include "Circle.hpp"
#include "utils.hpp"

class GrapplingHook : public GameObject {
public:
	int gameobjPlacement;
	Circle hitbox;
	Entity chain;
	const int LIFESPAN = 30;
	const float SIZECHANGE = 3;

	GrapplingHook();
	GrapplingHook(float xtemp, float ytemp, float xv, float yv, int g);
	
	bool draw(vector<GameObject*>& gameobjs, Stage* stage);
	void customDraw(RenderWindow* window);
};