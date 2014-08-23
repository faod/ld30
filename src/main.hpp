/*
    Allegro C++ CodeBase
    Copyright (C) 2014 Bayle Jonathan

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
	Main header.
**/
#pragma once
#ifndef MAIN_H
#define MAIN_H

#include <cstdint>

using std::int32_t;

#define APP_NAME "Allegro Game"

/** Creates an allegro context **/
class Main {
public:
	unsigned int screen_h, screen_w;
	unsigned int refresh_tick, animation_tick;
	int32_t loaded_mods;

	ALLEGRO_DISPLAY *display;
	ALLEGRO_EVENT_QUEUE *refreshEQ, *animationEQ, *inputEQ;
	ALLEGRO_TIMER *refreshTimer, *animationTimer;
	ALLEGRO_FONT *console_font;

	/* Constructor may throw a `Failure` exception. */
	Main(int screen_w, int screen_h);
	~Main();
};

#endif  /* MAIN_H */