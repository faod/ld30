#pragma once
#ifndef DEF_CHARACTER
#define DEF_CHARACTER

class Game;

class Character
{
	public:
	Character(Game& ga) : g(ga) {}
	virtual void tick() = 0;
	virtual void draw() const = 0;

	virtual ~Character() {}

	protected:
	Game& g;
};



#endif

