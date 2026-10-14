#define PROBLEM "https://judge.yosupo.jp/problem/segment_add_get_min"

#include "../../include/fastio.hpp"
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
	OptimizedIO<1 << 20> io;
	int n, q;
	io >> n >> q;
	std::vector<Query> initial(n), queries;
	std::vector<long long> coordinates;
	for (auto& query : initial) {
		io >> query.l >> query.r >> query.a >> query.b;
		coordinates.push_back(query.l);
		coordinates.push_back(query.r);
	}
	for (int i = 0; i < q; ++i) {
		Query query;
		io >> query.type;
		if (query.type == 0) {
			io >> query.l >> query.r >> query.a >> query.b;
			coordinates.push_back(query.l);
			coordinates.push_back(query.r);
		} else {
			io >> query.l;
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
