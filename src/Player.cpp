#include <iostream>
#include <math.h>

#include "Player.hpp"
#include "RenderWindow.hpp"

using namespace std;

Player::Player() {

}

Player::Player(Character c, Controllers p) : character(c), playerNum(p) {
	spawn();
	setRect();
}

void Player::spawn() {
	y = 0;
	if (playerNum == PLAYER1) {
		x = 300;
	}
	if (playerNum == PLAYER2) {
		x = 900;
	}
}

void Player::zeroInitializeHitboxes() {
	hitboxChanges = vector<vector<vector<Circle>>>(ANIMATIONS);

	for (int x = 0; x < ANIMATIONS; x++) {
		hitboxChanges[x] = vector<vector<Circle>>(animationLength[x]);
//		cout << "animlen: " << animationLength[x] << endl;
		for (int y = 0; y < animationLength[x]; y++) {
			hitboxChanges[x][y] = vector<Circle>(baseHitboxes.size());
			for (int z = 0; z < baseHitboxes.size(); z++) {
				hitboxChanges[x][y][z] = Circle(0, 0, 0);
			}
		}
	}
}

void Player::applyFrame(vector<GameObject*>& gameobjs, Stage* s, Inputs& input) {
	Player* opponent = static_cast<Player*>(gameobjs.at(1 - playerNum));

	onGround = false;

	if (input.tech && techFrames > TECHLOCKOUT) {
		techFrames = 0;
	}
	else {
		techFrames++;
	}

	if (hitlag <= 0) {
		// cout << texture << endl;
		// cout << "EL AM AY OW" << endl;
		// cout << "x: " << x << " y: " << y << " xvel: " << xvel << " yvel: " << yvel << endl;
		for (int tempnum = hitboxes.size() - 1; tempnum >= 0; tempnum--) {
			Circle& c = hitboxes.at(tempnum);
			float angle;
			while (s->collidesWith(c)) {
				// cout << "x: " << x << " y: " << y << " c.x: " << c.x << " c.y: " << c.y << " xvel: " << xvel << " yvel: " << yvel << endl;
				angle = s->angleFrom(c.x, c.y, c.x - xvel, c.y - yvel);
				// angle = s->angleFrom(x - xvel + getDimentions()[0] / 2, y - yvel - (2 * c.radius) + getDimentions()[1]);
				// angle = s->angleFrom(x - xvel + getDimentions()[0] / 2, y - yvel + getDimentions()[1] / 2);
				// cout << "angel: " << angle << endl;
				// cout << s->collidesWith(c) << endl;

				if (abs(angle - M_PI / 2) < M_PI / 2) {
					onGround = true;
				}
				x += 5 * cos(angle);
				y -= 5 * sin(angle);
				c.x += 5 * cos(angle);
				c.y -= 5 * sin(angle);
			}
			if (onGround) {
				if (yvel > 0) {
					yvel = 0;
				}
				x -= 5 * cos(angle);
				y += 5 * sin(angle);
				break;
			}
		}

		// cout << previous.direction << endl;

		if (onGround && !wasOnGround) {
			if (!wasDown(previous.direction) && wasDown(input.direction)) {
				cout << "TECH: LLC\n";
				lag = 0;
				// TECH: Landing Lag Cancel
			}
			else {
				lag = landinglag;
				type = LAND;
			}
		}
		if (invulnerability > 0) {
			invulnerability--;
			invulnFlash++;
			if (invulnFlash > 2 * INVULNFLASHTIME) {
				invulnFlash = 0;
			}
		}

		// cout << lag << endl;
		// cout << type << endl;
		if ((charge == 0) == (input.charge && charge <= CHARGEMAX)) {
			cout << "power: " << charge << "\n";
			attack(gameobjs, true, input.direction);
		}

		if (lag <= 0 || type == DASH) {
			if (lag > 0) {
				lag--;
			}

			if (onGround && wasDown(input.direction)) {
				xvel *= slowDown;
			}
			// ACTIVE
			if (ableToMove) {
				if (input.direction.magnitude > moveMagnitude) {
					bool rest = abs(xvel) < 0.1;
					if (wasRight(input.direction)) {
						if (onGround && ((!facingRight && (lag > 0 || rest)) || (facingRight && lag == 0 && wasNeutral(previous.direction) && rest))) {
							lag = dashframes;
							type = DASH;
							xvel += dashspeed;
							facingRight = !facingRight;
						}
						else if (onGround && !facingRight && lag == 0) {
							lag = turnAround;
							type = TURNAROUND;
						}
						else {
							xvel += walkspeed;
						}
					}
					if (wasLeft(input.direction)) {
						if (onGround && ((facingRight && (lag > 0 || rest)) || (!facingRight && lag == 0 && wasNeutral(previous.direction) && rest))) {
							lag = dashframes;
							type = DASH;
							xvel -= dashspeed;
							facingRight = !facingRight;
						}
						else if (onGround && facingRight && lag == 0) {
							lag = turnAround;
							type = TURNAROUND;
						}
						else {
							xvel -= walkspeed;
						}
					}
				}
				if (input.jump && !previous.jump) {
					if (onGround) {
						lag = jumpSquat;
						type = JUMP;
					}
					else {
						if (jumpsUsed < maxjumps) {
							yvel = -jumpspeed;
							jumpsUsed++;
						}
					}
				}
			}
			// cout << "Lag: " << lag << endl;
			if (input.quick && !previous.quick) {
				attack(gameobjs, false, input.direction);
			}
			if (input.charge) {
				lag = 1;
				type = CHARGE;
			}
		}
		else {
			lag--;

			if (type == TURNAROUND) {
				xvel /= 2;
			}
			if (!onGround) {
				xvel += cos(input.direction.angle) * input.direction.magnitude * walkspeed;
				if (type == LAND) {
					// TECH: Edge Cancel
					lag = 0;
				}
			}

			if (lag == 0) {
				switch (type) {
					case CHARGE:
						charge++;
						lag = 1;
						if (!onGround) {
							// Extra drag when charging in the air
							xvel *= drag;
							yvel *= drag;
						}
						break;
					case JUMP:
						yvel = -jumpspeed;
						if (!input.jump) {
							yvel *= shorthop;
						}
						jumpsUsed++;
						break;
					case TURNAROUND:
						facingRight = !facingRight;
						break;
				}
				if (type != CHARGE) {
					type = NONE;
				}
			}
		}


		// ALWAYS HAPPEN
		if (onGround) {
			jumpsUsed = 1;
			fastFalling = false;

			xvel *= friction;
			yvel *= friction;
		}
		else {
			if (!wasDown(previous.direction) && wasDown(input.direction) && ((yvel < fallspeed) || (yvel > 0))) {
				fastFalling = true;
				// TECH: Fast Fall
				cout << "TECH: FF\n";
			}

			if (fastFalling) {
				yvel += fallspeed * fastFall;

				if (!wasUp(previous.direction) && wasUp(input.direction)) {
					fastFalling = false;
					// TECH : Fast Fall Cancel
					cout << "TECH: FFC\n";
				}
			}
			else {
				yvel += fallspeed;
			}

			xvel *= drag;
			yvel *= drag;
		}
		if (input.secondStick.magnitude > minMagnitude) {
			secondStick(gameobjs, input.secondStick);
		}

	}
	else {
		// Hitlag options
		hitlag--;
		if (wasNeutral(previous.direction) && wasDown(input.direction)) {
			fastFalling = true;
		}
		if (wasNeutral(previous.direction) && wasUp(input.direction)) {
			fastFalling = false;
		}

		Direction change = input.direction - previous.direction;
		xvel += cos(change.angle) * change.magnitude;
		yvel -= sin(change.angle) * change.magnitude;
	}

	previous.direction = input.direction;
	wasOnGround = onGround;

	// cout << type << endl;

	switch (type) {
		case NONE:
			animationtype = JAB;
			animationFrame = 0;
			break;
		case JUMP:
			animationtype = SQUAT;
			animationFrame = (jumpSquat - lag) * animationLength[SQUAT] / jumpSquat;
			break;
		case LAND:
			animationtype = SQUAT;
			animationFrame = (landinglag - lag) * animationLength[SQUAT] / landinglag;
			break;
		case TURNAROUND:
			animationtype = TURN;
			animationFrame = (turnAround - lag) * animationLength[TURN] / turnAround;
			break;
		case ATTACK:
			animationtype = JAB;
			if (charge == 0) {
				animationFrame = 8 - lag;
			}
			else {
				// animationFrame = lag + 4;
			}
			break;
/*
		case HIT:
		case CHARGE:
			animationtype = JAB;
			animationFrame = 4;
			break;
*/
	}

	modifier = 0;
	if (!facingRight) {
		modifier += 1;
	}
	if (invulnerability > 0 && invulnFlash < INVULNFLASHTIME) {
		modifier += 2;
	}



	for (int z = 0; z < baseHitboxes.size(); z++) {
		Circle& c = hitboxes[z];
		c = baseHitboxes[z] + hitboxChanges[animationtype][animationFrame][z];
		c.onHit = hitboxChanges[animationtype][animationFrame][z].onHit;
		c.x = x + xvel;
		c.y = c.basey + y + yvel;

		if (facingRight) {
			c.x += c.basex;
		}
		else {
			c.x += getDimentions()[0] - c.basex;
		}

		if (c.attack) {
			if (opponent->collides(c)) {
				// cout << "a\n";
				// cout << &c << endl;
				// cout << c.onHit << endl;
				// cout << c.onHit->angle << endl;
				c.onHit.setAngle(c.onHit.angle, opponent->angleBetween(c.x, c.y), opponent->previous.direction);
				// cout << "b\n";
				opponent->damage(c.onHit, this);
				// cout << "c\n";
			}
		}
	}

	// DEATH
	if (abs(x - 600) > 2000 || abs(y - 600) > 2000) {
		cout << "DEATH" << endl;
		// cout << "x: " << x << " y: " << y << " xvel: " << xvel << " yvel: " << yvel << endl;
		spawn();
		invulnerability = 120;
		percent = 0;
		stale.clear();
		// cout << 0 / 0 << endl;
	}
	
	previous = input;
	setRect();
}

