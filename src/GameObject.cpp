#include <iostream>

#include "GameObject.hpp"

using namespace std;

bool GameObject::draw(vector<GameObject*>& gameobjs) {
	x += xvel;
	y += yvel;

	return false;
}