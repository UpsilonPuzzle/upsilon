#include <cstdio>
#include <cstdlib>
#include <string>

#include "subgame_env.hpp"
#include "util.hpp"
#include "main.hpp"

using namespace std;

static Env* env_obj; // THE env object

Node::Node(string name, string texture_str, SDL_Renderer* renderer) : name(name), texture_str(texture_str), renderer(renderer) {
}

Node::~Node() {
	if (texture != NULL)
		SDL_DestroyTexture(texture);
}

void Node::create_texture(const unsigned long node_width, const unsigned long node_height, const string subgame_name) {
	width = node_width;
	height = node_height;
	
	// Create surface
	Uint8 red = 0, green = 0, blue = 0;
	if (texture_str == "") {
		texture = NULL;
	} else if (rgb_deserialize(texture_str, red, green, blue)) { // RGB ?
		texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
		if (texture == NULL) {
			fprintf(stderr, "Node \"%s\": unable to create RGB texture: %s\n", name.c_str(), IMG_GetError());
			exit(EXIT_FAILURE);
		}
		SDL_SetRenderTarget(renderer, texture);
		SDL_SetRenderDrawColor(renderer, red, green, blue, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		SDL_SetRenderTarget(renderer, NULL);
	} else {
		texture = IMG_LoadTexture(renderer, ("subgames/"+subgame_name+"/"+texture_str).c_str());
		if (texture == NULL) {
			fprintf(stderr, "Node \"%s\": unable to load texture \"%s\": %s\n", name.c_str(), texture_str.c_str(), IMG_GetError());
			exit(EXIT_FAILURE);
		}
	}
}

void Node::draw(const int x, const int y) {
	SDL_Rect rect;
	rect.x = x*width; rect.y = y*height;
	rect.w = width; rect.h = height;
	if (texture != NULL)
		SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void register_node(string name, luwra::Table nodedef) {
	string texture = "";
	if (nodedef.template has<const char*>("texture"))
		texture = nodedef.template get<const char*>("texture");
	Node* node = new Node(name, texture, env_obj->renderer);
	env_obj->registered_nodes.push_back(node);
}

Env::Env(SDL_Window* sdl_window, int displayed_x, int displayed_y, const string subgame_name) : subgame_name(subgame_name) {
	env_obj = this;
	
	window = sdl_window;
	renderer = SDL_GetRenderer(window);
	
	width = WINDOW_WIDTH/displayed_x;
	height = WINDOW_WIDTH/displayed_y;
	
	state.loadStandardLibrary();
	
	state[UPSILON_TABLE] = luwra::FieldVector {};
}

Env::~Env() {
	for (int i = 0 ; i < registered_nodes.size() ; i++) {
		delete registered_nodes[i];
	}
}

void Env::add_function(string name, lua_CFunction function) {
	state[UPSILON_TABLE][name] = function;
}

void Env::load() {
	// Run init.lua file
	add_function("RGB", LUWRA_WRAP(rgb_serialize));
	add_function("register_node", LUWRA_WRAP(register_node));
	
	if (state.runFile("subgames/"+subgame_name+"/"+INIT_FILE) != 0) {
		fprintf(stderr, "Error: Unable to load file %s.\n", ("subgames/"+subgame_name+"/"+INIT_FILE).c_str());
		exit(EXIT_FAILURE);
	}
	
	// Set the window title
	if (state.has("title"))
		SDL_SetWindowTitle(window, ("Upsilon ("+state.template get<string>("title")+")").c_str());
	
	// Create nodes textures	
	for (int i = 0 ; i < registered_nodes.size() ; i++) {
		registered_nodes[i]->create_texture(width, height, subgame_name);
	}
}

void Env::draw_node(const string name, const int x, const int y) {
	for (int i = 0 ; i < registered_nodes.size() ; i++) {
		if (registered_nodes[i]->get_name() == name) {
			registered_nodes[i]->draw(x, y);
			return;
		}
	}
}

