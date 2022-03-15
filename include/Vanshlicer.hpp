#pragma once

#include "Player.hpp"
#include "RenderWindow.hpp"
#include "Characters.hpp"
#include "Moves.hpp"

class Vanshlicer : public Player {
public:
	Vanshlicer();
	Vanshlicer(RenderWindow& window, Controllers p);

	void attack(vector<GameObject*>& gameobjs, bool charge, Direction d);
	virtual GameObject* createObject();
};