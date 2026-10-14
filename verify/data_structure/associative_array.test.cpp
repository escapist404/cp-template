#define PROBLEM "https://judge.yosupo.jp/problem/associative_array"

#include <iostream>
#include <utility>
#include <vector>

#include "../../include/coordinate_compression.hpp"
#include "../../include/fastio.hpp"

struct Query {
	int type;
	long long key, value;
};

int main() {
	OptimizedIO<1 << 20> io;
	int q;
	io >> q;
	std::vector<Query> queries(q);
	std::vector<long long> keys;
	for (auto& query : queries) {
		io >> query.type >> query.key;
		keys.push_back(query.key);
		if (query.type == 0) io >> query.value;
	}
	auto compressed = coordinate_compress(keys);
	std::vector<long long> value(compressed.first.size());
	for (auto query : queries) {
		int index = std::lower_bound(compressed.first.begin(), compressed.first.end(), query.key)
			- compressed.first.begin();
		if (query.type == 0) value[index] = query.value;
		else std::cout << value[index] << '\n';
	}
}
