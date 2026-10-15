#ifndef COORDINATE_COMPRESSION_HPP
#define COORDINATE_COMPRESSION_HPP

#include <algorithm>
#include <utility>
#include <vector>

// ============================================================================
// 【坐标压缩 / 离散化 (coordinate_compression.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   template <typename T>
//   std::pair<std::vector<T>, std::vector<int>> coordinate_compress(const std::vector<T>& values)
// 功能与参数说明：
//   - 输入参数 values：待离散化的原始元素数组，元素类型 T 需支持 < 与 == 比较。
//   - 返回值 std::pair{unique_values, index}：
//     1. unique_values (std::vector<T>)：将 values 升序排序并去重后的唯一值表，长度为不同元素个数 m。
//     2. index (std::vector<int>)：与 values 等长的 0-based 排名数组，满足 0 <= index[i] < m 且
//        unique_values[index[i]] == values[i]。
// 复杂度：时间复杂度 O(n log n)，空间复杂度 O(n)。
// ============================================================================

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
