#include <string>

using namespace std;

string rgb_serialize(const unsigned char red, const unsigned char green, const unsigned char blue); // Serialize RGB value into string (useful for texture strings)
bool rgb_deserialize(const string& serialized_string, unsigned char& red, unsigned char& green, unsigned char& blue); // Deserialize string into RGB value, returns true if the string is deserializable
