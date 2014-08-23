#include <iostream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>

#include "main.hpp"
#include "failure.hpp"

#include "3D/glw.hpp"
#include "3D/types.hpp"
#include "3D/transfo.hpp"
#include "3D/camera.hpp"
#include "3D/shaders.hpp"
#include "3D/geometry.hpp"

using std::cerr;

// Initialize Allegro and its addons
Main::Main(int screen_w, int screen_h) {
	this->screen_h = screen_h;
	this->screen_w = screen_w;

	refresh_tick = 30;
	animation_tick = 60;

	if (!al_init()) throw Failure("failed to initialize allegro!");

	al_set_app_name(APP_NAME);
	al_set_org_name(APP_NAME);

	// Multisampling
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 4, ALLEGRO_SUGGEST); // 4x

	if (display = al_create_display(screen_w, screen_h), !display) throw Failure("failed to create display!");

	if (!al_install_keyboard()) throw Failure("failed to initialize the keyboard!");
	if (!al_install_mouse()) throw Failure("failed to initialize the mouse!");
	//if (!al_install_joystick()) throw Failure("failed to initialize the joystick!");

	if (!(refreshEQ = al_create_event_queue())) throw Failure("failed to create the frame event queue!");
	if (!(animationEQ = al_create_event_queue())) throw Failure("failed to create the animation event queue!");
	if (!(inputEQ = al_create_event_queue())) throw Failure("failed to create the input event queue!");

	if (!(refreshTimer = al_create_timer(1.0/refresh_tick))) throw Failure("failed to initialise the frame timer!");
	if (!(animationTimer = al_create_timer(1.0/animation_tick))) throw Failure("failed to initialise the animation timer!");

	if (!al_init_image_addon()) throw Failure("failed to initialise image support!");

	//if (!al_install_audio()) throw Failure("failed to initialise audio support!");
	//if (!al_init_acodec_addon()) throw Failure("failed to initialise audio codecs support!");

	if (!al_init_primitives_addon()) throw Failure("failed to initialise primitives!");

	al_init_font_addon();
	if (!(console_font = al_create_builtin_font())) throw Failure("failed to initialise console font!");

	// ALLEGRO_EVENT_DISPLAY_*
	al_register_event_source(inputEQ, al_get_display_event_source(display));
	// ALLEGRO_EVENT_JOYSTICK_*
	//al_register_event_source(inputEQ, al_get_joystick_event_source());
	// ALLEGRO_EVENT_KEY_*
	al_register_event_source(inputEQ, al_get_keyboard_event_source());

	// ALLEGRO_EVENT_TIMER frame update
	//al_register_event_source(frameEQ, al_get_timer_event_source(frameTimer));
	// ALLEGRO_EVENT_TIMER animation update
	al_register_event_source(animationEQ, al_get_timer_event_source(animationTimer));
}

// Deinitialize Allegro and its addons
Main::~Main() {
	al_destroy_font(console_font);

	al_shutdown_primitives_addon();
	
	//al_uninstall_audio();

	al_shutdown_image_addon();

	al_destroy_timer(animationTimer);
	al_destroy_timer(frameTimer);

	al_destroy_event_queue(inputEQ);
	al_destroy_event_queue(animationEQ);
	al_destroy_event_queue(refreshEQ);
	
	//al_uninstall_joystick();
	al_uninstall_mouse();
	al_uninstall_keyboard();
	
	al_destroy_display(display);
}

/** OpenGL 3.3+ modern **/
int main(int argc, char *argv[]) {
	try {
		Main m(800, 600);
		ALLEGRO_EVENT ev;
		int step = 0;
		al_start_timer(m.animationTimer);
		
		bool loop = true;
		while (al_wait_for_event(m.animationEQ, &ev), loop) {
			al_clear_to_color(al_map_rgb(0., 0., 0.));
			al_flip_display();
		}
	}
	catch (Failure f) {
		cerr << f.what();
	}
	return 0;
}
