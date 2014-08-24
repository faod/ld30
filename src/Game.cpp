#include "Game.hpp"
#include "Player.hpp"
#include "Monster.hpp"

bool Game::debug = true;

Game::Game(char *mapPath) : m(1280, 720), w(b2Vec2(0.0f, 10.0f)), map(mapPath, w), player(NULL)
{
	player = new Player(*this, b2Vec2(3., 4.5));
	characters.push_back(player);
	characters.push_back(new Monster(*this, b2Vec2(2., 2.)));
	characters.push_back(new Monster(*this, b2Vec2(3., 1.)));
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

void Game::anim(ALLEGRO_THREAD* )
{
	ALLEGRO_EVENT ev;

	al_start_timer(m.animationTimer);

 	while (m.loop)
	{
		al_wait_for_event(m.animationEQ, &ev);
		
		w.Step(1./m.animation_tick, 6, 2);
		
		for(std::vector<Character*>::iterator it = characters.begin(), end = characters.end(); it != end; ++it)
			(*it)->tick();
	}
}

void Game::input(ALLEGRO_THREAD* )
{
	ALLEGRO_EVENT ev;
 
 		while(al_wait_for_event(m.inputEQ, &ev), m.loop)
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
					default : break;
				}
			}
			else if (ev.type == ALLEGRO_EVENT_KEY_UP)
			{
				switch(ev.keyboard.keycode)
				{
					case ALLEGRO_KEY_LEFT: { this->player->stopLeft(); break; }
					case ALLEGRO_KEY_RIGHT: { this->player->stopRight(); break; }
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
	
		b2Vec2 pos = getScreenCorner();
		map.draw(pos.x, pos.y, m.screen_w, m.screen_h);

		for(std::vector<Character*>::const_reverse_iterator it = characters.rbegin(), end = characters.rend() ; it != end; ++it)
		{
			(*it)->draw();
		}

		if(debug)
		{
			b2Body* elem = w.GetBodyList();
			while(elem)
			{
				b2Fixture* fixture = elem->GetFixtureList();
				while(fixture)
				{
					b2PolygonShape *polygon = dynamic_cast<b2PolygonShape*>(fixture->GetShape());
					
					float x1, x2, y1, y2;
					b2Vec2 worldc; 

					const b2Vec2 screen = getScreenCorner();
					int i;
					for(i=0; i<polygon->GetVertexCount()-1; i++) {
						
						worldc = elem->GetWorldPoint(polygon->GetVertex(i));
						x1 = worldc.x * Game::pixelpm;
						y1 = worldc.y * Game::pixelpm;
						
						worldc = elem->GetWorldPoint(polygon->GetVertex(i + 1));
						x2 = worldc.x * Game::pixelpm;
						y2 = worldc.y * Game::pixelpm;
					
						al_draw_line(x1 - screen.x, y1 - screen.y, x2 - screen.x, y2 - screen.y, al_map_rgb(0,0,255), 2.);
					}
						worldc = elem->GetWorldPoint(polygon->GetVertex(i));
						x1 = worldc.x * Game::pixelpm;
						y1 = worldc.y * Game::pixelpm;
						
						worldc = elem->GetWorldPoint(polygon->GetVertex(0));
						x2 = worldc.x * Game::pixelpm;
						y2 = worldc.y * Game::pixelpm;
					
						al_draw_line(x1 - screen.x, y1 - screen.y, x2 - screen.x, y2 - screen.y, al_map_rgb(0,0,255), 2.);

					fixture = fixture->GetNext();
				}
					


				elem = elem->GetNext();
			}

		}
			
		al_flip_display();
	}
}


b2Vec2 Game::getScreenCorner() const
{
	return b2Vec2(player->getCenter().x * pixelpm - al_get_display_width(m.display) / 2, player->getCenter().y * pixelpm - al_get_display_height(m.display) /2);
}
