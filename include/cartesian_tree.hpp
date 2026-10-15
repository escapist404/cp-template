#ifndef CARTESIAN_TREE_HPP
#define CARTESIAN_TREE_HPP

#include <functional>
#include <utility>
#include <vector>

// ============================================================================
// 【笛卡尔树 Cartesian Tree (cartesian_tree.hpp)】
// ----------------------------------------------------------------------------
// 结果结构体签名：
//   struct CartesianTreeResult {
//       int root = -1;                  // 整棵笛卡尔树的根节点 0-based 下标（空数组时为 -1）
//       std::vector<int> left, right;   // left[i], right[i] 为节点 i 的左、右儿子下标（无则为 -1）
//       std::vector<int> father;        // father[i] 为节点 i 的父节点下标（根节点为 -1）
//   };
// 构建函数签名：
//   template <typename T, typename Compare = std::less<T>>
//   CartesianTreeResult cartesian_tree(const std::vector<T>& values, Compare compare = Compare{})
// 功能与约定：
//   - 使用单调栈在严格线性 O(n) 时间内构造序列 values 的笛卡尔树，中序遍历严格还原原数组 0-based 下标 0..n-1。
//   - 默认 Compare = std::less<T> 构造【小根笛卡尔树】（父节点值 <= 子节点值；相等元素按下标较小者作为祖先）。
//   - 若需构造【大根笛卡尔树】，请传入 std::greater<T>{}。
// ============================================================================

struct CartesianTreeResult {
    int root = -1;
    std::vector<int> left, right, father;
};

// template <typename T, typename Compare = std::less<T>> CartesianTreeResult cartesian_tree(const std::vector<T>& values, Compare compare = Compare{}) — 功能：CartesianTreeResult cartesian_tree(const std::vector<T>& values, Compare compare = Compare{})
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
