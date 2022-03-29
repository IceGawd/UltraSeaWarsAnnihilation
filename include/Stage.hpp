#pragma once

#include "Circle.hpp"
#include "Entity.hpp"

class Stage : public Entity {
public:
	Circle collide;

	Stage(Circle c, SDL_Texture* t);
	bool collidesWith(Circle& c);
	float angleFrom(int tempx, int tempy, int prevx, int prevy);
};