#include <iostream>

#include "Entity.hpp"

using namespace std;

/*
Entity::~Entity() {
	// cout << "lmao gone" << endl;
	SDL_DestroyTexture(texture.get());
}
*/

Entity::Entity() : x(0), y(0) {

}

Entity::Entity(float px, float py, SDL_Texture* ptexture, int pw, int ph) : x(px), y(py), width(pw), height(ph), show_width(pw), show_height(ph) {
	setTexture(ptexture);
	setRect();
}

Entity::Entity(float px, float py, SDL_Texture* ptexture) : x(px), y(py) {
	setTexture(ptexture);
	fullPicSize();
	setRect();
}

void Entity::setTexture(SDL_Texture* ptexture) {
	texture = shared_ptr<SDL_Texture>(ptexture, sdl_deleter());
}

// COPY CONSTRUCTOR
/*
Entity::Entity(const Entity& e) {
	cout << "COPY" << endl;

	texture = new SDL_Texture();
	*texture = *(e.texture);
}
*/

void Entity::fullPicSize() {
	SDL_Point p = getsize();
	width = p.x;
	show_width = p.x;
	height = p.y;
	show_height = p.y;
}

void Entity::setRect() {
	currentFrame.x = animationFrame * width;
	currentFrame.y = (ANIMATIONS * modifier + animationtype) * height;
	currentFrame.w = width;
	currentFrame.h = height;
}

SDL_Point Entity::getsize() {
    SDL_Point size;
    SDL_QueryTexture(texture.get(), NULL, NULL, &size.x, &size.y);
    return size;
}

void Entity::customDraw(RenderWindow* window) {
	
}
