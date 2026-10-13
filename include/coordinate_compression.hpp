#ifndef COORDINATE_COMPRESSION_HPP
#define COORDINATE_COMPRESSION_HPP

#include <algorithm>
#include <utility>
#include <vector>

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
