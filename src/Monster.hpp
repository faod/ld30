#pragma once
#ifndef DEF_MONSTER
#define DEF_MONSTER

#include "Character.hpp"

#include <allegro5/allegro.h>
#include <Box2D/Box2D.h>
#include <spine/spine-allegro.h>

class Monster : public Character
{
	public:
	Monster(Game& g, b2Vec2 p);

	virtual void tick();
	virtual void draw() const;

	virtual void moveLeft();
	virtual void moveRight();
	virtual void jump();
	bool goingRight();

	virtual void damage(int);
	virtual void stopLeft();
	virtual void stopRight();
	virtual void kill();
	virtual bool dead() const;
	virtual void on_jump();
	virtual void on_land();
	virtual void attack(Character&);
	virtual ~Monster();

	private:

	b2BodyDef bodyDef;
	b2Body*	  body;
	b2FixtureDef fixtureDef;
	b2PolygonShape dynamicBox;

	// Spine
	SkeletonDrawable *model;
	
	static spSkeletonData   *modelData;
	static spAtlas *modelAtlas;
	static spSkeletonJson *jsonSkel;
	static spAnimationStateData *stateData;

	b2FixtureDef swordDef;
	b2Fixture* swordFix;

	
	//states
	bool right;
	bool resting;
	bool attacking;
	int attackcooldown;
	int life;
};

#endif
