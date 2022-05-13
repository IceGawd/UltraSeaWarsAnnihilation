#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
class RenderWindow;

#include "Input.hpp"

class Entity {
public:
	Entity();
	~Entity();
	Entity(float px, float py, SDL_Texture* ptexture);
	Entity(float px, float py, SDL_Texture* ptexture, int pw, int ph);
	// Entity(const Entity& e);
	void setRect();
	void fullPicSize();
	SDL_Point getsize();
	virtual void customDraw(RenderWindow* window);

	float x;
	float y;
	int width;
	int height;
	int show_width;
	int show_height;
	bool textureDraw = true;
	
	int animationFrame = 0;
	Animation animationtype = JAB;
	int modifier = 0;
	SDL_Rect currentFrame;
	SDL_Texture* texture = nullptr;
};