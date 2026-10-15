#ifndef LINK_CUT_TREE_HPP
#define LINK_CUT_TREE_HPP

#include <algorithm>
#include <cassert>
#include <vector>

// ============================================================================
// 【动态树 Link-Cut Tree (link_cut_tree.hpp)】
// ----------------------------------------------------------------------------
// 模板类签名：template <typename T> class LinkCutTree
//   基于 Splay 树与实链剖分维护森林的动态加删边、连通性判定与路径点权和查询。
//   【重要约定】
//   1. 顶点编号严格为【1-based】，合法范围 1 <= x <= n（0 号节点内部保留为空节点哨兵，传入 0 会触发 assert）。
//   2. 聚合运算固定为点权加法 +，因 make_root 会翻转路径方向，类型 T 的 + 必须满足交换律与结合律。
// 构造函数与成员函数签名详解（所有操作均摊时间复杂度 O(log n)）：
//   - LinkCutTree(int n = 0)
//     功能：初始化包含 n 个孤立顶点 [1, n] 的动态森林，初始点权均为 T()。时空复杂度 O(n)。
//   - void set_value(int x, T value)
//     功能：将顶点 x (1 <= x <= n) 的点权修改为 value 并更新路径聚合和。均摊 O(log n)。
//   - T value(int x) const
//     功能：读取顶点 x (1 <= x <= n) 的当前单点权值。时间复杂度 O(1)。
//   - bool connected(int x, int y)
//     功能：判断顶点 x 与 y 当前是否处于同一棵连通树中。均摊 O(log n)。
//   - bool link(int x, int y)
//     功能：在顶点 x 与 y 之间连接一条无向树边；若已连通则拒绝连边并返回 false，成功连边返回 true。均摊 O(log n)。
//   - bool cut(int x, int y)
//     功能：断开顶点 x 与 y 之间的直接无向边；若 x 与 y 之间不存在直接相连的树边，则不修改并返回 false。均摊 O(log n)。
//   - T path_query(int x, int y)
//     功能：查询顶点 x 到 y 的唯一简单路径上的点权总和（调用前必须保证 connected(x, y) == true，否则触发 assert）。均摊 O(log n)。
// ============================================================================

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
	// LinkCutTree(int n = 0) — 功能：初始化包含 n 个孤立顶点 [1, n] 的动态森林，初始点权均为 T()。时空复杂度 O(n)。
	LinkCutTree(int n = 0) : tree(n + 1) {}

	// void set_value(int x, T value) — 功能：将顶点 x (1 <= x <= n) 的点权修改为 value 并更新路径聚合和。均摊 O(log n)。
	void set_value(int x, T value) {
		check(x);
		access(x);
		tree[x].value = value;
		pull(x);
	}

	// T value(int x) const — 功能：读取顶点 x (1 <= x <= n) 的当前单点权值。时间复杂度 O(1)。
	T value(int x) const {
		check(x);
		return tree[x].value;
	}

	// bool connected(int x, int y) — 功能：判断顶点 x 与 y 当前是否处于同一棵连通树中。均摊 O(log n)。
	bool connected(int x, int y) {
		check(x), check(y);
		return x == y || find_root(x) == find_root(y);
	}

	// bool link(int x, int y) — 功能：在顶点 x 与 y 之间连接一条无向树边；若已连通则拒绝连边并返回 false，成功连边返回 true。均摊 O(log n)。
	bool link(int x, int y) {
		check(x), check(y);
		make_root(x);
		if (find_root(y) == x) return false;
		tree[x].parent = y;
		return true;
	}

	// bool cut(int x, int y) — 功能：断开顶点 x 与 y 之间的直接无向边；若 x 与 y 之间不存在直接相连的树边，则不修改并返回 false。均摊 O(log n)。
	bool cut(int x, int y) {
		check(x), check(y);
		split(x, y);
		if (tree[y].child[0] != x || tree[x].child[1]) return false;
		tree[y].child[0] = tree[x].parent = 0;
		pull(y);
		return true;
	}

	// T path_query(int x, int y) — 功能：查询顶点 x 到 y 的唯一简单路径上的点权总和（调用前必须保证 connected(x, y) == true，否则触发 assert）。均摊 O(log n)。
	T path_query(int x, int y) {
		check(x), check(y);
		split(x, y);
		return tree[y].sum;
	}
};

#endif
