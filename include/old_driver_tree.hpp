#ifndef OLD_DRIVER_TREE_HPP
#define OLD_DRIVER_TREE_HPP
#include <map>
#include <vector>

// 【珂朵莉树 / 颜色段均摊 (old_driver_tree.hpp)】
// - 基于 std::map 维护连续相同值区间的结构，0-based 左闭右开区间 [l, r)
// - set(l, r, v): 将半开区间 [l, r) (0 <= l < r <= n) 推平赋值为 v
// - get(pos)    : 查询下标 pos (0 <= pos < n) 处的当前值
// - 仅执行 set 与 get 时，q 次操作总时间复杂度在任意数据下均摊为 O((n + q) log n)

template <typename T>
class OldDriverTree {
private:
    std::map<int, T> map;
 
public:
    OldDriverTree(const std::vector<T>& arr) {
        for (int i = 0; i < (int)arr.size(); ++i) {
            map[i] = arr[i];
        }
        map[arr.size()] = T();
    }
    inline auto split(int pos) {
        auto item = map.lower_bound(pos);
        if (item->first == pos) return item;
        --item;
        map.insert({pos, item->second});
        return ++item;
    }
    void set(int l, int r, T v) {
        auto end = split(r);
        map.erase(split(l), end);
        map[l] = v;
    }
    T get(int pos) {
        auto item = --map.upper_bound(pos);
        return item->second;
    }
};

#endif
