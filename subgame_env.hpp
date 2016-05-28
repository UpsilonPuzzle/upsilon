#pragma once

#include <vector>
#include <string>
#include <functional>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "luwra.hpp"
#include <lua.h>

#define INIT_FILE "init.lua"
#define UPSILON_TABLE "upsilon"

using namespace std;

class Node {
	public:
	Node(string name, string texture_str, SDL_Renderer* renderer); // Constructor (node name, texture string)
	~Node(); // Destructor
	void create_texture(const unsigned long width, const unsigned long height, const string subgame_name); // Create the node surface using given width and height (in nodes)
	void draw(const int x, const int y); // Draw the node at given position (in nodes number)
	string get_name() { return name; } // Return the node name
	
	private:
	string name; // The node name
	string texture_str; // The node texture string
	SDL_Texture* texture; // The node SDL texture
	SDL_Renderer* renderer;
	
	unsigned long width; // The node width in pixels
	unsigned long height; // The node height in pixels
};

void register_node(string name, luwra::Table nodedef);
void register_at_exit(luwra::NativeFunction<void> lua_function);

class Env {
	public:
	Env(SDL_Window* window, int displayed_x, int displayed_y, const string subgame_name); // Constructor
	~Env(); // Destructor
	void add_function(string name, lua_CFunction function); // Add a function to environment
	void load();
	void draw_node(const string name, const int x, const int y); // Draw a node at given position (in nodes number)
	
	private:
	const string subgame_name;
	vector<Node*> registered_nodes; // Registered nodes
	SDL_Window* window;
	SDL_Renderer* renderer;
	luwra::StateWrapper state; // The luwra state wrapper
	int width, height; // The node width and height in pixels
	
	vector<function<void(void)>* > functions_at_exit;
	
	
	friend void register_node(string name, luwra::Table nodedef);
	friend void register_at_exit(luwra::NativeFunction<void> function);
};

