#pragma once

#include "Player.hpp"
#include "RenderWindow.hpp"
#include "Characters.hpp"
#include "Moves.hpp"
#include "HitscanRay.hpp"
#include "Spore.hpp"
#include "HookShot.hpp"

class Avigunner : public Player {
public:
	Avigunner();
	Avigunner(RenderWindow& window, Controllers p);

	void startForwardCharge(vector<GameObject*>& gameobjs, Direction d);
	void startBackCharge(vector<GameObject*>& gameobjs, Direction d);
	void releaseForwardCharge(vector<GameObject*>& gameobjs, Direction d);
	void releaseBackCharge(vector<GameObject*>& gameobjs, Direction d);
	void forwardQuick(vector<GameObject*>& gameobjs, Direction d);
	void downQuick(vector<GameObject*>& gameobjs, Direction d);
	void forwardAerial(vector<GameObject*>& gameobjs, Direction d);

	void applyFrame(vector<GameObject*>& gameobjs, Stage* s, Inputs& input);
	GameObject* createObject();
};