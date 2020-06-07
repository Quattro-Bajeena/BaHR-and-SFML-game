#pragma once
//Singleton Random number generator
class RNG
{
private:

	inline static std::random_device rd;
	RNG()
	{};

	
public:
	static RNG& get(){

		static RNG RNGinstance;
		return RNGinstance;
	}
	RNG(const RNG&) = delete;

	
	 inline const int randomI(const int min, const int max) {
		
		static std::mt19937 mt(this->rd());
		std::uniform_int_distribution<> dist(min, max);
		return dist(mt);
	}

	 inline const int discreteI(const std::vector<int> weights) {
		static std::mt19937 mt(this->rd());
		std::discrete_distribution<> dist(weights.begin(), weights.end());
		return dist(mt);

	}
	
	 inline const float randomF(const float min, float max) {

		static std::mt19937 mt(this->rd());
		std::uniform_real_distribution<float> dist(min, max);
		return dist(mt);
	}
	
};

