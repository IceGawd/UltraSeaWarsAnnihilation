#pragma once

class Entity;
#include "RenderWindow.hpp"
#include "Input.hpp"

#include <math.h>

void particleTrails(int startx, int starty, int endx, int endy, Entity& particle, RenderWindow* window);
Direction directionFromCoords(float x, float y); 