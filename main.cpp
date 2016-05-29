#include <iostream>
#include <cstdlib>
#include <cstdio>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "map.hpp"
#include "main.hpp"
#include "subgame_env.hpp"
#include "callback.hpp"
#include "menu.hpp"

void event_loop(Callbacks& callbacks, Map& map) {
	SDL_Event event;
	
	while (true) {
		SDL_WaitEvent(&event);
		switch (event.type) {
			case SDL_QUIT:
                		return;
                		break;
                	case SDL_KEYDOWN:
                		callbacks.on_key_down(event.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				callbacks.on_click(event.button, map.get_map_struct().width, map.get_map_struct().height);
				break;
		}
		map.draw();
	}
}

int main(int argc, char* argv[]) {
	/*
	Menu
	*/
	const string subgame = menu::print(argv[1]);
	
	/*
	Load SDL lib
	*/
	
	// SDL_Init / SDL_Quit
	atexit(SDL_Quit);
	if (SDL_Init(SDL_INIT_VIDEO) == -1) {
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}
	
	// Window and icon
	SDL_Window *window = SDL_CreateWindow("Upsilon game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if (window == NULL) {
		fprintf(stderr, "Unable to create window: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}
	SDL_Surface* icon = IMG_Load(ICON_NAME);
	if (icon == NULL) {
		fprintf(stderr, "Unable to load icon: %s\n", IMG_GetError());
		return EXIT_FAILURE;
	}
	SDL_SetWindowIcon(window, icon);
	SDL_FreeSurface(icon);
	
	// Renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE);
	if (renderer == NULL) {
		fprintf(stderr, "Unable to create renderer: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}
	
	// Note: window and renderer are destroyed in the map class destructor.
	
	/*
	Load game
	*/
	Map map(window, subgame); // Load map
	Env* env = map.get_env();
	Callbacks callbacks(env);
	env->load();
	map.draw();
	
	/*
	Events loop
	*/
	event_loop(callbacks, map);
		
	return EXIT_SUCCESS;
}

