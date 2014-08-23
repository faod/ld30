#pragma once
#ifndef DEF_MONSTER
#define DEF_MONSTER

#include "Character.hpp"

#include <allegro5/allegro.h>
#include <Box2D/Box2D.h>

class Monster : public Character
{
	public:
	Monster(Game& g, b2Vec2 p);

	virtual void tick();
	virtual void draw() const;

	virtual void moveLeft();
	virtual void moveRight();
	virtual void jump();

	virtual ~Monster();

	private:
	ALLEGRO_BITMAP *bm;

	b2BodyDef bodyDef;
	b2Body*	  body;
	b2FixtureDef fixtureDef;
	b2PolygonShape dynamicBox;
};

#endif
