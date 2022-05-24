#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <math.h>
#include <iostream>

#include "Entity.hpp"

using namespace std;

class RenderWindow {
public:
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* sans;
	int x = 0;
	int y = 0;
	float zoom = 1.0;

	RenderWindow(const char* title, int w, int h); 
	SDL_Texture* loadTexture(const char* filePath);
	void cleanUp();
	void clear();
	SDL_Rect getDestRect(Entity* entity, bool stationary = false);
	void render(Entity* entity, bool stationary = false);
	void display();
	void setColor(int r, int g, int b, int a);
	void drawLine(int x1, int y1, int x2, int y2);
	void drawCircle(int x, int y, int radius);
	void drawText(string text, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int x, int y, int w, int h);
};