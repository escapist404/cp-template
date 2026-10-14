#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite_large_array"

#include <algorithm>
#include <iostream>
#include <vector>

#include "../../include/coordinate_compression.hpp"
#include "../../include/fastio.hpp"
#include "../../include/modint.hpp"
#include "../../include/segtree.hpp"

using Z = ModInt<long long, 998244353>;

struct Affine {
	Z a, b;
};

Affine operation(Affine left, Affine right) {
	return {right.a * left.a, right.a * left.b + right.b};
}

Affine identity() { return {Z(1), Z(0)}; }

struct Query {
	int type;
	long long x, y, z;
};

int main() {
	OptimizedIO<1 << 20> io;
	long long n;
	int q;
	io >> n >> q;
	std::vector<Query> queries(q);
	std::vector<long long> positions;
	for (auto& query : queries) {
		io >> query.type;
		if (query.type == 0) {
			io >> query.x >> query.y >> query.z;
			positions.push_back(query.x);
		} else io >> query.x >> query.y >> query.z;
	}
	std::sort(positions.begin(), positions.end());
	positions.erase(std::unique(positions.begin(), positions.end()), positions.end());
	SegmentTree<Affine, operation, identity> tree((int)positions.size());
	for (auto query : queries) {
		if (query.type == 0) {
			int p = std::lower_bound(positions.begin(), positions.end(), query.x) - positions.begin();
			tree.set(p, {Z(query.y), Z(query.z)});
		} else {
			int l = std::lower_bound(positions.begin(), positions.end(), query.x) - positions.begin();
			int r = std::lower_bound(positions.begin(), positions.end(), query.y) - positions.begin();
			Affine f = tree.prod(l, r);
			std::cout << f.a * Z(query.z) + f.b << '\n';
		}
	}
}
