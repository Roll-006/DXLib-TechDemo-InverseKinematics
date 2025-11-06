#pragma once
#include <random>

#include "../Base/singleton_base.hpp"
#include "../Concept/common_concepts.hpp"

class RandomGenerator final : public SingletonBase<RandomGenerator>
{
public:
	/// @brief —”‚ğæ“¾(min <= num < max)
	/// @param min Å¬’l(”ÍˆÍ‚ÉŠÜ‚Ş)
	/// @param max Å‘å’l(”ÍˆÍ‚ÉŠÜ‚Ü‚È‚¢)
	template<common_concepts::ArithmeticT ArithmeticT>
	[[nodiscard]] ArithmeticT GetRandClosedOpen(const ArithmeticT min, const ArithmeticT max)
	{
		// ®”Œ^‚Å‚ ‚éê‡
		if constexpr (std::is_integral_v<ArithmeticT>)
		{
			std::uniform_int_distribution<ArithmeticT>  rand(min, max - 1);
			return rand(m_rand_generator);
		}
		// •‚“®¬”“_”Œ^‚Å‚ ‚éê‡
		if constexpr (std::is_floating_point_v<ArithmeticT>)
		{
			std::uniform_real_distribution<ArithmeticT> rand(min, max);
			return rand(m_rand_generator);
		}
		return 0;
	}

	/// @brief —”‚ğæ“¾(min < num <= max)
	/// @param min Å¬’l(”ÍˆÍ‚ÉŠÜ‚Ü‚È‚¢)
	/// @param max Å‘å’l(”ÍˆÍ‚ÉŠÜ‚Ş)
	template<common_concepts::ArithmeticT ArithmeticT>
	[[nodiscard]] ArithmeticT GetRandOpenClosed(const ArithmeticT min, const ArithmeticT max)
	{
		// ®”Œ^‚Å‚ ‚éê‡
		if constexpr (std::is_integral_v<ArithmeticT>)
		{
			std::uniform_int_distribution<ArithmeticT>  rand(min + 1, max);
			return rand(m_rand_generator);
		}
		// •‚“®¬”“_”Œ^‚Å‚ ‚éê‡
		if constexpr (std::is_floating_point_v<ArithmeticT>)
		{
			std::uniform_real_distribution<ArithmeticT> rand(-max, -min);
			return -rand(m_rand_generator);
		}
		return 0;
	}

private:
	RandomGenerator();
	~RandomGenerator();

private:
	std::random_device m_seed;
	std::mt19937 m_rand_generator;

	friend SingletonBase<RandomGenerator>;
};
