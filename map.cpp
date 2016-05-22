#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>

#include <SDL2/SDL_image.h>

#include "map.hpp"
#include "util.hpp"
#include "main.hpp"

using namespace std;

static Map* map_obj; // THE map object

Map::Map(SDL_Window* window, const string subgame_name, int number) : window(window), subgame_name(subgame_name), number(number) {
	map_obj = this;
	
	renderer = SDL_GetRenderer(window);
	
	load_map(number);
}

Map::~Map() {
	SDL_DestroyTexture(background_texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	delete env;
}

void Map::load_map(int new_num) {
	number = new_num;
	
	// Run lua file
	const string map_directory = "subgames/"+subgame_name+"/map/";
	string filename = map_directory+"map"+to_string(number)+".lua";
	state.loadStandardLibrary();
	state["RGB"] = LUWRA_WRAP(rgb_serialize);
	state["level"] = number;
	state["map"] = luwra::FieldVector {};
	if (state.runFile(map_directory+"map.lua") != 0)
		fprintf(stderr, "Warning: Unable to open %s file.\n", (map_directory+"map.lua").c_str());
	
	if (state.runFile(filename) != 0) {
		if (state.runFile(map_directory+"map1.lua") != 0) {
			fprintf(stderr, "Error: Unable to open map file. Make sure at least the file map1.lua is in the %s directory.\n", map_directory.c_str());
			exit(EXIT_FAILURE);
		} else {
			number = 1;
			printf("Info: Using file map1.lua as map file, because there is no %s file.\n",  filename.c_str());
		}
	}
	
	if (!state.has("map")) {
		fprintf(stderr, "Error: Unable to get the \"map\" global variable in Lua file %s.\n", filename.c_str());
		exit(EXIT_FAILURE);
	}
	
	luwra::Table map_table = state["map"];
	
	/// Map_struct
	
	// pos_x and pos_y
	if (map_table.has("pos_x"))
		map_struct.pos_x = map_table["pos_x"];
	else
		map_struct.pos_x = 0;
	
	if (map_table.has("pos_y"))
		map_struct.pos_y = map_table["pos_y"];
	else
		map_struct.pos_y = 0;
	
	// Nodes, width and height
	if (!map_table.has("nodes")) {
		fprintf(stderr, "Error: Unable to get map[nodes] table in Lua file %s.\n", filename.c_str());
		exit(EXIT_FAILURE);
	}
	luwra::Table nodes_table = map_table["nodes"];
	
	// Note: the Lua nodes table has form nodes[y][x] ; but I want the C++ nodes vector has form nodes[x][y].
	for (map_struct.height = 0 ; nodes_table.has(map_struct.height+1) ; map_struct.height++);
	if (map_struct.height == 0) {
		fprintf(stderr, "Error: %s map height is 0\n", filename.c_str());
		exit(EXIT_FAILURE);
	}
	for (map_struct.width = 0 ; nodes_table.template get<luwra::Table>(1).has(map_struct.width+1) ; map_struct.width++);
	if (map_struct.width == 0) {
		fprintf(stderr, "Error: %s map width is 0\n", filename.c_str());
		exit(EXIT_FAILURE);
	}
	
	nodes.resize(map_struct.width);
	for (int i = 0 ; i < map_struct.width ; i++) {
		nodes[i].resize(map_struct.height);
		
		for (int j = 0 ; j < map_struct.height ; j++) {
			nodes[i][j] = nodes_table.template get<luwra::Table>(j+1).template get<string>(i+1);
		}
	}
	
	// displayed_x and displayed_y
	if (map_table.has("displayed_x"))
		map_struct.displayed_x = map_table["displayed_x"];
	else
		map_struct.displayed_x = map_struct.width;
	
	if (map_table.has("displayed_y"))
		map_struct.displayed_y = map_table["displayed_y"];
	else
		map_struct.displayed_y = map_struct.height;
	
	
	/// Background
	string background_str;
	if (map_table.has("background"))
		background_str = map_table.get<string>("background");
	else
		background_str = "_rgb255;255;255";
	
	Uint8 red = 0, green = 0, blue = 0;
	if (rgb_deserialize(background_str, red, green, blue)) { // RGB ?
		background_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,  SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
		SDL_SetRenderTarget(renderer, background_texture);
		SDL_SetRenderDrawColor(renderer, red, green, blue, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		SDL_SetRenderTarget(renderer, NULL);
	} else {
		background_texture = IMG_LoadTexture(renderer, (map_directory+background_str).c_str());
		if (background_texture == NULL) {
			fprintf(stderr, "Unable to load texture \"%s\": %s\n", background_str.c_str(), IMG_GetError());
			exit(EXIT_FAILURE);
		}
	}
}

Env* Map::load_env() {
	env = new Env(window, map_struct.displayed_x, map_struct.displayed_y, subgame_name);
	ADD_FUNCTION(move_map_right);
	ADD_FUNCTION(move_map_left);
	ADD_FUNCTION(move_map_up);
	ADD_FUNCTION(move_map_down);
	ADD_FUNCTION(move_map_to);
	ADD_FUNCTION(move_node);
	ADD_FUNCTION(add_node);
	ADD_FUNCTION(remove_node);
	ADD_FUNCTION(set_node);
	ADD_FUNCTION(get_node);
	ADD_FUNCTION(get_node_pos);
	ADD_FUNCTION(get_node_screen_pos);
	ADD_FUNCTION(next_map);
	return env;
}

void Map::draw() {
	SDL_RenderCopy(renderer, background_texture, NULL, NULL);
	for (int x = 0 ; x < map_struct.displayed_x ; x++) {
		for (int y = 0 ; y < map_struct.displayed_y ; y++) {
			if (nodes[x+map_struct.pos_x][y+map_struct.pos_y] != "none")
				env->draw_node(nodes[x+map_struct.pos_x][y+map_struct.pos_y], x, y);
		}
	}
	SDL_RenderPresent(renderer);
}

bool move_map_right() {
	if (map_obj->map_struct.pos_x <= 0)
		return false;
	map_obj->map_struct.pos_x--;
	return true;
}
bool move_map_left() {
	if (map_obj->map_struct.pos_x+map_obj->map_struct.displayed_x >= map_obj->map_struct.width)
		return false;
	map_obj->map_struct.pos_x++;
	return true;
}
bool move_map_up() {
	if (map_obj->map_struct.pos_y <= 0)
		return false;
	map_obj->map_struct.pos_y--;
	return true;
}
bool move_map_down() {
	if (map_obj->map_struct.pos_y+map_obj->map_struct.displayed_y >= map_obj->map_struct.height)
		return false;
	map_obj->map_struct.pos_y++;
	return true;
}
bool move_map_to(int x, int y) {
	if (x < 0 || x+map_obj->map_struct.displayed_x > map_obj->map_struct.width || y < 0 || y+map_obj->map_struct.displayed_y > map_obj->map_struct.height)
		return false;
	map_obj->map_struct.pos_x = x;
	map_obj->map_struct.pos_y = y;
	return true;
}

bool check_pos(int x, int y) {
	if (x >= map_obj->map_struct.width || x < 0 || y >= map_obj->map_struct.height || y < 0)
		return false;
	return true;
}
bool move_node(int pos_x, int pos_y, int move_x, int move_y) {
	if (!check_pos(pos_y, pos_y) || !check_pos(pos_x+move_x, pos_y+move_y))
		return false;
	
	map_obj->nodes[pos_x+move_x][pos_y+move_y] = map_obj->nodes[pos_x][pos_y];
	map_obj->nodes[pos_x][pos_y] = "none";
	return true;
}
bool add_node(int x, int y, string name) {
	if (!check_pos(x, y))
		return false;
	if (map_obj->nodes[x][y] != "none")
		return false;
	
	map_obj->nodes[x][y] = name;
	return true;
}
bool remove_node(int x, int y) {
	if (!check_pos(x, y))
		return false;
	
	map_obj->nodes[x][y] = "none";
	return true;
}
bool set_node(int x, int y, string new_name) {
	if (!check_pos(x, y))
		return false;
	
	map_obj->nodes[x][y] = new_name;
	return true;
}

string get_node(int x, int y) {
	if (!check_pos(x, y))
		return "";
	return map_obj->nodes[x][y];
}

luwra::FieldVector get_node_pos(string name) {
	luwra::FieldVector field_vector;
	for (int x = 0 ; x < map_obj->nodes.size() ; x++) {
		for (int y = 0 ; y < map_obj->nodes[x].size() ; y++) {
			if (map_obj->nodes[x][y] == name)
				field_vector.push_back(std::pair<int, luwra::FieldVector> {field_vector.size(), {{"x", x}, {"y", y}}});
		}
	}
	return field_vector;
}
luwra::FieldVector get_node_screen_pos(string name) {
	luwra::FieldVector field_vector;
	for (int x = map_obj->map_struct.pos_x ; x < map_obj->map_struct.width+map_obj->map_struct.pos_x ; x++) {
		for (int y = map_obj->map_struct.pos_y ; y < map_obj->map_struct.height+map_obj->map_struct.pos_y ; y++) {
			if (map_obj->nodes[x][y] == name)
				field_vector.push_back(std::pair<int, luwra::FieldVector> {field_vector.size(), {{"x", x}, {"y", y}}});
		}
	}
	return field_vector;
}

int next_map() {
	map_obj->load_map(map_obj->number+1);
	return map_obj->number;
}

