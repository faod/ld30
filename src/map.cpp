#include <allegro5/allegro.h>

#include "map.hpp"
#include "failure.hpp"

#include "Game.hpp"
#include "Monster.hpp"

#include <Box2D/Box2D.h>
#include <cassert>
#include <string>

#define BMP_MS 2048

ALLEGRO_COLOR int_to_al_color(int color) {
	unsigned char r, g, b;
	
	r = (color >> 16) & 0xFF;
	g = (color >>  8) & 0xFF;
	b = (color)       & 0xFF;
	
	return al_map_rgb(r, g, b);
}

void* al_img_loader(const char *path) {
	ALLEGRO_BITMAP *res    = NULL;
	ALLEGRO_PATH   *alpath = NULL;
	
	if (!(alpath = al_create_path(path))) return NULL;
	al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
	res = al_load_bitmap(al_path_cstr(alpath, ALLEGRO_NATIVE_PATH_SEP));
	al_set_new_bitmap_flags(0);
	al_destroy_path(alpath);
	
	return (void*)res;
}

int gid_extract_flags(unsigned int gid) {
	int res = 0;
	
	if (gid & TMX_FLIPPED_HORIZONTALLY) res |= ALLEGRO_FLIP_HORIZONTAL;
	if (gid & TMX_FLIPPED_VERTICALLY)   res |= ALLEGRO_FLIP_VERTICAL;
	// FIXME allegro has no diagonal flip
	return res;
}

int gid_clear_flags(unsigned int gid) {
	return gid & TMX_FLIP_BITS_REMOVAL;
}

void render_layer(tmx_map *map, tmx_layer *layer) {
	unsigned long i, j;
	unsigned int x, y, w, h, flags;
	float op;

	tmx_tileset *ts;
	ALLEGRO_BITMAP *tileset;
	op = layer->opacity;

	for (i=0; i<map->height; i++) {
		for (j=0; j<map->width; j++) {
			ts = tmx_get_tile(map, layer->content.gids[(i*map->width)+j], &x, &y);
			if (ts) {
				w = ts->tile_width; h = ts->tile_height;
				tileset = (ALLEGRO_BITMAP*)ts->image->resource_image;
				flags = gid_extract_flags(layer->content.gids[(i*map->width)+j]);
				al_draw_tinted_bitmap_region(tileset, al_map_rgba_f(op, op, op, op), x, y, w, h, j*ts->tile_width, i*ts->tile_height, flags);
			}
		}
	}
}

// Splits the map in several video bitmaps
void Map::split_map(ALLEGRO_BITMAP *map) {
	unsigned long w_offset, h_offset;

	bmp_x_count = ceil( width/(float)BMP_MS);
	bmp_y_count = ceil(height/(float)BMP_MS);
	int bmp_count = bmp_x_count * bmp_y_count;

	bmp = new ALLEGRO_BITMAP*[bmp_count];
	if (!bmp) throw Failure("Map::split_map(): failed to allocate array of bitmaps!");

	for (int i=0; i<bmp_count; i++) {
		bmp[i] = al_create_bitmap(BMP_MS, BMP_MS);
		if (!bmp[i]) throw Failure("Map::split_map(): failed to create bitmap!");
	}

	for (int i=0; i<bmp_x_count; i++) {
		for (int j=0; j<bmp_y_count; j++) {
			w_offset = i*BMP_MS;
			h_offset = j*BMP_MS;

			al_set_target_bitmap(bmp[j*bmp_x_count+i]);
			al_clear_to_color(al_map_rgba_f(0., 0., 0., 0.));

			al_draw_bitmap_region(map, w_offset, h_offset, BMP_MS, BMP_MS, 0, 0, 0);
		}
	}
}

void Map::render_map() {
	ALLEGRO_BITMAP *layer_bmp = NULL;
	tmx_layer *layers = tmxMap->ly_head;
	
	width  = tmxMap->width  * tmxMap->tile_width;  // Bitmap's width and height 
	height = tmxMap->height * tmxMap->tile_height;
	backgroundcolor = int_to_al_color(tmxMap->backgroundcolor);

	while (layers) {
		if (bmp == NULL && layers->type == L_LAYER && layers->visible) {
			al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
			if (!(layer_bmp = al_create_bitmap(width, height))) throw Failure("Map::render_map(): failed to create bitmap!");
			al_set_new_bitmap_flags(0);
			al_set_target_bitmap(layer_bmp);
			al_clear_to_color(al_map_rgba_f(0., 0., 0., 0.));

			render_layer(tmxMap, layers);
			split_map(layer_bmp);
		}
		else if(layers->type == L_OBJGR)
		{
			tmx_object *object = layers->content.head;
			processObjects(object);
		}
		layers = layers->next;
	}
	al_set_new_bitmap_flags(0);
	al_set_target_backbuffer(al_get_current_display());
}

