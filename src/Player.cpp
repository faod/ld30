#include "Player.hpp"
#include "Game.hpp"

#include <string>
#include <cstdlib>

Player::Player(Game& g, b2Vec2 p) : Character(g), bm(al_create_bitmap(32, 64)), left(false), right(false), landed(true), contact(false), lastproc(0), life(100)
{
	al_set_target_bitmap(bm);
	al_clear_to_color(al_map_rgb(255, 0, 0));
	al_set_target_bitmap(al_get_backbuffer(this->g.m.display));


	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(p.x, p.y);
	bodyDef.fixedRotation = true;

	body = g.w.CreateBody(&bodyDef);

	dynamicBox.SetAsBox(.25f, 0.9f);
	
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 5.0f;
	fixtureDef.friction = 0.0f;
	
	fixtureDef.filter.categoryBits = PLAYER;
	fixtureDef.filter.maskBits = MONSTER | TRIGGER | WALL;

	body->CreateFixture(&fixtureDef);

	//add "feet" to detect floor
	dynamicBox.SetAsBox(0.1, 0.1, b2Vec2(0, 0.9f), 0);
	fixtureDef.isSensor = true;
	fixtureDef.density = 0.1;
	fixtureDef.filter.categoryBits = FOOT;
	fixtureDef.filter.maskBits = WALL | MONSTER;
	(body->CreateFixture(&fixtureDef))->SetUserData(this);
}

Player::~Player()
{
	al_destroy_bitmap(bm);
}

void Player::tick()
{
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
	
	if(contact)
	{
		if(lastproc == 0)
			damage(3);
			
		lastproc++;

		if(lastproc >= 60)
			lastproc = 0;

	}

}



void Player::draw() const
{
	al_draw_bitmap(bm, al_get_display_width(g.m.display)/2 -al_get_bitmap_width(bm) / 2 , al_get_display_height(g.m.display)/2 - al_get_bitmap_height(bm)/2 , 0);
	
	//hp bar
	al_draw_rectangle(al_get_display_width(g.m.display) / 2 - 102,
					  al_get_display_height(g.m.display) - 62,
					  al_get_display_width(g.m.display) / 2 + 102,
					  al_get_display_height(g.m.display) - 38,
					  al_map_rgb(80, 0, 0),
					  4);
	al_draw_filled_rectangle(al_get_display_width(g.m.display) / 2 - 100,
							 al_get_display_height(g.m.display) - 60,
							 al_get_display_width(g.m.display) / 2 - 100 + life * 2,
							 al_get_display_height(g.m.display) - 40,
							 al_map_rgb(255, 0, 0));

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
	if (life <= 0)
		g.m.loop = 0;
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