bool Player::draw(vector<GameObject*>& gameobjs, Stage* stage) {
	if (hitlag <= 0) {
		return GameObject::draw(gameobjs, stage);
	}
	return false;
}


Circle* Player::collides(int x, int y) {
	for (Circle& c : hitboxes) {
		if (c.collides(x, y)) {
			return &c;
		}
	}
	return nullptr;
}

Circle* Player::collides(Circle& c2) {
	for (Circle& c : hitboxes) {
		if (c.collides(c2)) {
			return &c;
		}
	}
	return nullptr;
}

float Player::angleBetween(int tempx, int tempy) {
	vector<int>& iTemp = getDimentions(); // Apples chest moment
	int anglex = x + (iTemp[0] / 2) - tempx;
	int angley = y + (iTemp[1] / 2) - tempy;
	float angle;
	if (anglex == 0) {
		angle = (tempy > 0) ? M_PI / 2.0 : M_PI / -2.0;
	}
	else {
		angle = atan(angley / anglex);
	}


	if (anglex < 0) {
		angle += M_PI;
	}
	return angle;
}

void Player::damage(DamageInfo& damageinfo, Player* attacker) {
	if (invulnerability <= 0) {
		float staled = 1;
		int timesUsed = 0;

		list<Move>& attackerstale = attacker->stale;

		auto it = attackerstale.begin();

	//	cout << attackerstale.size() << endl;

		for (int x = 0; x < attackerstale.size(); x++) {
			if (*it == attacker->inUse) {
				timesUsed++;
				staled -= ((9.0 - x) / 100.0);
			}
			it++;
		}

	//	cout << staled << endl;

		if (attacker->attackNumber != lastAttackNumHitBy) {
			lastAttackNumHitBy = attacker->attackNumber;
			attackerstale.push_front(attacker->inUse);

			if (attackerstale.size() > 9) {
				attackerstale.pop_back();
			}
		}

		percent += damageinfo.damage * staled;
//		cout << damageinfo.damage << endl;
		int totalKnockback = staled * (damageinfo.knockback + damageinfo.modifier * percent);

		invulnerability = damageinfo.invulnerability;

		if (type != HIT) {
			lag = 0;
		}

		lag = totalKnockback * hitstunModifier + 1;
		hitlag += damageinfo.damage * staled / 3 + damageinfo.invulnerability;
		type = HIT;
		
//		cout << "ending angle: " << damageinfo.angle << endl;
//		cout << "totkKB: " << totalKnockback << endl;
//		cout << "stdaly: " << staled << endl;
//		cout << damageinfo.knockback << endl;
//		cout << damageinfo.modifier << endl;
//		cout << percent << endl;

		xvel += cos(damageinfo.angle) * totalKnockback;
		yvel -= sin(damageinfo.angle) * totalKnockback;
	}
}

