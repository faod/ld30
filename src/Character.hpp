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
	virtual bool damage(int )  { return false;} //true if dmg were lethal therefore should kill (delete) character after

	virtual void on_jump(){}
	virtual void on_land(){}

	virtual ~Character() {}

	protected:
	Game& g;
};



#endif

