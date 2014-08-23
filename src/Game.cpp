#include "Game.hpp"

Game::Game() : m(800, 600)
{
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

		//process
	}
}
