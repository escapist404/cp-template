# AhoCorasick

给定一批模式串和一段文本，统计每个模式串在文本中出现了多少次。

设第 `i` 个模式串为 $P_i$，文本为 $T$，`query(T)[i]` 返回

$$
\left|\left\{j\mid 0\le j\le |T|-|P_i|,\ T[j:j+|P_i|)=P_i\right\}\right|,
$$

即 `P_i` 在 `T` 中的出现次数，包含重叠出现。重复插入同一个模式串会得到不同编号，但对应计数相同。

## 算法状态

- Trie 节点表示某个模式串前缀。
- `fail(v)` 指向该前缀的最长真后缀对应的节点。
- 扫描文本时，当前节点表示“已经读入的文本的最长后缀，且它是某个模式前缀”。
- `build()` 会补全转移；`query()` 统计扫描过程中各节点的访问次数，再沿 `fail` 关系累加，从而得到每个模式串的出现次数。

## 类型

```cpp
class AhoCorasick;
```

字符集**固定为 26 个小写字母** `'a'`–`'z'`。

## 成员函数

### insert

```cpp
int insert(const std::string &s);
```

插入一个模式串。

**返回值**
- 该模式串的编号，**从 0 开始**（即第 $i$ 次调用返回 $i-1$）

**约束**
- 源码有 `assert(!built)`：**`build()` 之后不允许再 `insert`**
- 源码有 `assert(0 <= x && x < 26)`，其中 `x = c - 'a'`：字符串必须只含小写字母

**复杂度**
- $O(|s|)$

### build

```cpp
void build();
```

构建 fail 指针与转移。

**行为**
- 幂等（内部 `if (built) return;`），但**不清空旧状态**
- 会把根节点缺失的转移改为**自环**（`trie[0].next[c] = 0`），并把所有缺失转移补全为 fail 转移——即 trie 结构被**就地改写为自动机**，`next` 不再有 `-1`

**约束**
- 必须在所有 `insert` 之后调用一次，且在 `query` 之前

### query

```cpp
std::vector<int> query(const std::string &text) const;
```

统计每个模式串在 `text` 中的出现次数。

**返回值**
- 长度等于模式串个数的数组，第 $i$ 项为第 $i$ 次 `insert` 的模式串在 `text` 中的出现次数（**含重叠**）

**约束**
- 源码有 `assert(built)`：必须先 `build()`
- 同样要求 `text` 只含小写字母

**复杂度**
- 设所有模式串总长度为 $S$，模式串数量为 $m$。
- `insert` 总计 $O(S)$，`build` 为 $O(26S)$，`query` 为 $O(|text| + S + m)$。

## 使用示例

```cpp
#include "ac_automaton.hpp"
#include <iostream>

int main() {
    AhoCorasick ac;

    int id_she = ac.insert("she");   // 编号 0
    int id_he  = ac.insert("he");    // 编号 1
    int id_her = ac.insert("her");   // 编号 2

    ac.build();

    auto counts = ac.query("ushershe");

    // counts == {2, 2, 1}
    // "she" 出现 2 次，"he" 出现 2 次，"her" 出现 1 次
    for (int c : counts) std::cout << c << ' ';
    std::cout << '\n';

    return 0;
}
```

### 只关心某个模式串

`insert` 的返回值就是索引，保存下来即可：

```cpp
int id = ac.insert("abc");
ac.build();
int occurrences = ac.query(text)[id];
```

## 相关文档

- [Trie](trie.md) —— 单模式前缀统计
- [KMP](kmp.md) —— 单模式串匹配
- [SuffixAutomaton](suffix_automaton.md) —— 后缀自动机
