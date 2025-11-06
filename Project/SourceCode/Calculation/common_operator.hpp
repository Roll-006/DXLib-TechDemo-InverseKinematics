#pragma once
#include <unordered_map>

template<typename FirstT, typename SecondT>
inline bool operator==(const std::pair<FirstT, SecondT> pair1, const std::pair<FirstT, SecondT> pair2)
{
	return pair1.first == pair2.first && pair1.second == pair2.second;
}

template<typename FirstT, typename SecondT>
inline bool operator!=(const std::pair<FirstT, SecondT> pair1, const std::pair<FirstT, SecondT> pair2)
{
	return !(pair1 == pair2);
}
