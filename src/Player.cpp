#include "Player.hpp"
#include "Game.hpp"

Player::Player(Game& g, b2Vec2 p) : Character(g), bm(al_create_bitmap(32, 64))
{
	al_set_target_bitmap(bm);
	al_clear_to_color(al_map_rgb(255, 0, 0));
	al_set_target_bitmap(al_get_backbuffer(this->g.m.display));

	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(p.x, p.y);

	body = g.w.CreateBody(&bodyDef);

	dynamicBox.SetAsBox(1.0f, 1.0f);
	
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.5f;

	body->CreateFixture(&fixtureDef);
}

Player::~Player()
{
	al_destroy_bitmap(bm);
}

void Player::tick()
{
}

void Player::draw() const
{
	al_draw_bitmap(bm, body->GetPosition().x * Game::pixelpm, body->GetPosition().y * Game::pixelpm, 0);
}
