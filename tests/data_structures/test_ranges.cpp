#include <cassert>
#include <vector>
#include "../../include/cartesian_tree.hpp"
#include "../../include/dynamic_segtree.hpp"
#include "../../include/fenwick.hpp"
#include "../../include/fenwick2d.hpp"
#include "../../include/lazy_segtree.hpp"
#include "../../include/lichao.hpp"
#include "../../include/persistent_segtree.hpp"
#include "../../include/segtree.hpp"
#include "../../include/sparse_table.hpp"
#include "../../include/wavelet_tree.hpp"

int sum_op(int a, int b) { return a + b; }
int sum_e() { return 0; }
struct Sum { int value, length; };
Sum sum_merge(Sum a, Sum b) { return {a.value + b.value, a.length + b.length}; }
Sum sum_identity() { return {0, 0}; }
Sum sum_mapping(int f, Sum a) { return {a.value + f * a.length, a.length}; }

struct Line {
    int a;
    int b;
    int operator()(int x) const { return a * x + b; }
};

int main() {
    auto cartesian = cartesian_tree(std::vector<int>{3, 1, 2, 1});
    assert(cartesian.root == 1);
    assert(cartesian.left[1] == 0 && cartesian.right[1] == 3);
    assert(cartesian.left[3] == 2 && cartesian.father[3] == 1);

    Fenwick<int> fenwick(std::vector<int>{1, 2, 3});
    assert(fenwick.get(0, 3) == 6 && fenwick.lower_bound(4) == 2);
    Fenwick2D<int> two(3, 3);
    two.add(1, 1, 5);
    assert(two.get(0, 0, 3, 3) == 5);
    DynamicSegmentTree<int, sum_op, sum_e> dynamic(0, 8);
    int root = 0;
    dynamic.add(root, 2, 3);
    dynamic.add(root, 5, 4);
    assert(dynamic.prod(root, 0, 8) == 7);
    LiChaoTree<int, Line> lichao({0, 1, 2});
    lichao.add({1, 0});
    lichao.add({0, 2});
    assert(*lichao.get(0) == 0);
    WaveletTree<int> wavelet({3, 1, 2, 1});
    assert(wavelet.kth(0, 4, 1) == 1);
    assert(wavelet.count(0, 4, 1, 3) == 3);
    PersistentSegmentTree<int, sum_op, sum_e> persistent(3);
    const int version = persistent.build({1, 2, 3});
    const int next = persistent.add(version, 1, 4);
    assert(persistent.prod(version, 0, 3) == 6);
    assert(persistent.prod(next, 0, 3) == 10);
    SparseTable<int> sparse(std::vector<int>{4, 1, 3, 2});
    assert(sparse.query(1, 4) == 3);
    SegmentTree<int, sum_op, sum_e> segment(std::vector<int>{1, 2, 3, 4});
    assert(segment.prod(1, 3) == 5);
    segment.set(2, 7);
    assert(segment.all_prod() == 14);
    LazySegmentTree<Sum, sum_merge, sum_identity, int, sum_mapping, sum_op, sum_e> lazy(
        std::vector<Sum>(4, {0, 1}));
    lazy.apply(1, 3, 2);
    assert(lazy.prod(1, 3).value == 4);
}
