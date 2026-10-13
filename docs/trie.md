# Trie

给定一批字符串，统计其中有多少个字符串以指定字符串为前缀。

## 类型

```cpp
class Trie;
```

非模板类，字符集硬编码为**小写字母 `'a'`–`'z'`**（内部 `child[26]`）。

## 成员函数

### insert

```cpp
void insert(const std::string &s);
```

插入字符串 `s`。

**约束**
- `s` 中只能出现 `'a'`–`'z'`：源码有 `assert(0 <= x && x < 26)`，其中 `x = ch - 'a'`。debug 构建下其他字符会断言失败；release 构建下为越界访问。

**复杂度**
- $O(|s|)$，源码未注明

### query

```cpp
int query(const std::string &s) const;
```

返回**以 `s` 为前缀**的已插入字符串个数（包含长度恰好等于 `|s|` 的那些，即插入过 `s` 本身也计入）。

**约束**
- 同样要求 `s` 只含小写字母

**复杂度**
- $O(|s|)$，源码未注明

## 使用示例

```cpp
#include "trie.hpp"
#include <iostream>

int main() {
    Trie trie;

    trie.insert("apple");
    trie.insert("app");
    trie.insert("application");

    // query 返回的是「以该串为前缀」的数量
    std::cout << trie.query("app") << std::endl;    // 3
    std::cout << trie.query("appl") << std::endl;   // 2 (apple, application)
    std::cout << trie.query("apple") << std::endl;  // 2 (apple, application 以 apple 为前缀)

    std::cout << trie.query("banana") << std::endl; // 0

    return 0;
}
```

### 判断某串是否被插入过

`query` 计的是前缀数，含更长串，因此**无法直接区分「插入过 s」与「插入过以 s 为前缀的更长串」**。若需要精确判定，可用如下写法：

```cpp
trie.insert("app");
trie.insert("apple");

trie.query("app")   == 2;  // 是前缀，但不知道 "app" 本身是否被插入
trie.query("apple") == 1;  // 长度为 |s| 的前缀数恰为 1 ⟺ 插入过 "apple"
```

即：`query(s)` 等于 1 并不意味着只插入过 `s`，但若 `query(s)` 中所有匹配串长度都恰好是 `|s|`，则说明插入过 `s`。

## 相关算法

- [AhoCorasick](ac_automaton.md) —— Trie + fail 指针，多模式串匹配
- [SuffixAutomaton](suffix_automaton.md) —— 后缀自动机
- [PalindromicTree](palindromic_tree.md) —— 回文树
