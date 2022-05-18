#include <iostream>

#include "GameObject.hpp"

using namespace std;

GameObject::GameObject() {
	
}

bool GameObject::draw(vector<GameObject*>& gameobjs) {
	x += xvel;
	y += yvel;

	return false;
}

GameObject* GameObject::createObject() {
	return new GameObject(*this);
}