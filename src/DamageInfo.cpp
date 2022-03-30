#include "DamageInfo.hpp"

#include <iostream>

using namespace std;

DamageInfo::DamageInfo() {}
DamageInfo::DamageInfo(int d, int k, float m, float i) : damage(d), baseknockback(k), basemodifier(m), knockback(k), modifier(m), invulnerability(i) {}

float DamageInfo::angleDifference(float angle1, float angle2) {
	float actualDifference = angle1 - angle2;
	float pi2 = 2 * M_PI;
	actualDifference -= floor(actualDifference / pi2) * pi2;
	return (actualDifference > M_PI) ? (pi2 - actualDifference) : actualDifference;
}

void DamageInfo::setAngle(float initialangle, float attackangle, Direction& diangle) {
//	cout << "a1.5\n";
	float x = 3 * cos(initialangle) + cos(attackangle) + diangle.magnitude * cos(diangle.angle);
	float y = 3 * sin(initialangle) + sin(attackangle) + diangle.magnitude * sin(diangle.angle);

	/*
	if (abs(diangle - M_PI) < 0.01) {
//		cout << "uwu\n";
		x -= cos(diangle);
		y -= sin(diangle);
	}
	*/

//	cout << "aa: " << attackangle << endl;
//	cout << "di: " << diangle << endl;
//	cout << "x: " << x << endl;
//	cout << "y: " << y << endl;
//	cout << "cos: " << cos(initialangle) << endl;
//	cout << "sin: " << sin(initialangle) << endl;

	angle = atan(y / x);
	if (x < 0) {
		angle += M_PI;
	}

	cout << "angle: " << angle << endl;

	float mod = 1 - (angleDifference(angle, initialangle) + sqrt(pow(x, 2) + pow(y, 2)) / 10);

	knockback = baseknockback * mod;
	modifier = basemodifier * mod;

//	cout << knockback << endl;
}