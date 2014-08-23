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

#include <vector>
#include <memory>

class Game
{
	public:
	
	Game();
	~Game();

	static void *startAnim(ALLEGRO_THREAD* t, void *arg);
	static void *startInput(ALLEGRO_THREAD* t, void *arg);
	static void *startRefresh(void *arg);

	Main m;
	
	private:
	void anim(ALLEGRO_THREAD*);
	void input(ALLEGRO_THREAD*);
	void refresh();

	std::vector<Character*> characters;

};

#endif


