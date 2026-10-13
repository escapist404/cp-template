#ifndef CARTESIAN_TREE_HPP
#define CARTESIAN_TREE_HPP

#include <functional>
#include <utility>
#include <vector>

// In-order traversal follows the input order, and Compare defines the heap
// order. Equal values keep the earlier index above the later.
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
