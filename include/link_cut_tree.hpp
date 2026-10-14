#ifndef LINK_CUT_TREE_HPP
#define LINK_CUT_TREE_HPP

#include <algorithm>
#include <cassert>
#include <vector>

// 1-based Link-Cut Tree maintaining dynamic forest connectivity and path sums in amortized O(log n).

template <typename T>
class LinkCutTree {
	struct Node {
		int child[2] = {};
		int parent = 0;
		bool reversed = false;
		T value = T();
		T sum = T();
	};

	std::vector<Node> tree;

	bool is_root(int x) const {
		int p = tree[x].parent;
		return !p || (tree[p].child[0] != x && tree[p].child[1] != x);
	}

	void pull(int x) {
		tree[x].sum = tree[tree[x].child[0]].sum + tree[x].value +
			tree[tree[x].child[1]].sum;
	}

	void reverse(int x) {
		if (!x) return;
		std::swap(tree[x].child[0], tree[x].child[1]);
		tree[x].reversed = !tree[x].reversed;
	}

	void push(int x) {
		if (!tree[x].reversed) return;
		reverse(tree[x].child[0]);
		reverse(tree[x].child[1]);
		tree[x].reversed = false;
	}

	void rotate(int x) {
		int y = tree[x].parent, z = tree[y].parent;
		int direction = tree[y].child[1] == x;
		int middle = tree[x].child[direction ^ 1];
		if (!is_root(y)) tree[z].child[tree[z].child[1] == y] = x;
		tree[x].parent = z;
		tree[x].child[direction ^ 1] = y;
		tree[y].parent = x;
		tree[y].child[direction] = middle;
		if (middle) tree[middle].parent = y;
		pull(y), pull(x);
	}

	void splay(int x) {
		std::vector<int> path;
		for (int cur = x;; cur = tree[cur].parent) {
			path.push_back(cur);
			if (is_root(cur)) break;
		}
		while (!path.empty()) {
			push(path.back());
			path.pop_back();
		}
		while (!is_root(x)) {
			int y = tree[x].parent, z = tree[y].parent;
			if (!is_root(y))
				((tree[y].child[0] == x) == (tree[z].child[0] == y)) ? rotate(y) : rotate(x);
			rotate(x);
		}
	}

	void access(int x) {
		int last = 0;
		for (int cur = x; cur; cur = tree[cur].parent) {
			splay(cur);
			tree[cur].child[1] = last;
			pull(cur);
			last = cur;
		}
		splay(x);
	}

	void make_root(int x) {
		access(x);
		reverse(x);
	}

	int find_root(int x) {
		access(x);
		while (tree[x].child[0]) {
			push(x);
			x = tree[x].child[0];
		}
		splay(x);
		return x;
	}

	void split(int x, int y) {
		make_root(x);
		access(y);
	}

	void check(int x) const { assert(1 <= x && x < (int)tree.size()); }

public:
	LinkCutTree(int n = 0) : tree(n + 1) {}

	void set_value(int x, T value) {
		check(x);
		access(x);
		tree[x].value = value;
		pull(x);
	}

	T value(int x) const {
		check(x);
		return tree[x].value;
	}

	bool connected(int x, int y) {
		check(x), check(y);
		return x == y || find_root(x) == find_root(y);
	}

	bool link(int x, int y) {
		check(x), check(y);
		make_root(x);
		if (find_root(y) == x) return false;
		tree[x].parent = y;
		return true;
	}

	bool cut(int x, int y) {
		check(x), check(y);
		split(x, y);
		if (tree[y].child[0] != x || tree[x].child[1]) return false;
		tree[y].child[0] = tree[x].parent = 0;
		pull(y);
		return true;
	}

	T path_query(int x, int y) {
		check(x), check(y);
		split(x, y);
		return tree[y].sum;
	}
};

#endif
