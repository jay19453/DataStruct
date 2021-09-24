#pragma once
#include <cstdint>
#include <random>

namespace wnet {
	class RangeRandom {
	public:
		RangeRandom(int32_t min,int32_t max);
		~RangeRandom();

		int32_t Random();
	private:
		static std::random_device _random;
		static std::mt19937 _engine;
		std::uniform_int_distribution<int32_t> _uniform;
	};
}