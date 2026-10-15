#ifndef EULER_PATH_HPP
#define EULER_PATH_HPP

#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

// ============================================================================
// 【有向图 / 无向图欧拉路径与欧拉回路 Hierholzer 算法 (euler_path.hpp)】
// ----------------------------------------------------------------------------
// 结果结构体签名：
//   struct EulerPath {
//       bool exists = false;        // 是否存在经过所有边恰好一次的欧拉路径
//       std::vector<int> vertices;  // 按行进顺序经过的顶点序列（有解时长度为 m + 1）
//       std::vector<int> edges;     // 按行进顺序经过的边在输入 edges 数组中的 0-based 下标序列（长度为 m）
//   };
// 求解函数签名：
//   EulerPath euler_path(int n, const std::vector<std::pair<int, int>>& edges,
//                        bool directed = false, int start = -1)
// 参数与返回值详解：
//   - n：顶点数，顶点编号为 0-based [0, n)。
//   - edges：边列表，第 id 项为 {u, v}（支持平行重边与自环）。
//   - directed：false 表示无向图（默认），true 表示有向图。
//   - start：指定起点编号；传入默认 -1 时自动寻找合法起点（若有奇度点/出度大1点则选之，否则选首个非孤立点）。
//   - 返回值 EulerPath：若不存在欧拉路径，返回 exists == false；若存在，返回 exists == true 及完整的顶点序列与边编号序列。
// 复杂度：时间与空间复杂度均为严格线性 O(n + m)。
// ============================================================================

struct EulerPath {
	bool exists = false;
	std::vector<int> vertices, edges;
};

// Hierholzer 欧拉路径/回路算法：边编号对应 edges 数组下标 [0, m)，支持重边与自环。
EulerPath euler_path(int n, const std::vector<std::pair<int, int>>& edges,
		bool directed = false, int start = -1) {
	assert(n >= 0);
	assert(start == -1 || (0 <= start && start < n));
	std::vector<std::vector<std::pair<int, int>>> adj(n);
	std::vector<int> in(n), out(n);
	for (int id = 0; id < (int)edges.size(); ++id) {
		const auto [u, v] = edges[id];
		assert(0 <= u && u < n && 0 <= v && v < n);
		adj[u].push_back({v, id});
		++out[u];
		++in[v];
		if (!directed) {
			adj[v].push_back({u, id});
			++out[v];
			++in[u];
		}
	}
	int required = -1, ends = 0, begins = 0, first = -1;
	for (int u = 0; u < n; ++u) {
		if (out[u] && first == -1) first = u;
		if (directed) {
			if (out[u] - in[u] == 1) {
				required = u;
				++begins;
			} else if (in[u] - out[u] == 1) {
				++ends;
			} else if (in[u] != out[u]) {
				return {};
			}
		} else if (out[u] & 1) {
			required = u;
			++ends;
		}
	}
	if (directed ? !((begins == 0 && ends == 0) || (begins == 1 && ends == 1))
	             : !(ends == 0 || ends == 2)) return {};
	if (edges.empty()) {
		return {true, n ? std::vector<int>{start == -1 ? 0 : start} : std::vector<int>{}, {}};
	}
	if (start == -1) start = required == -1 ? first : required;
	if (!out[start] || (directed && begins && start != required) ||
		(!directed && ends && !(out[start] & 1))) return {};
	std::vector<int> index(n);
	std::vector<char> used(edges.size());
	std::vector<std::pair<int, int>> stack{{start, -1}};
	EulerPath result;
	while (!stack.empty()) {
		const int u = stack.back().first;
		while (index[u] < (int)adj[u].size() && used[adj[u][index[u]].second]) ++index[u];
		if (index[u] == (int)adj[u].size()) {
			result.vertices.push_back(u);
			if (stack.back().second != -1) result.edges.push_back(stack.back().second);
			stack.pop_back();
		} else {
			const auto [v, id] = adj[u][index[u]++];
			used[id] = true;
			stack.push_back({v, id});
		}
	}
	if ((int)result.edges.size() != (int)edges.size()) return {};
	std::reverse(result.vertices.begin(), result.vertices.end());
	std::reverse(result.edges.begin(), result.edges.end());
	result.exists = true;
	return result;
}

#endif