void Player::attack(vector<GameObject*>& gameobjs, bool charge, Direction d) {
//	cout << "bro...\n";
	if (charge) {
		if (this->charge == 0) {
			// START CHARGING
			inUse = ForwardQuick; // Temp no
			this->charge = 1;
			bool right = wasRight(d);
			bool left = wasLeft(d);
			if (right || left) {
				if ((right && facingRight) || (left && !facingRight)) {
					inUse = ForwardCharge;
					startForwardCharge(gameobjs, d);
				}
				else {
					inUse = BackCharge;
					startBackCharge(gameobjs, d);
				}
			}
			else {
				if (wasUp(d)) {
					inUse = UpCharge;
					startUpCharge(gameobjs, d);
				}
			}
		}
		else {
			// RELEASE CHARGE
			type = NONE;
			if (inUse == ForwardCharge) {
				releaseForwardCharge(gameobjs, d);
			}
			if (inUse == BackCharge) {
				releaseBackCharge(gameobjs, d);
			}
			if (inUse == UpCharge) {
				releaseUpCharge(gameobjs, d);
			}

			this->charge = 0;
		}
	}
	else {
		if (onGround) {
			// QUICK
			if ((wasRight(d) && facingRight) || (wasLeft(d) && !facingRight)) {
				inUse = ForwardQuick;
				forwardQuick(gameobjs, d);
			}
			if (wasDown(d)) {
				inUse = DownQuick;				
				downQuick(gameobjs, d);
			}
		}
		else {
			// AERIALS
			if ((wasRight(d) && facingRight) || (wasLeft(d) && !facingRight)) {
				inUse = ForwardAerial;
				forwardAerial(gameobjs, d);
			}
		}
	}

	attackNumber++;
}

void Player::startForwardCharge(vector<GameObject*>& gameobjs, Direction d) {

}

void Player::startBackCharge(vector<GameObject*>& gameobjs, Direction d) {

}

void Player::startUpCharge(vector<GameObject*>& gameobjs, Direction d) {

}

void Player::releaseForwardCharge(vector<GameObject*>& gameobjs, Direction d) {

}

void Player::releaseBackCharge(vector<GameObject*>& gameobjs, Direction d) {

}

void Player::releaseUpCharge(vector<GameObject*>& gameobjs, Direction d) {

}

void Player::forwardQuick(vector<GameObject*>& gameobjs, Direction d) {

}

void Player::downQuick(vector<GameObject*>& gameobjs, Direction d) {

}

void Player::forwardAerial(vector<GameObject*>& gameobjs, Direction d) {

}

void Player::secondStick(vector<GameObject*>& gameobjs, Direction d) {
	
}

GameObject* Player::createObject() {
	return new Player(*this);
}