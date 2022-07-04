#include "Input.hpp"
#include "utils.hpp"

Direction Direction::operator-(const Direction& d) {
	int x = this->magnitude * cos(this->angle) - d.magnitude * cos(d.angle);
	int y = this->magnitude * sin(this->angle) - d.magnitude * sin(d.angle);
	
	return directionFromCoords(x, y);
}