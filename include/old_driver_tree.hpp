#ifndef OLD_DRIVER_TREE_HPP
#define OLD_DRIVER_TREE_HPP
#include <map>
#include <vector>

// ============================================================================
// 【珂朵莉树 / 颜色段均摊 Old Driver Tree / Chtholly Tree (old_driver_tree.hpp)】
// ----------------------------------------------------------------------------
// 模板类签名：template <typename T> class OldDriverTree
//   基于 std::map<int, T> 维护连续相同值区间 [l, r)，所有下标均为 0-based。
//   当仅使用区间推平 set 与单点查询 get 时，在任意构造数据下 q 次操作总复杂度严格均摊 O((n + q) log n)。
// 构造函数与成员函数签名详解：
//   - OldDriverTree(const std::vector<T>& arr)
//     功能：以长度为 n 的初始数组 arr 初始化区间表，并在位置 n 插入右端哨兵。时间复杂度 O(n log n)。
//   - auto split(int pos)
//     功能：在下标 pos 处切开所属的同值区间，返回指向左端点为 pos 的 map 迭代器。O(log n)。
//   - void set(int l, int r, T v)
//     功能：将左闭右开区间 [l, r) (0 <= l < r <= n) 整体推平赋值为 v。均摊 O(log n)。
//   - T get(int pos)
//     功能：查询下标 pos (0 <= pos < n) 所属颜色段的当前值。时间复杂度 O(log n)。
// ============================================================================

template <typename T>
class OldDriverTree {
private:
    std::map<int, T> map;
 
public:
    // OldDriverTree(const std::vector<T>& arr) — 功能：以长度为 n 的初始数组 arr 初始化区间表，并在位置 n 插入右端哨兵。时间复杂度 O(n log n)。
    OldDriverTree(const std::vector<T>& arr) {
        for (int i = 0; i < (int)arr.size(); ++i) {
            map[i] = arr[i];
        }
        map[arr.size()] = T();
    }
    // auto split(int pos) — 功能：在下标 pos 处切开所属的同值区间，返回指向左端点为 pos 的 map 迭代器。O(log n)。
    inline auto split(int pos) {
        auto item = map.lower_bound(pos);
        if (item->first == pos) return item;
        --item;
        map.insert({pos, item->second});
        return ++item;
    }
    // void set(int l, int r, T v) — 功能：将左闭右开区间 [l, r) (0 <= l < r <= n) 整体推平赋值为 v。均摊 O(log n)。
    void set(int l, int r, T v) {
        auto end = split(r);
        map.erase(split(l), end);
        map[l] = v;
    }
    // T get(int pos) — 功能：查询下标 pos (0 <= pos < n) 所属颜色段的当前值。时间复杂度 O(log n)。
    T get(int pos) {
        auto item = --map.upper_bound(pos);
        return item->second;
    }
};

#endif
