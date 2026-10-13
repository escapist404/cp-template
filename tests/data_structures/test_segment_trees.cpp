#include <algorithm>
#include <cassert>
#include <limits>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include "../../include/segtree.hpp"
#include "../../include/lazy_segtree.hpp"
#include "../../include/dynamic_segtree.hpp"
#include "../../include/persistent_segtree.hpp"

using i64 = long long;
i64 sum_op(i64 a, i64 b) { return a + b; }
i64 sum_e() { return 0; }
i64 min_op(i64 a, i64 b) { return std::min(a, b); }
i64 min_e() { return std::numeric_limits<i64>::max(); }
struct Value { i64 sum; int len; };
struct Tag { i64 a, b; };
Value op(Value x, Value y) { return {x.sum + y.sum, x.len + y.len}; }
Value e() { return {0, 0}; }
Value mapping(Tag f, Value x) { return {f.a * x.sum + f.b * x.len, x.len}; }
Tag composition(Tag f, Tag g) { return {f.a * g.a, f.a * g.b + f.b}; }
Tag id() { return {1, 0}; }
using Lazy = LazySegmentTree<Value, op, e, Tag, mapping, composition, id>;

std::string concat(std::string a, std::string b) { return a + b; }
std::string empty() { return ""; }
std::string assign(char f, std::string s) { return f ? std::string(s.size(), f) : s; }
char compose(char f, char g) { return f ? f : g; }
char no_tag() { return 0; }

template<class Tree>
void check_order(Tree& st, std::string s) {
    int n = s.size();
    for (int l = 0; l <= n; ++l) for (int r = l; r <= n; ++r) {
        assert(st.prod(l, r) == s.substr(l, r - l));
        std::string target = s.substr(l, r - l);
        assert(st.max_right(l, [&](std::string x) {
            return x.size() <= target.size() && x == target.substr(0, x.size());
        }) == r);
        assert(st.min_left(r, [&](std::string x) {
            return x.size() <= target.size() && x == target.substr(target.size() - x.size());
        }) == l);
    }
}

