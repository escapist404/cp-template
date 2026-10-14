#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum_large_array"

#include <algorithm>
#include <iostream>
#include <vector>

#include "../../include/fastio.hpp"
#include "../../include/lazy_segtree.hpp"
#include "../../include/modint.hpp"

using Z = ModInt<long long, 998244353>;

struct Node {
	Z sum, length;
};

struct Tag {
	Z a, b;
};

Node operation(Node left, Node right) {
	return {left.sum + right.sum, left.length + right.length};
}

Node identity() { return {Z(0), Z(0)}; }

Node mapping(Tag tag, Node node) {
	return {tag.a * node.sum + tag.b * node.length, node.length};
}

Tag composition(Tag newer, Tag older) {
	return {newer.a * older.a, newer.a * older.b + newer.b};
}

Tag tag_identity() { return {Z(1), Z(0)}; }

struct Query {
	int type;
	long long l, r, a, b;
};

int main() {
	OptimizedIO<1 << 20> io;
	long long n;
	int q;
	io >> n >> q;
	std::vector<Query> queries(q);
	std::vector<long long> coordinates{0, n};
	for (auto& query : queries) {
		io >> query.type >> query.l >> query.r;
		coordinates.push_back(query.l);
		coordinates.push_back(query.r);
		if (query.type == 0) io >> query.a >> query.b;
	}
	std::sort(coordinates.begin(), coordinates.end());
	coordinates.erase(std::unique(coordinates.begin(), coordinates.end()), coordinates.end());
	std::vector<Node> initial;
	for (int i = 0; i + 1 < (int)coordinates.size(); ++i)
		initial.push_back({Z(0), Z(coordinates[i + 1] - coordinates[i])});
	LazySegmentTree<Node, operation, identity, Tag, mapping, composition, tag_identity>
		tree(std::move(initial));
	for (auto query : queries) {
		int l = std::lower_bound(coordinates.begin(), coordinates.end(), query.l) - coordinates.begin();
		int r = std::lower_bound(coordinates.begin(), coordinates.end(), query.r) - coordinates.begin();
		if (query.type == 0) tree.apply(l, r, {Z(query.a), Z(query.b)});
		else std::cout << tree.prod(l, r).sum << '\n';
	}
}
