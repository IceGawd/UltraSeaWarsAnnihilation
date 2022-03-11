#include <iostream>

#include "RenderWindow.hpp"
#include "Entity.hpp"

using namespace std;

RenderWindow::RenderWindow(const char* title, int w, int h) : window(NULL), renderer(NULL) {
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);

	if (window == NULL) {
		cout << "Window failed to Init: " << SDL_GetError() << "\n"; 
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//	cout << "sans\n";
	sans = TTF_OpenFont("res/gfx/zephyrea.ttf", 24);
};

void RenderWindow::cleanUp() {
	SDL_DestroyWindow(window);
}

SDL_Texture* RenderWindow::loadTexture(const char* filePath) {
	SDL_Texture* texture = IMG_LoadTexture(renderer, filePath);

	if (texture == NULL) {
		cout << "Failed to load a texture " << SDL_GetError() << "\n";
	}

	return texture;
}

void RenderWindow::clear() {
	SDL_RenderClear(renderer);
}

void RenderWindow::setColor(int r, int g, int b, int a) {
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void RenderWindow::drawLine(int x1, int y1, int x2, int y2) {
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

SDL_Rect RenderWindow::getDestRect(Entity* entity, bool stationary) {
	SDL_Rect dest;
	if (stationary) {
		dest.x = (int) entity->x;
		dest.y = (int) entity->y;
		dest.w = entity->show_width;
		dest.h = entity->show_height;
	}
	else {
		dest.x = (int) ((entity->x - x) * zoom);
		dest.y = (int) ((entity->y - y) * zoom);
		dest.w = (int) (entity->show_width * zoom);
		dest.h = (int) (entity->show_height * zoom);
	}
	return dest;
}

void RenderWindow::render(Entity* entity, bool stationary) {
	if (entity->textureDraw) {
//		cout << "X: " << entity->x << "Y: " << entity->y << endl;

		SDL_Rect dest = getDestRect(entity, stationary);

	//	cout << "x2: " << entity->getsize().x << endl;
	//	cout << "y2: " << entity->getsize().y << endl;

		SDL_RenderCopy(renderer, entity->texture, &(entity->currentFrame), &dest);
	}

	entity->customDraw(this);
}

void RenderWindow::display() {
	SDL_RenderPresent(renderer);
}

void RenderWindow::drawCircle(int x, int y, int radius) {
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	for (int w = 0; w < radius * 2; w++) {
		for (int h = 0; h < radius * 2; h++) {
			int dx = radius - w; // horizontal offset
			int dy = radius - h; // vertical offset
			if ((dx * dx + dy * dy) <= (radius * radius)) {
				SDL_RenderDrawPoint(renderer, x + dx, y + dy);
			}
		}
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void RenderWindow::drawText(string text, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int x, int y, int w, int h) {

	// as TTF_RenderText_Solid could only be used on
	// SDL_Surface then you have to create the surface first
//	cout << 1 << endl;
	SDL_Color color = {r, g, b, a};
//	cout << 1.5 << endl;
//	cout << SDL_GetError() << endl;
//	cout << TTF_GetError() << endl;
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(sans, text.c_str(), color); 
//	cout << 2 << endl;

	// now you can convert it into a texture
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
//	cout << 3 << endl;

	SDL_Rect Message_rect; //create a rect
	Message_rect.x = x;  //controls the rect's x coordinate 
	Message_rect.y = y; // controls the rect's y coordinte
	Message_rect.w = w; // controls the width of the rect
	Message_rect.h = h; // controls the height of the rect

	// (0,0) is on the top left of the window/screen,
	// think a rect as the text's box,
	// that way it would be very simple to understand

	// Now since it's a texture, you have to put RenderCopy
	// in your game loop area, the area where the whole code executes

	// you put the renderer's name first, the Message,
	// the crop size (you can ignore this if you don't want
	// to dabble with cropping), and the rect which is the size
	// and coordinate of your texture
	SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

	// Don't forget to free your surface and texture
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
}