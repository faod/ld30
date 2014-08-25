#pragma once
#ifndef MAP_H
#define MAP_H

#include "tmx.h"
#include <vector>

class b2World;
class Player;
class Game;
class Monster;

class Map {
	tmx_map *tmxMap;
	unsigned long width, height;
	ALLEGRO_COLOR backgroundcolor;

	b2World &wo;

	tmx_object *playerspawn;
	std::vector<tmx_object*> monsterspawn;

	// Split the map into several video bitmaps
	ALLEGRO_BITMAP **bmp;
	int bmp_x_count, bmp_y_count;
	void split_map(ALLEGRO_BITMAP *map);

	void render_map();
public:
	Map(const char *filename, b2World& w);
	~Map();
	void draw(int x_offset, int y_offset, int width, int height);

	Player* playerSpawn(Game &g) const;
	Monster* monsterSpawn(Game &g);
};

#endif /* MAP_H */