void Map::processObjects(tmx_object* object)
{
	while(object)
	{
		switch(object->shape)
		{	
			case S_SQUARE:
			{
				if(std::string("spawn") == object->name)
				{
					playerspawn = object;
					break;
				}
				if(std::string(object->name).find("monsterspawn") != std::string::npos)
				{
					monsterspawn.push_back(object);
					break;
				}
						
				b2BodyDef groundBodyDef;
				float width = object->width / static_cast<float>(Game::pixelpm);
				float height = object->height / static_cast<float>(Game::pixelpm);

				groundBodyDef.position.Set(object->x / static_cast<float>(Game::pixelpm) + width / 2.f, object->y / static_cast<float>(Game::pixelpm) + height / 2.f);
						
				b2Body* groundBody = wo.CreateBody(&groundBodyDef);

				b2PolygonShape groundBox;
				b2FixtureDef   groundFixDef;
						

				groundBox.SetAsBox(width / 2.f, height / 2.f);
				groundFixDef.shape = &groundBox;
						
				if(std::string(object->name).find("block") != std::string::npos)
				{
					groundFixDef.filter.categoryBits = WALL;
					groundFixDef.filter.maskBits = PLAYER | MONSTER | FOOT;
				}
				else
				{
					groundFixDef.filter.categoryBits = TRIGGER;
					groundFixDef.filter.maskBits = PLAYER;
					if(std::string("kill") == object->name)
						groundFixDef.filter.maskBits = PLAYER | MONSTER;
					groundFixDef.isSensor = true;
					groundFixDef.userData = object->name;
				}

				groundBody->CreateFixture(&groundFixDef);

			break;
			}
			case S_POLYGON:
			{
				b2BodyDef groundBodyDef;
				groundBodyDef.position.Set(object->x / Game::pixelpm, object->y / Game::pixelpm);

				b2Body* groundBody = wo.CreateBody(&groundBodyDef);

				b2PolygonShape groundBox;
				b2FixtureDef groundFixDef;

				b2Vec2 *vec = new b2Vec2[object->points_len];
				for(int i = 0; i < object->points_len; ++i)
				{
					vec[i].x = object->points[i][0] / static_cast<float>(Game::pixelpm);
					vec[i].y = object->points[i][1] / static_cast<float>(Game::pixelpm);
				}
						
				groundBox.Set(vec, object->points_len);
				assert(groundBox.Validate());
				groundFixDef.shape = &groundBox;

				if(std::string(object->name).find("block") != std::string::npos)
				{
					groundFixDef.filter.categoryBits = WALL;
					groundFixDef.filter.maskBits = PLAYER | MONSTER | FOOT;
				}
				else
				{
					groundFixDef.filter.categoryBits = TRIGGER;
					groundFixDef.filter.maskBits = PLAYER;
				}
						
				groundBody->CreateFixture(&groundFixDef);
			break;
			}
			default: throw Failure("Square or polygons in tmx to box2d (render_map), im too lazy for the rest");
		}
		object = object->next;
	}
}

Map::Map(const char *filename, b2World& w) : wo(w), playerspawn(NULL)  {
	rsc_img_load_func = al_img_loader;
	rsc_img_free_func = (void (*)(void*))al_destroy_bitmap;

	tmxMap = tmx_load(filename);
	if (!tmxMap)
		throw Failure(tmx_strerr());

	bmp = NULL;
	render_map();
}

Map::~Map() {
	tmx_map_free(tmxMap);
}

void Map::draw(int x_offset, int y_offset, int width, int height) {
	int i, j, start_x, start_y, x_delta, y_delta, x_count, y_count, sx, sy, dx, dy;
	al_clear_to_color(backgroundcolor);

	start_x = x_offset/BMP_MS;
	start_y = y_offset/BMP_MS;

	x_count = std::min<int>((x_offset +  width)/BMP_MS, bmp_x_count-1);
	y_count = std::min<int>((y_offset + height)/BMP_MS, bmp_y_count-1);

	for (i=start_x; i<=x_count; i++) {
		for (j=start_y; j<=y_count; j++) {
			x_delta = x_offset-i*BMP_MS;
			y_delta = y_offset-j*BMP_MS;

			sx = (i==start_x) ? x_delta: 0;
			sy = (j==start_y) ? y_delta: 0;

			dx = (i==start_x) ? 0: i*BMP_MS-x_offset;
			dy = (j==start_y) ? 0: j*BMP_MS-y_offset;

			al_draw_bitmap_region(bmp[j*bmp_x_count+i],
				                  sx,     sy, 
								  BMP_MS, BMP_MS,
								  dx,     dy,           0);
		}
	}
}

Player* Map::playerSpawn(Game &g) const
{
	if(playerspawn == NULL)
		throw Failure("spawn not found in tmx map");

	return new Player(g, b2Vec2(playerspawn->x / static_cast<float>(Game::pixelpm), playerspawn->y / static_cast<float>(Game::pixelpm)));	
}

Monster* Map::monsterSpawn(Game &g) 
{
	if(!monsterspawn.empty())
	{
		tmx_object *m = monsterspawn.back();

		if(m == NULL)
			throw Failure("empty monsterspawn :/");
		monsterspawn.pop_back();
		return new Monster(g, b2Vec2(m->x / static_cast<float>(Game::pixelpm), m->y / static_cast<float>(Game::pixelpm)));
	}
	return NULL;
}
