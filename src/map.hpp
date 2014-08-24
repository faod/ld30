#pragma once
#ifndef MAP_H
#define MAP_H

#include "tmx.h"

class b2World;

class Map {
	tmx_map *tmxMap;
	b2World &w;
public:
	Map(const char *filename, b2World& w);
	~Map();
	void draw(int x_offset, int y_offset, int width, int height);
	
};

#endif /* MAP_H */
