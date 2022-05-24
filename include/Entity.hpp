#pragma once

#include <memory>
class RenderWindow;

#include "Input.hpp"

using namespace std;

class Entity {
public:
	Entity();
	// ~Entity();
	Entity(float px, float py, SDL_Texture* ptexture);
	Entity(float px, float py, SDL_Texture* ptexture, int pw, int ph);
	// Entity(const Entity& e);
	void setRect();
	void fullPicSize();
	SDL_Point getsize();
	void setTexture(SDL_Texture* ptexture);
	virtual void customDraw(RenderWindow* window);

	float x;
	float y;
	int width;
	int height;
	int show_width;
	int show_height;
	bool textureDraw = true;
	float angle = 0;
	
	int animationFrame = 0;
	Animation animationtype = JAB;
	int modifier = 0;
	SDL_Rect currentFrame;
	shared_ptr<SDL_Texture> texture;
};