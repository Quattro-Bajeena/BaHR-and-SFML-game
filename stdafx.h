#pragma once
#ifndef STDAFX_H
#define STDAFX_H

#define _USE_MATH_DEFINES

//Core
#include <algorithm>
#include <deque>
#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <random>
#include <memory>
#include <limits>
//#include <set>
//#include <thread>
#include <utility>
//#include <list>

//SFML
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>

//My enums
enum class powerUpType { DEFAULT = 0, FAST, THREE, ROUND, HEAVY, RANDOM };
enum class ammoType {};
enum class gunModels {null = 0, NORMAL, REVOLVER, CANNON, SPREAD, SHOTGUN, MAGIC};
enum class enemyState { FULL = 0, SCRATCHED, DAMAGED, GRAVE, DEAD };
enum class enemyType { REGULAR = 0, COMMANDO, STATIONARY, BOSS };
enum class guiStates { IDLE = 0, HOVER, ACTIVE, RELEASED };
enum TileTypes { DEFAULT = 0, ABOVE, DAMAGING };

namespace util {
	//My Global Functions

//Deletes a vector of pointers
	template<class T> void purge(std::vector<T>& v) {
		for (auto item : v) delete item;
		v.clear();
	}

	//Clipping values within bounds
	template <typename T>
	T clamp(const T& n, const T& lower, const T& upper) {
		return std::max(lower, std::min(n, upper));
	}

	//For chcecking the sign (direction) of velocity
	template <typename T> int sgn(T val) {
		return (T(0) < val) - (val < T(0));
	}

	//Normalizes Vector2f
	inline sf::Vector2f normalize(const sf::Vector2f& vector) {
		return vector / sqrt(pow(vector.x, 2) + pow(vector.y, 2));
	}

	inline float vectorLength(const sf::Vector2f& vector) {
		return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
	}

	//rotates vector2f around 0,0 and normalizes it
	inline sf::Vector2f rotateVector(const sf::Vector2f& org, const float& rotation)
	{
		sf::Vector2f rotated;
		double rad = (M_PI / 180) * rotation;
		rotated.x = cos(rad) * org.x - sin(rad) * org.y;
		rotated.y = sin(rad) * org.x + cos(rad) * org.y;
		//Algebra is back in town....Rotator Givensa bejbeeeee!!!!!
		return normalize(rotated);
	}

	//rotates vector2f around a point
	inline sf::Vector2f rotateVector(const sf::Vector2f& org, const sf::Vector2f& point, const float& rotation)
	{
		sf::Vector2f rotated;

		double rad = (M_PI / 180) * rotation;
		rotated.x = cos(rad) * (org.x - point.x) - sin(rad) * (org.y - point.y);
		rotated.y = sin(rad) * (org.x - point.x) + cos(rad) * (org.y - point.y);
		//Algebra is back in town....Rotator Givensa bejbeeeee!!!!!
		rotated.x += point.x;
		rotated.y += point.y;
		return rotated;
	}
}


#endif // !STDAFX