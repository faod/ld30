#pragma once
#ifndef DEF_PLAYER
#define DEF_PLAYER

#include "Character.hpp"

#include <allegro5/allegro.h>
#include <Box2D/Box2D.h>


class Player: public Character
{
	public:

	Player(Game &g, b2Vec2 p);

	virtual void tick();
	virtual void draw() const;
	
	virtual void moveLeft();
	virtual void moveRight();
	virtual void jump();
	virtual void stopLeft();
	virtual void stopRight();
	virtual void damage(int qt);
	virtual void on_jump();
	virtual void on_land();

	void onMonsterContact();
	void onMonsterSeparation();

	b2Vec2 getCenter() const; 

	virtual ~Player();

	private:
	ALLEGRO_BITMAP* bm;


	b2BodyDef	bodyDef;
	b2Body*		body;
	b2FixtureDef fixtureDef;
	b2PolygonShape dynamicBox;

	bool left;
	bool right;
	bool landed;
	bool contact;
	int lastproc;

	int life;
};

#endif
