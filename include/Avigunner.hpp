#pragma once

#include "Player.hpp"
#include "RenderWindow.hpp"
#include "Characters.hpp"
#include "Moves.hpp"
#include "HitscanRay.hpp"
#include "Spore.hpp"
#include "GrapplingHook.hpp"
#include "HookShot.hpp"

class Avigunner : public Player {
public:
	bool hookOut = false;
	int storedCharges = 0;
	const int ROCKETTIMER = 10;

	Avigunner();
	Avigunner(RenderWindow& window, Controllers p);

	void startForwardCharge(vector<GameObject*>& gameobjs, Direction d);
	void startBackCharge(vector<GameObject*>& gameobjs, Direction d);
	void startUpCharge(vector<GameObject*>& gameobjs, Direction d);
	void releaseForwardCharge(vector<GameObject*>& gameobjs, Direction d);
	void releaseBackCharge(vector<GameObject*>& gameobjs, Direction d);
	void releaseUpCharge(vector<GameObject*>& gameobjs, Direction d);
	void forwardQuick(vector<GameObject*>& gameobjs, Direction d);
	void downQuick(vector<GameObject*>& gameobjs, Direction d);
	void forwardAerial(vector<GameObject*>& gameobjs, Direction d);

	void secondStick(vector<GameObject*>& gameobjs, Direction d);

	void applyFrame(vector<GameObject*>& gameobjs, Stage* s, Inputs& input);
	GameObject* createObject();
};