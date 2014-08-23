#include "Monster.hpp"
#include "Game.hpp"

Monster::Monster(Game& g, b2Vec2 p) : Character(g), bm(al_create_bitmap(32, 64)), pos(p)
{
	al_set_target_bitmap(bm);
	al_clear_to_color(al_map_rgb(0, 255, 0));
	al_set_target_bitmap(al_get_backbuffer(this->g.m.display));
}

Monster::~Monster()
{
	al_destroy_bitmap(bm);
}

void Monster::tick()
{
}

void Monster::draw() const
{
	al_draw_bitmap(bm, pos.x, pos.y, 0);
}
