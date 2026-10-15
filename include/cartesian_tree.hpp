#ifndef CARTESIAN_TREE_HPP
#define CARTESIAN_TREE_HPP

#include <functional>
#include <utility>
#include <vector>

// 【笛卡尔树 (cartesian_tree.hpp)】
// - 使用单调栈在严格 O(n) 时间内构造序列的笛卡尔树，中序遍历对应原数组 0-based 下标顺序
// - 默认 Compare = std::less<T> 构造小根笛卡尔树（父节点 <= 子节点；相等元素保持原下标靠前者在上）
// - 返回 CartesianTreeResult{root, left, right, father}，所有下标均为 0-based，不存在的子节点或父节点为 -1

struct CartesianTreeResult {
    int root = -1;
    std::vector<int> left, right, father;
};

template <typename T, typename Compare = std::less<T>>
CartesianTreeResult cartesian_tree(const std::vector<T>& values,
        Compare compare = Compare{}) {
    CartesianTreeResult result;
    result.left.assign(values.size(), -1);
    result.right.assign(values.size(), -1);
    result.father.assign(values.size(), -1);
    std::vector<int> stack;
    for (int index = 0; index < (int)values.size(); ++index) {
        int last = -1;
        while (!stack.empty() && compare(values[index], values[stack.back()])) {
            last = stack.back();
            stack.pop_back();
        }
        if (!stack.empty()) {
            result.right[stack.back()] = index;
            result.father[index] = stack.back();
        }
        if (last != -1) {
            result.left[index] = last;
            result.father[last] = index;
        }
        stack.push_back(index);
    }
    if (!stack.empty()) result.root = stack.front();
    return result;
}

#endif
