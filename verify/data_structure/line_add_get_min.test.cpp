#define PROBLEM "https://judge.yosupo.jp/problem/line_add_get_min"

#include "../../include/fastio.hpp"
#include <iostream>
#include <tuple>
#include <vector>

#include "../../include/lichao.hpp"

using i64 = long long;

struct Line {
	i64 a, b;
	i64 operator()(i64 x) const { return a * x + b; }
};

struct Query {
	int type;
	i64 a, b;
};

int main() {
	OptimizedIO<1 << 20> io;
	int n, q;
	io >> n >> q;
	std::vector<Line> initial(n);
	for (auto& line : initial) io >> line.a >> line.b;
	std::vector<Query> queries(q);
	std::vector<i64> coordinates;
	for (auto& query : queries) {
		io >> query.type >> query.a;
		if (query.type == 0) io >> query.b;
		else coordinates.push_back(query.a);
	}
	if (coordinates.empty()) return 0;
	LiChaoTree<i64, Line> tree(coordinates);
	for (const auto& line : initial) tree.add(line);
	for (const auto& query : queries) {
		if (query.type == 0) tree.add({query.a, query.b});
		else std::cout << *tree.get(query.a) << '\n';
	}
}
