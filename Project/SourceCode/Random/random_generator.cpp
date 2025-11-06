#include "random_generator.hpp"

RandomGenerator::RandomGenerator() : 
	m_seed(),
	m_rand_generator(m_seed())
{
	// ˆ—‚È‚µ
}

RandomGenerator::~RandomGenerator()
{
	// ˆ—‚È‚µ
}
