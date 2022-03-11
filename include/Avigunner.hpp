#pragma once

#include "Player.hpp"
#include "RenderWindow.hpp"
#include "Characters.hpp"
#include "Moves.hpp"
#include "HitscanRay.hpp"
#include "Spore.hpp"

class Avigunner : public Player {
public:
	Avigunner(RenderWindow& window, Controllers p);

	void attack(vector<GameObject*>& gameobjs, bool charge, Direction d);
	void applyFrame(vector<GameObject*>& gameobjs, Stage* s, Inputs& input);
};