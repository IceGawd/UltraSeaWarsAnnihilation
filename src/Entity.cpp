#include <iostream>

#include "Entity.hpp"

using namespace std;

Entity::~Entity() {
	// cout << "lmao gone" << endl;
	SDL_DestroyTexture(texture);
}


Entity::Entity() : x(0), y(0) {

}

Entity::Entity(float px, float py, SDL_Texture* ptexture, int pw, int ph) : x(px), y(py), texture(ptexture), width(pw), height(ph), show_width(pw), show_height(ph) {
	setRect();
}

Entity::Entity(float px, float py, SDL_Texture* ptexture) : x(px), y(py), texture(ptexture) {
	fullPicSize();
	setRect();
}

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
    SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
    return size;
}

void Entity::customDraw(RenderWindow* window) {
	
}
