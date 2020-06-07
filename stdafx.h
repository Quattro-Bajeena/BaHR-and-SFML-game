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

//MY headers

//My enums
enum class powerUpType { DEFAULT = 0, FAST, THREE, ROUND, HEAVY, RANDOM };
enum class collectableType{DEFAULT = 0, AMMO , HEART};
enum class gunModels {null = 0, NORMAL, REVOLVER, CANNON, SPREAD, SHOTGUN, MAGIC};
enum class enemyState { FULL = 0, SCRATCHED, DAMAGED, GRAVE, DEAD };
enum class enemyType { REGULAR = 0, COMMANDO, STATIONARY, BOSS };
enum class guiStates { IDLE = 0, HOVER, ACTIVE, RELEASED };
enum TileTypes { DEFAULT = 0, ABOVE, DAMAGING };



#endif // !STDAFX