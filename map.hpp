#pragma once

#include <vector>
#include <string>

#include <SDL2/SDL.h>

#include "luwra.hpp"
#include "subgame_env.hpp"

#define ADD_FUNCTION(n) env->add_function(#n, LUWRA_WRAP(n))

using namespace std;

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
bool add_node(int x, int y, string name);
bool remove_node(int x, int y);
bool set_node(int x, int y, string new_name);

string get_node(int x, int y);

luwra::FieldVector get_node_pos(string name);
luwra::FieldVector get_node_screen_pos(string name);

int next_map();

class Map {
	public:
	Map(SDL_Window* window, const string subgame_name, int number = 1); // Constructor (SDL window, subgame name, map number)
	~Map(); // Destructor
	void load_map(int new_num);
	Env* load_env();
	void draw(); // Draw the nodes
	
	private:
	const string subgame_name;
	Map_struct map_struct;
	Env* env; // The subgame environment pointer
	vector< vector< string > > nodes; // The nodes vector (form nodes[x][y])
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
	friend bool add_node(int x, int y, string name);
	friend bool remove_node(int x, int y);
	friend bool set_node(int x, int y, string new_name);
	friend string get_node(int x, int y);
	friend luwra::FieldVector get_node_pos(string name);
	friend luwra::FieldVector get_node_screen_pos(string name);
	friend int next_map();
};

