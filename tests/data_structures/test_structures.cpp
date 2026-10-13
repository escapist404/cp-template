#include <cassert>
#include <functional>
#include <vector>
#include "../../include/dsu.hpp"
#include "../../include/dynamic_bitset.hpp"
#include "../../include/linear_basis.hpp"
#include "../../include/old_driver_tree.hpp"
#include "../../include/scapegoat.hpp"
#include "../../include/treap.hpp"
#include "../../include/monotone_queue.hpp"
#include "../../include/convex_hull_trick.hpp"
#include "../../include/coordinate_compression.hpp"

int main() {
	std::vector<int> values{1, 3, -1, -3, 5, 3, 6, 7};
	assert((sliding_window(values, 3) == std::vector<int>{3, 3, 5, 5, 6, 7}));
	assert((sliding_window(values, 3, std::greater<int>{}) ==
		std::vector<int>{-1, -3, -3, -3, 3, 3}));

	ConvexHullTrick<long long> hull;
	hull.add_line(1, 0);
	hull.add_line(2, -1);
	hull.add_line(3, 0);
	assert(hull.query(-10) == -30);
	assert(hull.query(10) == 10);

	auto compression = coordinate_compress(std::vector<int>{5, 1, 5, 3});
	assert(compression.first == std::vector<int>({1, 3, 5}));
	assert(compression.second == std::vector<int>({2, 0, 2, 1}));

    DisjointSetUnion dsu(4);
    dsu.merge(0, 1);
    dsu.merge(1, 2);
    assert(dsu.same(0, 2) && dsu.size(0) == 3 && dsu.component_number() == 2);

    DynamicBitset bits(70);
    bits.set(1), bits.set(64), bits.set(69);
    assert(bits.count() == 3 && bits.test(64) && !bits.test(2));
    bits.reset(64);
    assert(bits.count() == 2);
    DynamicBitset other(70); other.set(2);
    assert((bits | other).count() == 3);

    LinearBasis basis;
    basis.insert(6), basis.insert(10);
    assert(basis.contains(12) && basis.max_xor(1) == 13);

    OldDriverTree<int> odt({1, 1, 2, 2});
    odt.set(1, 3, 7);
    assert(odt.get(0) == 1 && odt.get(1) == 7 && odt.get(2) == 7 && odt.get(3) == 2);

    ScapegoatTree<int> scapegoat;
    scapegoat.insert(3), scapegoat.insert(1), scapegoat.insert(2), scapegoat.insert(2);
    assert(scapegoat.size() == 4 && scapegoat.get_rank(2) == 2);
    assert(scapegoat.get_value(3) == 2 && scapegoat.get_prev(2) == 1 && scapegoat.get_next(2) == 3);
    scapegoat.remove(2);
    assert(scapegoat.size() == 3);

    Treap<int> treap;
    treap.insert(3), treap.insert(1), treap.insert(2), treap.insert(2);
    assert(treap.get_rank(2) == 2 && treap.get_value(3) == 2);
    assert(treap.get_prev(2) == 1 && treap.get_next(2) == 3);
    treap.remove(2);
    assert(treap.get_rank(3) == 3);
}
