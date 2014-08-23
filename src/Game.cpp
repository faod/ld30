#include "Game.hpp"
#include "Player.hpp"
#include "Monster.hpp"

Game::Game() : m(800, 600) ,  player(new Player(*this, b2Vec2(10., 10.)))
{
	characters.push_back(player);
	characters.push_back(new Monster(*this, b2Vec2(80., 10.)));
	characters.push_back(new Monster(*this, b2Vec2(140., 10.)));
}
Game::~Game()
{
	for(std::vector<Character*>::iterator it = characters.begin(), end = characters.end() ; it != end ; ++it)
		delete *it;
}

void* Game::startAnim(ALLEGRO_THREAD* t, void* arg)
{
	static_cast<Game*>(arg)->anim(t);
	return NULL;
}

void* Game::startInput(ALLEGRO_THREAD* t, void* arg)
{
	static_cast<Game*>(arg)->input(t);
	return NULL;
}

void* Game::startRefresh(void *arg)
{
	static_cast<Game*>(arg)->refresh();
	return NULL;
}

void Game::anim(ALLEGRO_THREAD* t)
{
	ALLEGRO_EVENT ev;

	al_start_timer(m.animationTimer);

 	while (m.loop)
	{
		al_wait_for_event(m.animationEQ, &ev);

		//process
	}
}

void Game::input(ALLEGRO_THREAD* t)
{
	ALLEGRO_EVENT ev;
 	while (m.loop)
	{
 		if(al_get_next_event(m.inputEQ, &ev))
		{
			if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
 			{
				m.loop = false;
				break;
			}
			else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				switch (ev.keyboard.keycode)
				{
					case ALLEGRO_KEY_LEFT: { this->player->moveLeft(); break;}
					case ALLEGRO_KEY_RIGHT: { this->player->moveRight(); break; }
					case ALLEGRO_KEY_UP: { this->player->jump(); break; }
				}
			}
//			if (ev.type == ALLEGRO_
		}
	}
}

void Game::refresh()
{
	ALLEGRO_EVENT ev;

	al_start_timer(m.refreshTimer);

 	while (m.loop)
	{
		al_wait_for_event(m.refreshEQ, &ev);
		al_clear_to_color(al_map_rgb(255, 255, 255));	

		for(std::vector<Character*>::const_iterator it = characters.begin(), end = characters.end() ; it != end; ++it)
		{
			(*it)->draw();
		}

		al_flip_display();
	}
}
