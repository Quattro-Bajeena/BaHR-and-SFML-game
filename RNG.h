#pragma once
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

	
	int randomI(int min, int max) {
		
		static std::mt19937 mt(this->rd());
		std::uniform_int_distribution<> distribution(min, max);
		return distribution(mt);
	}
	
	float randomF(float min, float max) {

		static std::mt19937 mt(this->rd());
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(mt);
	}
	
};

