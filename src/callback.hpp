#include <vector>
#include <functional>

#include <SDL2/SDL.h>

#include "subgame_env.hpp"
#include "luwra.hpp"

using namespace std;


void register_on_up(luwra::NativeFunction<void> function);
void register_on_down(luwra::NativeFunction<void> function);
void register_on_right(luwra::NativeFunction<void> function);
void register_on_left(luwra::NativeFunction<void> function);
void register_on_left_click(luwra::NativeFunction<void> function);
void register_on_right_click(luwra::NativeFunction<void> function);


class Callbacks {
	public:
	Callbacks(Env* env);
	~Callbacks();
	void on_key_down(SDL_Keycode key);
	void on_click(SDL_MouseButtonEvent button, int width, int height);
	
	private:
	vector< function<void(void)>* > on_up;
	vector< function<void(void)>* > on_down;
	vector< function<void(void)>* > on_right;
	vector< function<void(void)>* > on_left;
	vector< function<void(int, int)>* > on_left_click;
	vector< function<void(int, int)>* > on_right_click;
	
	friend void register_on_up(luwra::NativeFunction<void> lua_function);
	friend void register_on_down(luwra::NativeFunction<void> function);
	friend void register_on_right(luwra::NativeFunction<void> function);
	friend void register_on_left(luwra::NativeFunction<void> function);
	friend void register_on_left_click(luwra::NativeFunction<void> function);
	friend void register_on_right_click(luwra::NativeFunction<void> function);
};

