#include "Player.hpp"
#include "Game.hpp"

#include <string>
#include <cstdlib>

Player::Player(Game& g, b2Vec2 p) : Character(g), left(false), right(false), landed(true), contact(false), lastproc(0), life(100)
{
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(p.x, p.y);
	bodyDef.fixedRotation = true;

	body = g.w.CreateBody(&bodyDef);

	dynamicBox.SetAsBox(.25f, 0.9f);
	
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 5.0f;
	fixtureDef.friction = 0.0f;
	
	fixtureDef.filter.categoryBits = PLAYER;
	fixtureDef.filter.maskBits = MONSTER | TRIGGER | WALL | SWORD;

	(body->CreateFixture(&fixtureDef))->SetUserData(this);

	//add "feet" to detect floor
	dynamicBox.SetAsBox(0.1, 0.1, b2Vec2(0, 0.9f), 0);
	fixtureDef.isSensor = true;
	fixtureDef.density = 0.1;
	fixtureDef.filter.categoryBits = FOOT;
	fixtureDef.filter.maskBits = WALL | MONSTER;
	(body->CreateFixture(&fixtureDef))->SetUserData(this);

	//add sword to kill monsters
	dynamicBox.SetAsBox(0.4, 0.1, b2Vec2(0.65, 0), 0);
	swordDef.shape = &dynamicBox;
	swordDef.isSensor = true;
	swordDef.density = 0.1;
	swordDef.filter.categoryBits = SWORD;
	swordDef.filter.maskBits = MONSTER;
	(body->CreateFixture(&swordDef))->SetUserData(this);
}

Player::~Player()
{
	g.w.DestroyBody(body);
	body = NULL;
}

void Player::tick()
{
	//Velocity update
	float needvel;
	if(right && landed)
		needvel = 10.;
	else if(left && landed)
		needvel = -10.;
	else if(right && !landed)
		needvel = 7.;
	else if(left && !landed)
		needvel = -7.;
	else
		needvel = 0.;
	
	float velchange = needvel - body->GetLinearVelocity().x;
	float force = body->GetMass() * velchange / (1 /static_cast<float>( g.m.animation_tick));
	body->ApplyForce(b2Vec2(force, 0), body->GetWorldCenter(), true);
	

	//damage over time management
	if(contact)
	{
		if(lastproc == 0)
			damage(3);
			
		lastproc++;

		if(lastproc >= 60)
			lastproc = 0;

	}

	//sword side TO UPDATE TO FIT SPINE SIDE

}



void Player::draw() const
{
	al_draw_filled_rectangle(g.m.screen_w/2 - 16, g.m.screen_h/2 - 32, g.m.screen_w/2 + 16, g.m.screen_h/2 + 32, al_map_rgb(255, 0, 0));
	
	//hp bar
	al_draw_rectangle(al_get_display_width(g.m.display) / 2 - 102,
					  al_get_display_height(g.m.display) - 62,
					  al_get_display_width(g.m.display) / 2 + 102,
					  al_get_display_height(g.m.display) - 38,
					  al_map_rgb(80, 0, 0),
					  4);
	al_draw_filled_rectangle(al_get_display_width(g.m.display) / 2 - 100,
							 al_get_display_height(g.m.display) - 60,
							 al_get_display_width(g.m.display) / 2 - 100 + ((life >= 0) ? life * 2 : 0),
							 al_get_display_height(g.m.display) - 40,
							 al_map_rgb(255, 0, 0));

	if(Game::debug)
	{
		if(landed)
			al_draw_filled_circle(al_get_display_width(g.m.display) / 2 + 120,
							      al_get_display_height(g.m.display) - 50,
							      10,
							      al_map_rgb(0, 255, 0));
	}

}


void Player::moveLeft()
{
		left = true;
}

void Player::moveRight()
{	
		right = true;
}

void Player::jump()
{
	if(landed)
		body->ApplyLinearImpulse(b2Vec2(0, -30), body->GetWorldCenter(), true);
}

void Player::stopLeft()
{
	left = false;
}
void Player::stopRight()
{
	right = false;
}

b2Vec2 Player::getCenter() const
{
	return body->GetWorldCenter();
}

void Player::damage(int qt)
{
	life -= qt;
	if(life > 100)
		life = 100;
}
void Player::on_jump()
{
	landed = false;
}
void Player::on_land()
{
	landed = true;
}

void Player::onMonsterContact()
{
	contact = true;
	lastproc = 0;
	on_land();
}
void Player::onMonsterSeparation()
{
	contact = false;
	on_jump();
}

void Player::finish()
{
	g.finish();
}

void Player::kill()
{
	life = 0;
}

bool Player::dead() const
{
	return life <= 0;
}

void Player::attack(Character& c)
{
	c.damage(60);
}
