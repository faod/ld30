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

	virtual void moveLeft() = 0;
	virtual void moveRight() = 0;
	virtual void jump()  = 0;
	virtual void stopLeft()  = 0;
	virtual void stopRight()  = 0;
	virtual void damage(int )  = 0;
	virtual bool dead() const  = 0;
	virtual void kill() = 0;
	virtual void on_jump() = 0;
	virtual void on_land() = 0;
	virtual void attack(Character&) = 0; 

	virtual ~Character() {}

	protected:
	Game& g;
};



#endif

