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

namespace Env {

	class Node {
		public:
		Node(std::string name, std::string texture_str, SDL_Renderer* renderer); // Constructor (node name, texture string)
		~Node(); // Destructor
		void create_texture(const unsigned long width, const unsigned long height, const std::string subgame_name); // Create the node surface using given width and height (in nodes)
		void draw(const int x, const int y); // Draw the node at given position (in nodes number)
		std::string get_name() { return name; } // Return the node name
	
		private:
		std::string name; // The node name
		std::string texture_str; // The node texture string
		SDL_Texture* texture; // The node SDL texture
		SDL_Renderer* renderer;
	
		unsigned long width; // The node width in pixels
		unsigned long height; // The node height in pixels
	};

	void register_node(std::string name, luwra::Table nodedef);
	void register_at_exit(luwra::NativeFunction<void> lua_function);

	class Env {
		public:
		Env(SDL_Window* window, const std::string subgame_name); // Constructor
		~Env(); // Destructor
		void add_function(std::string name, lua_CFunction function); // Add a function to environment
		void load();
		void draw_node(const std::string name, const int x, const int y); // Draw a node at given position (in nodes number)
		void update_dim(const int displayed_x, const int displayed_y);
	
		private:
		const std::string subgame_name;
		std::vector<Node*> registered_nodes; // Registered nodes
		SDL_Window* window;
		SDL_Renderer* renderer;
		luwra::StateWrapper state; // The luwra state wrapper
		int width, height; // The node width and height in pixels
	
		std::vector<std::function<void(void)>* > functions_at_exit;
	
	
		friend void register_node(std::string name, luwra::Table nodedef);
		friend void register_at_exit(luwra::NativeFunction<void> function);
	};
}

