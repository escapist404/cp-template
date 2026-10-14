#ifndef OLD_DRIVER_TREE_HPP
#define OLD_DRIVER_TREE_HPP
#include <map>
#include <vector>

// 0-based Chtholly tree (ODT) maintaining interval assignments on [l, r) via std::map.

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
