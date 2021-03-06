#pragma once
#ifndef DEF_PLAYER
#define DEF_PLAYER

#include "Character.hpp"

#include <allegro5/allegro.h>
#include <Box2D/Box2D.h>
#include <spine/spine-allegro.h>

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
	virtual void kill();
	virtual bool dead() const;
	virtual void attack(Character& c);

	void onAttack();

	void onMonsterContact();
	void onMonsterSeparation();

	void finish();	

	b2Vec2 getCenter() const; 

	virtual ~Player();

	private:
	b2BodyDef	bodyDef;
	b2Body*		body;
	b2FixtureDef fixtureDef;
	b2FixtureDef swordDef;
	b2Fixture*  swordFix;

	b2PolygonShape dynamicBox;

	// Spine
	static SkeletonDrawable *model;
	static spSkeletonData   *modelData;
	static spAtlas *modelAtlas;
	static spSkeletonJson *jsonSkel;
	static spAnimationStateData *stateData;

	bool left;
	bool right;
	bool landed;
	bool contact;
	int lastproc;

	int life;
	bool attacking;
	int attackcooldown;
};

#endif
