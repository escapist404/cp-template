#define PROBLEM "https://judge.yosupo.jp/problem/segment_add_get_min"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "../../include/lichao.hpp"

struct Line {
	long long a, b;
	long long operator()(long long x) const { return a * x + b; }
};

struct Query {
	int type;
	long long l, r, a, b;
};

int main() {
	int n, q;
	std::cin >> n >> q;
	std::vector<Query> initial(n), queries;
	std::vector<long long> coordinates;
	for (auto& query : initial) {
		std::cin >> query.l >> query.r >> query.a >> query.b;
		coordinates.push_back(query.l);
		coordinates.push_back(query.r);
	}
	for (int i = 0; i < q; ++i) {
		Query query;
		std::cin >> query.type;
		if (query.type == 0) {
			std::cin >> query.l >> query.r >> query.a >> query.b;
			coordinates.push_back(query.l);
			coordinates.push_back(query.r);
		} else {
			std::cin >> query.l;
			coordinates.push_back(query.l);
		}
		queries.push_back(query);
	}
	LiChaoTree<long long, Line> tree(coordinates);
	for (const auto& query : initial)
		tree.add({query.a, query.b}, query.l, query.r);
	for (const auto& query : queries) {
		if (query.type == 0) {
			tree.add({query.a, query.b}, query.l, query.r);
		} else {
			auto result = tree.get(query.l);
			if (result) std::cout << *result << '\n';
			else std::cout << "INFINITY\n";
		}
	}
}
