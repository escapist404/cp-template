# SuffixAutomaton

在线追加字符串字符，并查询当前字符串的子串存在性、不同子串数量和各状态代表的出现次数。

## 模板参数

```cpp
template <typename Symbol = char, typename Compare = std::less<Symbol>>
class SuffixAutomaton;
```

- `Symbol`：字符（转移键）类型，默认 `char`
- `Compare`：内部 `std::map` 的比较器，默认 `std::less<Symbol>`

由于用 `std::map` 而非定长数组，**字符集大小无限制**。

## 嵌套类型

```cpp
struct Node {
    int length = 0;
    int link = -1;
    std::map<Symbol, int, Compare> next;
    long long terminals = 0;
};
```

## 构造函数

默认构造，根节点为编号 0（`length = 0`，`link = -1`）。

## 成员函数

### add

```cpp
int add(const Symbol &symbol);
```

追加一个字符。

**返回值**
- 新建状态 `cur` 的编号（**即使发生 clone，返回的仍是 `cur`，不是 clone 的编号**）

**行为**
- 若此前未 `build`，则直接在现有状态上继续追加

**复杂度**
- 均摊 $O(\log \Sigma)$，源码未注明

### build

```cpp
template <typename Sequence>
void build(const Sequence &text);
```

**重置**（清空全部状态、`last = 0`）后逐个 `add`。

**注意**
- 会**清空历史状态**，不能用来「继续追加」
- 返回 `void`，不返回状态数
- `Sequence` 需可范围遍历，元素需可隐式转换为 `Symbol`

### contains

```cpp
template <typename Sequence>
bool contains(const Sequence &text) const;
```

判断 `text` 是否为子串。从根沿转移走，全部命中返回 `true`；空序列返回 `true`。

**复杂度**
- $O(|\mathrm{text}| \log \Sigma)$

### distinct_substrings

```cpp
long long distinct_substrings() const;
```

返回不同子串的个数，由

$$\sum_{i \ge 1} \bigl(\mathrm{length}_i - \mathrm{length}_{\mathrm{link}_i}\bigr)$$

计算。

### occurrences

```cpp
std::vector<long long> occurrences() const;
```

返回每个状态的**出现次数**（该状态代表的所有子串的出现次数）。

**返回值**
- 长度等于状态数的数组，下标即状态编号
- 值为 `terminals` 沿 link 树累加后的结果

**行为**
- 每次调用**新建向量**（源码注释：A fresh vector each call, so propagation is repeatable after more adds.），因此 `add` 之后可以再次调用
- 内部按 `length` **降序**排序后累加

### states

```cpp
const std::vector<Node> &states() const;
```

返回内部状态数组的常引用。

**注意**：返回引用，若随后 `add` / `build` 导致 `std::vector` 重分配，**原引用失效**。

## 使用示例

```cpp
#include "suffix_automaton.hpp"
#include <iostream>

int main() {
    SuffixAutomaton<> sam;
    sam.build(std::string("ababa"));

    std::cout << sam.contains(std::string("aba")) << '\n';   // 1
    std::cout << sam.contains(std::string("baa")) << '\n';   // 0
    std::cout << sam.contains(std::string("")) << '\n';      // 1

    std::cout << sam.distinct_substrings() << '\n';          // 9

    // 出现次数需要先调用 occurrences()
    auto occ = sam.occurrences();
    // occ[state] = 该状态代表子串的出现次数

    return 0;
}
```

### 用 add 在线追加

```cpp
SuffixAutomaton<> sam;
int last = 0;
for (char c : std::string("banana")) {
    last = sam.add(c);   // 返回新建状态 cur 的编号
}
// 此时可直接用 sam 查询，无需 build
```

### 统计出现次数最多的子串次数

```cpp
auto occ = sam.occurrences();
long long best = 0;
for (int i = 1; i < (int)occ.size(); ++i) {
    best = std::max(best, occ[i]);
}
```

## 前置条件与实现约定

- **`terminals` 不是最终出现次数**。`add` 中新建的 `cur` 置 1、clone 置 0，它只表示「该状态是否对应某个前缀的 endpos 代表」。要得到出现次数**必须调用 `occurrences()`** 做 link 树传播。
- **`build` 会清空历史**：它内部先 `nodes.assign(1, Node{})`。想连续追加多个字符串的字符只能用 `add`。
- `add` 返回的是 `cur` 而非 clone 编号——若你在 clone 发生时想拿 clone 的编号，需要自己从 `states()` 里找。
- `occurrences()` 内部按 `length` 降序排序，长度相同的状态之间相对顺序未指定（源码未说明），但不影响结果。
- `states()` 返回引用，`add` / `build` 后立即失效，不要长期持有。
- 根节点是 0，`link[0] = -1`，遍历 link 链时要判 `-1` 终止。
- `distinct_substrings()` 源码未注明是否需要先 `build` / `add`；按实现它只读取当前状态集合，空对象返回 0。

## 相关文档

- [PalindromicTree](palindromic_tree.md) —— 回文树，同为增量式自动机
- [SuffixArray](suffix_array.md) —— 离线构建，支持区间 LCP
- [AhoCorasick](ac_automaton.md) —— 多模式串匹配
- [Trie](trie.md) —— 前缀树
