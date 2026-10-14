#define PROBLEM "https://judge.yosupo.jp/problem/static_range_sum_with_upper_bound"

#include "../../include/fastio.hpp"
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

#include "../../include/fenwick.hpp"

struct Query {
	int l, r, id;
	long long x;
};

int main() {
	OptimizedIO<1 << 20> io;
	int n, q;
	io >> n >> q;
	std::vector<long long> a(n);
	for (auto& x : a) io >> x;
	std::vector<std::pair<long long, int>> values;
	for (int i = 0; i < n; ++i) values.push_back({a[i], i});
	std::sort(values.begin(), values.end());
	std::vector<Query> queries(q);
	for (int i = 0; i < q; ++i) {
		io >> queries[i].l >> queries[i].r >> queries[i].x;
		queries[i].id = i;
	}
	std::sort(queries.begin(), queries.end(), [](Query a, Query b) { return a.x < b.x; });
	Fenwick<int> count(n);
	Fenwick<long long> sum(n);
	std::vector<std::pair<int, long long>> answer(q);
	int p = 0;
	for (const auto& query : queries) {
		while (p < n && values[p].first <= query.x) {
			count.add(values[p].second, 1);
			sum.add(values[p].second, values[p].first);
			++p;
		}
		answer[query.id] = {count.get(query.l, query.r), sum.get(query.l, query.r)};
	}
	for (auto [count_value, sum_value] : answer)
		std::cout << count_value << ' ' << sum_value << '\n';
}
