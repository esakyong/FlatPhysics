#pragma once
#include <random>
#include "raylib.h"
class RandomHelper
{
private:
	static std::mt19937 MakeGen() {
		std::random_device rd;
		return std::mt19937(rd());
	}
	inline static std::mt19937 gen = MakeGen();
	
public:

	
	static int RandomInteger(int min, int max) {
		std::uniform_int_distribution<int> dis(min, max);
		return dis(gen);
	}
	static float RandomFloat(float min, float max) {
		std::uniform_real_distribution<> dist(min, max);
		return dist(gen);
	}
	static Color RandomColor() {
		std::uniform_int_distribution<int> dis(0, 255);
		
		return { unsigned char(dis(gen)), unsigned char(dis(gen)), unsigned char(dis(gen)), 255 };
	}
};

