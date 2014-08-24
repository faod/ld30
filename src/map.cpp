#include <allegro5/allegro.h>

#include "map.hpp"
#include "failure.hpp"

#include "Game.hpp"

#include <Box2D/Box2D.h>
#include <cassert>


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
	
	al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_WITH_ALPHA);
	res = al_load_bitmap(al_path_cstr(alpath, ALLEGRO_NATIVE_PATH_SEP));
	
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

void Map::render_map() {
	ALLEGRO_BITMAP *layer_bmp = NULL;
	tmx_layer *layers = tmxMap->ly_head;
	unsigned long w, h;
	
	w = tmxMap->width  * tmxMap->tile_width;  // Bitmap's width and height 
	h = tmxMap->height * tmxMap->tile_height;
	
	while (layers) {
		if (layers->visible && layers->type == L_LAYER) {
			if (!(layer_bmp = al_create_bitmap(w, h))) throw Failure("failed to create bitmap!");
			al_set_target_bitmap(layer_bmp);
			al_clear_to_color(al_map_rgba_f(0., 0., 0., 0.));

			render_layer(tmxMap, layers);
			layers->user_data = layer_bmp;
		}
		else if(layers->type == L_OBJGR)
		{
			tmx_object *object = layers->content.head;
			
			while(object)
			{
				switch(object->shape)
				{	
					case S_SQUARE:
					{
						b2BodyDef groundBodyDef;
						float width = object->width / static_cast<float>(Game::pixelpm);
						float height = object->height / static_cast<float>(Game::pixelpm);

						groundBodyDef.position.Set(object->x / static_cast<float>(Game::pixelpm) + width / 2.f, object->y / static_cast<float>(Game::pixelpm) + height / 2.f);
						
						b2Body* groundBody = wo.CreateBody(&groundBodyDef);

						b2PolygonShape groundBox;

						groundBox.SetAsBox(width / 2.f, height / 2.f);
						groundBody->CreateFixture(&groundBox, 0.0f);

					break;
					}
					case S_POLYGON:
					{
						b2BodyDef groundBodyDef;
						groundBodyDef.position.Set(object->x / Game::pixelpm, object->y / Game::pixelpm);

						b2Body* groundBody = wo.CreateBody(&groundBodyDef);

						b2PolygonShape groundBox;
						
						b2Vec2 *vec = new b2Vec2[object->points_len];
						for(int i = 0; i < object->points_len; ++i)
						{
							vec[i].x = object->points[i][0] / static_cast<float>(Game::pixelpm);
							vec[i].y = object->points[i][1] / static_cast<float>(Game::pixelpm);
						}
						
						groundBox.Set(vec, object->points_len);
						assert(groundBox.Validate());
						groundBody->CreateFixture(&groundBox, 0.0f);
					break;
					}
					default: throw Failure("Square or polygons in tmx to box2d (render_map), im too lazy for the rest");
					

				}

				object = object->next;
			}
		}
		layers = layers->next;
	}
	
	al_set_target_backbuffer(al_get_current_display());
}

Map::Map(const char *filename, b2World& w) : wo(w), playerspawn(NULL)  {
	rsc_img_load_func = al_img_loader;
	rsc_img_free_func = (void (*)(void*))al_destroy_bitmap;

	tmxMap = tmx_load(filename);
	if (!tmxMap)
		throw Failure(tmx_strerr());

	render_map();
}

Map::~Map() {
	tmx_map_free(tmxMap);
}

void Map::draw(int x_offset, int y_offset, int width, int height) {
	tmx_layer *layers = tmxMap->ly_head;

	al_clear_to_color(int_to_al_color(tmxMap->backgroundcolor));
	
	while (layers) {
		if (layers->visible && layers->type == L_LAYER) {
			al_draw_bitmap_region((ALLEGRO_BITMAP*)layers->user_data, x_offset, y_offset, width, height, 0, 0, 0);
		}
		layers = layers->next;
	}
}
