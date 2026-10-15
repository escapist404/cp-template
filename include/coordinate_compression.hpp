#ifndef COORDINATE_COMPRESSION_HPP
#define COORDINATE_COMPRESSION_HPP

#include <algorithm>
#include <utility>
#include <vector>

// 【坐标压缩 / 离散化 (coordinate_compression.hpp)】
// - 将任意可比较类型数组映射为 0-based 紧凑排名 [0, m)，时间复杂度 O(n log n)
// - 返回值: std::pair{unique_values, index}
//   1. unique_values: 升序排列且去重后的原值表（长度为不同元素个数 m）
//   2. index        : 与原数组等长的 0-based 排名数组，满足 unique_values[index[i]] == values[i]

template <typename T>
std::pair<std::vector<T>, std::vector<int>> coordinate_compress(
		const std::vector<T>& values) {
	std::vector<T> unique_values = values;
	std::sort(unique_values.begin(), unique_values.end());
	unique_values.erase(std::unique(unique_values.begin(), unique_values.end()),
		unique_values.end());
	std::vector<int> index(values.size());
	for (int i = 0; i < (int)values.size(); ++i) {
		index[i] = std::lower_bound(unique_values.begin(), unique_values.end(), values[i])
			- unique_values.begin();
	}
	return {std::move(unique_values), std::move(index)};
}

#endif
