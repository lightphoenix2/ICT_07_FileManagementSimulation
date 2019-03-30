#include"Functions.h"
#include <algorithm> 
#include <cctype>
#include <locale>
using namespace std;

// trim from start (in place)
void Functions::ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
void Functions::rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

// trim from both ends (in place)
void Functions::trim(std::string &s) {
	ltrim(s);
	rtrim(s);
}
