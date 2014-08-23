#pragma once

#ifndef DEF_GAME
#define DEF_GAME

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>


#include "main.hpp"
#include "Character.hpp"
#include "Player.hpp"
#include "map.hpp"

#include <Box2D/Box2D.h>

#include <vector>
#include <memory>

class Game
{
	public:
	
	Game(char *mapPath);
	~Game();

	static const int pixelpm = 128; 

	static void *startAnim(ALLEGRO_THREAD* t, void *arg);
	static void *startInput(ALLEGRO_THREAD* t, void *arg);
	static void *startRefresh(void *arg);

	
	//RETURNS COORD IN PIXEL
	b2Vec2 getScreenCorner() const;

	Main m;
	b2World w;
	Map map;

	private:
	void anim(ALLEGRO_THREAD*);
	void input(ALLEGRO_THREAD*);
	void refresh();

	std::vector<Character*> characters; //all the characters list
	Player* player;						//quick reference to the special character player
};

#endif


