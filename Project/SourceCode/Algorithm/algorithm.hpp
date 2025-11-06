#pragma once
#include <algorithm>
#include <unordered_map>

#include "../Kind/sort_kind.hpp"

namespace algorithm
{
	#pragma region 変換
	/// @brief std::vector<std::pair<key, value>>をstd::unorderd_map<key, value>へ変換
	/// @brief WARNING : 順番は保たれない
	template<typename KeyT, typename ValueT>
	[[nodiscard]] std::unordered_map<KeyT, ValueT> ConvertPairsToUmap(const std::vector<std::pair<KeyT, ValueT>>& pairs)
	{
		std::unordered_map<KeyT, ValueT> u_map(pairs.begin(), pairs.end());
		return u_map;
	}

	/// @brief std::unorderd_map<key, value>をstd::vector<std::pair<first, second>>へ変換
	/// @brief WARNING : 順番は保たれない
	template<typename KeyT, typename ValueT>
	[[nodiscard]] std::vector<std::pair<KeyT, ValueT>> ConvertUmapToPairs(const std::unordered_map<KeyT, ValueT>& u_map)
	{
		std::vector<std::pair<KeyT, ValueT>> pairs(u_map.begin(), u_map.end());
		return pairs;
	}
	#pragma endregion


	#pragma region ソート
	/// @brief std::vector<std::pair<first, second>>のsecondをソート
	template<typename FirstT, typename SecondT>
	[[nodiscard]] std::vector<std::pair<FirstT, SecondT>> Sort(const std::vector<std::pair<FirstT, SecondT>>& pairs, const SortKind sort_kind)
	{
		auto sorted_pairs = pairs;

		std::sort(sorted_pairs.begin(), sorted_pairs.end(), [=](const auto& a, const auto& b)
		{
			switch (sort_kind)
			{
			case SortKind::kAscending:	return a.second < b.second;
			case SortKind::kDescending:	return a.second > b.second;
			}
			return false;
		});

		return sorted_pairs;
	}

	/// @brief std::vector<std::pair<first, second>>のsecondをソート
	/// @brief WARNING : main_pairsとsub_pairsは同じデータ型、同じ個数である必要あり
	/// @brief WARNING : main_pairsとsub_pairsのfirstの並び順は一致している必要あり
	/// @param main_pairs ソートされるpairs
	/// @param sub_pairs main_pairsに同じsecondがあった場合、sub_pairsでソートを行う
	template<typename FirstT, typename SecondT>
	[[nodiscard]] std::vector<std::pair<FirstT, SecondT>> Sort(
		const std::vector<std::pair<FirstT, SecondT>>& main_pairs, 
		const std::vector<std::pair<FirstT, SecondT>>& sub_pairs, 
		const SortKind main_pairs_sort_kind, 
		const SortKind sub_pairs_sort_kind)
	{
		auto sorted_pairs = main_pairs;

		// 元のインデックスを記録
		std::unordered_map<FirstT, size_t> index_map;
		for (size_t i = 0; i < main_pairs.size(); ++i)
		{
			index_map[main_pairs[i].first] = i;
		}

		// サブソート用関数を定義
		auto SubSort = [&](const auto& a, const auto& b)
		{
			switch (sub_pairs_sort_kind)
			{
			case SortKind::kAscending:
				return sub_pairs[index_map.at(a.first)].second < sub_pairs[index_map.at(b.first)].second;

			case SortKind::kDescending:
				return sub_pairs[index_map.at(a.first)].second > sub_pairs[index_map.at(b.first)].second;
			}
			return false;
		};

		// メインソート
		std::sort(sorted_pairs.begin(), sorted_pairs.end(), [&](const auto& a, const auto& b)
		{
			switch (main_pairs_sort_kind)
			{
			case SortKind::kAscending:
				if (a.second != b.second) { return a.second < b.second; }
				
				return SubSort(a, b);

			case SortKind::kDescending:
				if (a.second != b.second) { return a.second > b.second; }

				return SubSort(a, b);
			}
			return false;
		});

		return sorted_pairs;
	}
	#pragma endregion
}
