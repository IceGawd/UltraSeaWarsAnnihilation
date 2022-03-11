#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

class DamageInfo {
public:
	DamageInfo();
	DamageInfo(int d, int k, float m, float i);
	void setAngle(float initialangle, float attackangle, float diangle);
	float angleDifference(float angle1, float angle2);
	int damage;
	int baseknockback;
	int knockback;
	float invulnerability;
	float basemodifier;
	float modifier;
	float angle;
};