#include "Game.hpp"
#include "Player.hpp"
#include "Monster.hpp"

#include "failure.hpp"

#include <algorithm>

bool Game::debug = true;

struct IsDead
{
	Character** p;
	bool operator()(Character* &c)
	{
		bool ret =  c == NULL || c->dead();
		if (ret && c)
		{
			if(*p == c)
				*p = NULL;
			delete c;
		}
		return ret;
	}
};


Game::Game(char *mapPath) : m(1280, 720), w(b2Vec2(0.0f, 10.0f)), map(mapPath, w),  screenCorner(0,0), playerCenter(0,0),  player(NULL)
{
	w.SetContactListener(&listener);
	player = map.playerSpawn(*this);
	playerCenter = player->getCenter();
	characters.push_back(player);

	Monster* monster;
	while((monster = map.monsterSpawn(*this)) != NULL)
		characters.push_back(monster);

	//tmp = al_create_bitmap(m.screen_w, m.screen_h);
	//if (!tmp) throw Failure("Game::Game(): Failed to create bitmap!");
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
	
		if(player)
		{
			for(std::vector<Character*>::iterator it = characters.begin(), end = characters.end(); it != end; ++it)
			{
				(*it)->tick();
			}
		}

		if (player)
		{
			playerCenter = player->getCenter();
			screenCorner.Set(playerCenter.x * pixelpm - al_get_display_width(m.display) / 2, playerCenter.y * pixelpm - al_get_display_height(m.display) /2);
		}

		IsDead d;
		d.p = reinterpret_cast<Character**>(&player);
		std::vector<Character*>::iterator del;
		if((del = std::remove_if(characters.begin(), characters.end(), d)) != characters.end())
		{
			characters.erase(del, characters.end());
		}
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
			else if(!player) continue;
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

		if(player)
		{
			b2Vec2 pos = getScreenCorner();
			map.draw(pos.x, pos.y, m.screen_w, m.screen_h);
		

		for(std::vector<Character*>::const_reverse_iterator it = characters.rbegin(), end = characters.rend() ; it != end; ++it)
		{
			if(*it)
				(*it)->draw();
		}
		}

		if(debug && player)
		{
			b2Body* elem = w.GetBodyList();
			while(elem)
			{
				const b2Vec2 screen = getScreenCorner();
				//Draw center +velocity vector
				b2Vec2 center = elem->GetWorldCenter();
				al_draw_line(center.x * Game::pixelpm - (screen.x + 5), center.y * Game::pixelpm - screen.y,
							 center.x * Game::pixelpm - (screen.x - 5), center.y * Game::pixelpm - screen.y,
							 al_map_rgb(0, 255, 0), 1.);

				al_draw_line(center.x * Game::pixelpm - screen.x, center.y * Game::pixelpm - (screen.y + 5),
							 center.x * Game::pixelpm - screen.x, center.y * Game::pixelpm - (screen.y - 5),
							 al_map_rgb(0, 255, 0), 1.);
				b2Vec2 velo = elem->GetLinearVelocity();
				al_draw_line(center.x * Game::pixelpm - screen.x, center.y * Game::pixelpm - screen.y,
							 center.x * Game::pixelpm - screen.x + (velo.x * 5), center.y * Game::pixelpm - screen.y + (velo.y * 5), al_map_rgb(0, 255, 0), 1.);

				//Draw shapes
				b2Fixture* fixture = elem->GetFixtureList();
				while(fixture)
				{
					ALLEGRO_COLOR col;
					switch(fixture->GetFilterData().categoryBits)
					{
						case PLAYER:	
							col = al_map_rgb(128, 0, 0);
							break;
						case WALL:
							col = al_map_rgb(0, 0, 255);
							break;
						case TRIGGER:
							col = al_map_rgb(0, 128, 0);
							break;
						case MONSTER:
							col = al_map_rgb(0, 0, 128);
							break;
						default:
							col = al_map_rgb(0, 0, 0);
							break;
					};


					b2PolygonShape *polygon = dynamic_cast<b2PolygonShape*>(fixture->GetShape());
					
					float x1, x2, y1, y2;
					b2Vec2 worldc; 

					int i;
					for(i=0; i<polygon->GetVertexCount()-1; i++) {
						
						worldc = elem->GetWorldPoint(polygon->GetVertex(i));
						x1 = worldc.x * Game::pixelpm;
						y1 = worldc.y * Game::pixelpm;
						
						worldc = elem->GetWorldPoint(polygon->GetVertex(i + 1));
						x2 = worldc.x * Game::pixelpm;
						y2 = worldc.y * Game::pixelpm;
					
						al_draw_line(x1 - screen.x, y1 - screen.y, x2 - screen.x, y2 - screen.y, col, 2.);
					}
						worldc = elem->GetWorldPoint(polygon->GetVertex(i));
						x1 = worldc.x * Game::pixelpm;
						y1 = worldc.y * Game::pixelpm;
						
						worldc = elem->GetWorldPoint(polygon->GetVertex(0));
						x2 = worldc.x * Game::pixelpm;
						y2 = worldc.y * Game::pixelpm;
					
						al_draw_line(x1 - screen.x, y1 - screen.y, x2 - screen.x, y2 - screen.y, col, 2.);

					fixture = fixture->GetNext();
				}
					


				elem = elem->GetNext();
			}

		}
			
		al_flip_display();
	}
}

void Game::finish()
{
	//TODO
}

b2Vec2 Game::getScreenCorner() const
{
	return screenCorner;
}
b2Vec2 Game::getPlayerCenter() const
{
	return playerCenter;
}
