#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "Entity.hpp"
#include "Stage.hpp"

using namespace std;

class GameObject : public Entity {
public:
	GameObject();
	
	float xvel = 0;
	float yvel = 0;

	virtual bool draw(vector<GameObject*>& gameobjs, Stage* stage);

	virtual GameObject* createObject();
};