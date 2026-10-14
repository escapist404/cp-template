# PalindromicTree

给定一个字符串，在线追加字符，并维护当前前缀中出现过的不同回文子串及其出现次数。

## 模板参数

```cpp
template <typename Symbol = char, typename Compare = std::less<Symbol>>
class PalindromicTree;
```

同 [SuffixAutomaton](suffix_automaton.md)，内部用 `std::map`，字符集大小无限制。

## 嵌套类型

```cpp
struct Node {
    int length = 0;
    int link = 0;      // 注意：默认值为 0，与 SuffixAutomaton 的 -1 不同
    std::map<Symbol, int, Compare> next;
    long long terminals = 0;
};
```

## 节点编号约定（重要）

| 编号 | 含义 |
|------|------|
| 0 | **奇根**，`length = -1` |
| 1 | **偶根**，`length = 0`，`link = 0` |
| $\ge 2$ | 实际回文串对应的节点 |

`last` 初值为 1，因此**空串时 `longest_suffix() == 0`**。

## 构造函数

```cpp
PalindromicTree();
```

初始化两个根节点，`last = 1`。

## 成员函数

### add

```cpp
int add(const Symbol &symbol);
```

追加一个字符。

**返回值**
- 返回 **`last`**，即新文本串的**最长回文后缀所对应的节点编号**
- 若该回文串已存在，返回的是已存在的节点编号；新建节点的编号可能不等于返回值

**副作用**
- `nodes[last].terminals` 自增

### build

```cpp
template <typename Sequence>
void build(const Sequence &sequence);
```

**整体重置**（`*this = PalindromicTree{};`）后逐个 `add`。

**注意**：重置会把 `compare` 也恢复为默认构造值——若 `Compare` 带状态则会丢失。

### distinct_palindromes

```cpp
int distinct_palindromes() const;
```

返回不同回文子串的个数，即 `nodes.size() - 2`（扣除两个根）。

### longest_suffix

```cpp
int longest_suffix() const;
```

返回当前文本串最长回文后缀的长度，即 `nodes[last].length`。空串时为 0。

### occurrences

```cpp
std::vector<long long> occurrences() const;
```

返回每个节点的出现次数。

**返回值**
- 长度等于节点数的数组，下标即节点编号
- 对 $i \ge 2$ 的节点按 `length` 降序把值累加到 `nodes[u].link`
- 节点 0、1 **不参与传播**，其值保持初始的 `terminals`（实现中恒为 0）

### states

```cpp
const std::vector<Node> &states() const;
```

返回内部节点数组的常引用。同样会在后续 `add` / `build` 后失效。

## 使用示例

```cpp
#include "palindromic_tree.hpp"
#include <iostream>
#include <string>

int main() {
    PalindromicTree<> pt;
    pt.build(std::string("ababa"));

    std::cout << pt.distinct_palindromes() << '\n';   // 不同回文子串个数
    std::cout << pt.longest_suffix() << '\n';         // 3 ("aba")

    auto occ = pt.occurrences();
    // occ[node] = 该回文串的出现次数

    return 0;
}
```

### 每个回文串对应的长度与出现次数

```cpp
const auto &nodes = pt.states();
auto occ = pt.occurrences();

// 跳过两个根节点
for (int i = 2; i < (int)nodes.size(); ++i) {
    std::cout << "len=" << nodes[i].length
              << " occurrences=" << occ[i] << '\n';
}
```

### 补充说明与使用约定

- **`link` 的默认值是 0**（与 `SuffixAutomaton` 的 `-1` 不同）。遍历 link 链时，奇根（节点 0）的 `link` 保持自指 0，`suffix()` 在节点 0 处必然返回，不会死循环。
- **两个根节点的语义容易搞反**：0 是奇根（`length = -1`），1 是偶根（`length = 0`）。
- `add` 返回的是 `last`，不是新建节点的编号。若新回文串已存在，不会有新节点。
- `terminals` 与 SAM 一样**不是最终出现次数**，必须调用 `occurrences()` 传播。
- `build()` 用的是整体赋值重置，**会把 `Compare` 也一起重置**——自定义比较器带状态时会丢。
- `add` 内部使用 `nodes[...].next.at(symbol)`，缺键时会抛 `std::out_of_range`（源码未说明其前置条件）。
- `occurrences()` 中节点 0、1 不参与传播，`result[0]` / `result[1]` 恒为 0，不要把它们当作回文串使用。

## 相关文档

- [SuffixAutomaton](suffix_automaton.md) —— 后缀自动机
- [Manacher](manacher.md) —— 最长回文子串（线性，但只给长度）
- [RollingHash](rolling_hash.md) —— 哈希判回文
