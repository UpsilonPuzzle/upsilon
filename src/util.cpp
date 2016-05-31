#include <sstream>

#include "util.hpp"

using namespace std;

string rgb_serialize(const unsigned char red, const unsigned char green, const unsigned char blue) {
	ostringstream format;
	format << "_rgb" << (int)red << ';' << (int)green << ';' << (int)blue; // format: _rgb(red);(green);(blue)
	return format.str();
}

bool rgb_deserialize(const string& serialized_string, unsigned char& red, unsigned char& green, unsigned char& blue) {
	if (serialized_string.substr(0,4) == "_rgb") {
		red = stoi(serialized_string.substr(4));
		green = stoi(serialized_string.substr(serialized_string.find(';')+1));
		blue = stoi(serialized_string.substr(serialized_string.rfind(';')+1));
		return true;
	} else
		return false;
}
