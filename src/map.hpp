#pragma once
#ifndef MAP_H
#define MAP_H

#include "tmx.h"

class Map {
	tmx_map *tmxMap;
	//Game &g;
public:
	Map(const char *filename);
	~Map();
	void draw(int x_offset, int y_offset, int width, int height);
	
};

#endif /* MAP_H */
