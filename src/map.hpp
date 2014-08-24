#pragma once
#ifndef MAP_H
#define MAP_H

#include "tmx.h"

class b2World;
class Player;
class Game;

class Map {
	tmx_map *tmxMap;
	b2World &wo;

	tmx_object *playerspawn;
	void render_map();
public:
	Map(const char *filename, b2World& w);
	~Map();
	void draw(int x_offset, int y_offset, int width, int height);

	Player* playerSpawn(Game &g) const;
};

#endif /* MAP_H */