int main() {
    SegmentTree<i64, sum_op, sum_e> empty_tree;
    Lazy empty_lazy;
    assert(empty_tree.prod(0, 0) == 0 && empty_tree.all_prod() == 0);
    assert(empty_lazy.prod(0, 0).len == 0 && empty_lazy.all_prod().sum == 0);
    empty_lazy.apply(0, 0, {0, 7});
    assert(empty_lazy.max_right(0, [](Value x) { return x.sum == 0; }) == 0);
    assert(empty_lazy.min_left(0, [](Value x) { return x.sum == 0; }) == 0);

    std::mt19937 rng(404);
    for (int n : {1, 2, 5, 17, 32}) {
        std::vector<i64> a(n);
        std::vector<Value> initial(n, {0, 1});
        Lazy st(initial);
        SegmentTree<i64, sum_op, sum_e> plain(n);
        for (int step = 0; step < 300; ++step) {
            int l = rng() % (n + 1), r = rng() % (n + 1);
            if (l > r) std::swap(l, r);
            int p = rng() % n;
            if (step % 3 == 0) {
                Tag f{(i64)(rng() % 2), (i64)(rng() % 8)}; // add or assign
                st.apply(l, r, f);
                for (int i = l; i < r; ++i) a[i] = f.a * a[i] + f.b;
            } else if (step % 3 == 1) {
                a[p] = rng() % 40;
                st.set(p, {a[p], 1});
            } else {
                st.apply(p, {2, 3});
                a[p] = 2 * a[p] + 3;
            }
            for (int i = 0; i < n; ++i) plain.set(i, a[i]);
            i64 want = std::accumulate(a.begin() + l, a.begin() + r, 0LL);
            assert(st.prod(l, r).sum == want && st.prod(l, r).len == r - l);
            assert(plain.prod(l, r) == want && st.get(p).sum == a[p]);
            assert(st.all_prod().sum == plain.all_prod());
            i64 limit = rng() % 100;
            int right = l, left = r;
            i64 sum = 0;
            while (right < n && sum + a[right] <= limit) sum += a[right++];
            sum = 0;
            while (left > 0 && sum + a[left - 1] <= limit) sum += a[--left];
            assert(st.max_right(l, [&](Value x) { return x.sum <= limit; }) == right);
            assert(st.min_left(r, [&](Value x) { return x.sum <= limit; }) == left);
            assert(plain.max_right(l, [&](i64 x) { return x <= limit; }) == right);
            assert(plain.min_left(r, [&](i64 x) { return x <= limit; }) == left);
        }
    }

    std::vector<std::string> chars{"a", "b", "c", "d", "e"};
    SegmentTree<std::string, concat, empty> strings(chars);
    check_order(strings, "abcde");
    LazySegmentTree<std::string, concat, empty, char, assign, compose, no_tag> lazy_strings(chars);
    lazy_strings.apply(0, 5, 'x');
    lazy_strings.apply(1, 4, 'y');
    lazy_strings.set(2, "z");
    check_order(lazy_strings, "xyzyx");

    DynamicSegmentTree<std::string, concat, empty> dynamic_strings(-3, 4);
    int text_root = 0;
    dynamic_strings.set(text_root, 2, "c");
    dynamic_strings.set(text_root, -2, "a");
    dynamic_strings.add(text_root, 0, "b");
    assert(dynamic_strings.all_prod(text_root) == "abc");
    assert(dynamic_strings.prod(text_root, -1, 4) == "bc");
    assert(dynamic_strings.prod(text_root, 1, 1).empty());

    DynamicSegmentTree<i64, sum_op, sum_e> dynamic_sum(-4, 5);
    int dynamic_root = 0;
    for (int i = -4; i < 5; ++i) dynamic_sum.set(dynamic_root, i, i + 4);
    for (int l = -4; l <= 5; ++l) for (int r = l; r <= 5; ++r) {
        i64 limit = 100000;
        i64 sum = 0;
        int want_right = l;
        while (want_right < 5 && sum + want_right + 4 <= limit)
            sum += want_right++ + 4;
        assert(dynamic_sum.max_right(dynamic_root, l, [&](i64 x) { return x <= limit; }) == want_right);
        sum = 0;
        int want_left = r;
        while (want_left > -4 && sum + want_left - 1 + 4 <= limit)
            sum += --want_left + 4;
        assert(dynamic_sum.min_left(dynamic_root, r, [&](i64 x) { return x <= limit; }) == want_left);
    }
    assert(dynamic_sum.max_right(dynamic_root, -4, [](i64 x) { return x <= 0; }) == -3);
    assert(dynamic_sum.min_left(0, 0, [](i64 x) { return x <= 0; }) == -4);
    assert(dynamic_sum.min_left(dynamic_root, 5, [](i64 x) { return x <= 0; }) == 5);

    DynamicSegmentTree<i64, min_op, min_e> sparse(-1000000000000LL, 1000000000000LL);
    int x = 0, y = 0;
    assert(sparse.prod(x, -5, 5) == min_e());
    sparse.set(x, -999999999999LL, 7);
    sparse.set(x, 3, 9);
    sparse.set(y, 3, 2);
    sparse.set(y, 999999999999LL, 8);
    sparse.merge(x, y);
    assert(y == 0 && sparse.all_prod(x) == 2 && sparse.get(x, 3) == 2);
    assert(sparse.prod(x, -1000000000000LL, 0) == 7);

    PersistentSegmentTree<std::string, concat, empty> persistent_strings(5);
    int v0 = persistent_strings.build(chars);
    int v1 = persistent_strings.set(v0, 1, "x");
    int v2 = persistent_strings.add(v1, 1, "y");
    assert(persistent_strings.all_prod(v0) == "abcde");
    assert(persistent_strings.prod(v1, 1, 4) == "xcd");
    assert(persistent_strings.get(v2, 1) == "xy");
    assert(persistent_strings.max_right(v2, 0, [](std::string x) { return x.size() <= 3; }) == 2);
    assert(persistent_strings.min_left(v2, 5, [](std::string x) { return x.size() <= 2; }) == 3);

    PersistentSegmentTree<i64, min_op, min_e> persistent_min(5);
    int min_root = persistent_min.set(0, 2, 7);
    assert(persistent_min.all_prod(0) == min_e());
    assert(persistent_min.all_prod(min_root) == 7);
    assert(persistent_min.prod(min_root, 0, 2) == min_e());

    PersistentSegmentTree<i64, sum_op, sum_e> pst(17);
    std::vector<int> roots{0};
    std::vector<std::vector<i64>> versions(1, std::vector<i64>(17));
    for (int step = 0; step < 200; ++step) {
        int old = rng() % roots.size(), p = rng() % 17;
        auto a = versions[old];
        i64 delta = rng() % 6;
        a[p] += delta;
        roots.push_back(pst.add(roots[old], p, delta));
        versions.push_back(a);
        int selected = rng() % roots.size();
        int l = rng() % 18, r = rng() % 18;
        if (l > r) std::swap(l, r);
        assert(pst.prod(roots[selected], l, r) == std::accumulate(
            versions[selected].begin() + l, versions[selected].begin() + r, 0LL));
        i64 limit = 100000;
        int right = 0;
        i64 sum = 0;
        while (right < 17 && sum + versions[selected][right] <= limit)
            sum += versions[selected][right++];
        assert(pst.max_right(roots[selected], 0, [&](i64 x) { return x <= limit; }) == right);
        int left = 17;
        sum = 0;
        while (left > 0 && sum + versions[selected][left - 1] <= limit)
            sum += versions[selected][--left];
        assert(pst.min_left(roots[selected], 17, [&](i64 x) { return x <= limit; }) == left);
    }
    int before = roots.back(), after = pst.add(before, 8, 3);
    for (int k = 0; k < 3; ++k) assert(pst.kth(before, after, k) == 8);
}
