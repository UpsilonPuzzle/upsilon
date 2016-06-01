#pragma once

#include <vector>
#include <string>
#include <map>

#include <SDL2/SDL.h>

#include "luwra.hpp"
#include "subgame_env.hpp"

#define ADD_FUNCTION(n) env->add_function(#n, LUWRA_WRAP(n))

namespace Map {

	struct Map_struct {
		int width; // The map width
		int height; // The map height
		int displayed_x; // The width of displayed map
		int displayed_y; // The height of displayed map
		int pos_x, pos_y; // The position on the displayed map (up right corner)
	};

	bool move_map_right();
	bool move_map_left();
	bool move_map_up();
	bool move_map_down();
	bool move_map_to(int x, int y);

	bool check_pos(int x, int y);
	bool move_node(int pos_x, int pos_y, int move_x, int move_y);
	bool add_node(int x, int y, std::string name);
	bool remove_node(int x, int y);
	bool set_node(int x, int y, std::string new_name);

	std::string get_node(int x, int y);

	std::map<int, std::map<std::string, int> > get_node_pos(std::string name);
	std::map<int, std::map<std::string, int> > get_node_screen_pos(std::string name);
	
	int get_map_width();
	int get_map_height();
	int get_map_displayed_x();
	int get_map_displayed_y();

	int next_map();

	class Map {
		public:
		Map(SDL_Window* window, const std::string subgame_name, int number = 1); // Constructor (SDL window, subgame name, map number)
		~Map(); // Destructor
		Map_struct get_map_struct();
		void load_map(int new_num);
		Env::Env* get_env();
		void draw(); // Draw the nodes
	
		private:
		void load_env();
	
		const std::string subgame_name;
		Map_struct map_struct;
		Env::Env* env; // The subgame environment pointer
		std::vector< std::vector< std::string > > nodes; // The nodes vector (form nodes[x][y])
		luwra::StateWrapper state; // The luwra state wrapper
		SDL_Renderer* renderer; // The SDL renderer
		SDL_Texture* background_texture; // The background texture
		SDL_Window* window; // The SDL window
		int number;
	
	
		friend bool move_map_right();
		friend bool move_map_left();
		friend bool move_map_up();
		friend bool move_map_down();
		friend bool move_map_to(int x, int y);
		friend bool check_pos(int x, int y);
		friend bool move_node(int pos_x, int pos_y, int move_x, int move_y);
		friend bool add_node(int x, int y, std::string name);
		friend bool remove_node(int x, int y);
		friend bool set_node(int x, int y,std:: string new_name);
		friend std::string get_node(int x, int y);
		friend std::map<int, std::map<std::string, int> > get_node_pos(std::string name);
		friend std::map<int, std::map<std::string, int> > get_node_screen_pos(std::string name);
		friend int get_map_width();
		friend int get_map_height();
		friend int get_map_displayed_x();
		friend int get_map_displayed_y();
		friend int next_map();
	};
}

