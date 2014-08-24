#pragma once
#ifndef DEF_CHARACTER
#define DEF_CHARACTER

#include <iostream>

class Game;

class Character
{
	public:
	Character(Game& ga) : g(ga) {}
	virtual void tick() = 0;
	virtual void draw() const = 0;

	virtual void moveLeft() {}
	virtual void moveRight() {}
	virtual void jump() {}
	virtual void stopLeft() {}
	virtual void stopRight() {}
	virtual void damage(int qt)  {}

	virtual ~Character() {}

	protected:
	Game& g;
};



#endif

