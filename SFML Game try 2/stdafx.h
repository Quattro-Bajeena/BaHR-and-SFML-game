#ifndef STDAFX_H
#define STDAFX_H
#define NOMINMAX

//Core
#include <algorithm>
#include<deque>
#include <iostream>
#include<map>
#include<memory>
#include<set>
#include<thread>
#include<utility>
#include<vector>
#include<list>
#include<stack>
#include<string>
#include<sstream>
#include<fstream>
#include<cmath>

//SFML
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>

//My templates

//Clipping values within bounds
template <typename T>
T clamp(const T& n, const T& lower, const T& upper) {
    return std::max(lower, std::min(n, upper));
}

//For chcecking the sign (direction) of velocity
template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

#endif // !STDAFX