#pragma once
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

	//Distance bewteen 2 points represeted by Vector2f
	inline const float distance(const sf::Vector2f& pos_1, const sf::Vector2f& pos_2) {
		sf::Vector2f vector = pos_1 - pos_2;
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