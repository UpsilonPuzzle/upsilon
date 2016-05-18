#include "callback.hpp"

using namespace std;

static Callbacks* callbacks_obj;

void register_on_up(luwra::NativeFunction<void> lua_function) {
	callbacks_obj->on_up.push_back(new function<void(void)>(lua_function));
}
void register_on_down(luwra::NativeFunction<void> lua_function) {
	callbacks_obj->on_down.push_back(new function<void(void)>(lua_function));
}
void register_on_right(luwra::NativeFunction<void> lua_function) {
	callbacks_obj->on_right.push_back(new function<void(void)>(lua_function));
}
void register_on_left(luwra::NativeFunction<void> lua_function) {
	callbacks_obj->on_left.push_back(new function<void(void)>(lua_function));
}

Callbacks::Callbacks(Env* env) {
	callbacks_obj = this;
	
	env->add_function("register_on_up", LUWRA_WRAP(register_on_up));
	env->add_function("register_on_down", LUWRA_WRAP(register_on_down));
	env->add_function("register_on_right", LUWRA_WRAP(register_on_right));
	env->add_function("register_on_left", LUWRA_WRAP(register_on_left));
}

Callbacks::~Callbacks() {
	for (int i = 0 ; i < on_up.size() ; i++)
		delete on_up[i];
	for (int i = 0 ; i < on_down.size() ; i++)
		delete on_down[i];
	for (int i = 0 ; i < on_right.size() ; i++)
		delete on_right[i];
	for (int i = 0 ; i < on_up.size() ; i++)
		delete on_left[i];
}

void Callbacks::on_key_down(SDL_Keycode key) {
	switch (key) {
		case SDLK_UP:
			for (int i = 0 ; i < on_up.size() ; i++)
				(*on_up[i])();
			break;
		case SDLK_DOWN:
			for (int i = 0 ; i < on_down.size() ; i++)
				(*on_down[i])();
			break;
		case SDLK_RIGHT:
			for (int i = 0 ; i < on_right.size() ; i++)
				(*on_right[i])();
			break;
		case SDLK_LEFT:
			for (int i = 0 ; i < on_left.size() ; i++)
				(*on_left[i])();
			break;
	}
}

