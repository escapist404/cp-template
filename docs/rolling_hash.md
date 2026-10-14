# RollingHash

预处理一段字符串后，快速判断任意两个等长子串是否相等。

## 类型

```cpp
class RollingHash;
```

## 构造函数

```cpp
RollingHash(const std::string &s);
```

预处理 `s` 的哈希与幂表。

**复杂度**
- $O(|s|)$

## 成员函数

### get

```cpp
u64 get(const int l, const int r) const;
```

返回**半开区间 `[l, r)`** 的子串哈希。

**注意是半开区间**，与很多「闭区间」习惯不同。

**约束**
- 需要 $0 \le l \le r \le |s|$，无 `assert`，越界为 UB

### equal

```cpp
bool equal(const int l1, const int r1, const int l2, const int r2) const;
```

判断两个**半开区间**子串是否相等。先比长度，再比哈希值。

**复杂度**
- $O(1)$

## 使用示例

```cpp
#include "rolling_hash.hpp"
#include <iostream>

int main() {
    std::string s = "abracadabra";
    RollingHash hash(s);

    std::cout << hash.equal(0, 4, 7, 11) << '\n';   // 1, "abra" == "abra"
    std::cout << hash.equal(0, 4, 0, 3) << '\n';    // 0, 长度不同

    return 0;
}
```

### 在更长串中找重复子串

```cpp
// 判断长度 L 的子串是否重复出现
bool has_duplicate_of_length(const RollingHash &hash, int n, int L) {
    std::vector<u64> values;
    for (int i = 0; i + L <= n; ++i) values.push_back(hash.get(i, i + L));
    std::sort(values.begin(), values.end());
    return std::adjacent_find(values.begin(), values.end()) != values.end();
}
```

## 哈希参数与安全性

- 使用 `u64` 的**自然溢出**（即模 $2^{64}$），**没有显式模数**
- `base = 911382323`，是 `static constexpr`
- **单一 base + 自然溢出，无抗碰撞保证**；源码未提供第二模数或随机化机制

### 补充说明与使用约定

- **`base` 不可修改**（`static constexpr`，且是私有成员）。这意味着使用本模板的提交代码的哈希取值是**公开且固定**的——在多测 / Hack 型比赛中（如 Codeforces），这是可被针对性构造碰撞的。需要抗 Hack 时请自行改为随机 base（例如运行时从 `chrono` 取值）。
- `get` 无边界检查，`l > r` 或 `r > |s|` 直接 UB。
- 字符映射为 `(unsigned char)s[i] + 1`，因此支持任意字节值（含 `'\0'` 与高位字节），不限于可打印字符。
- 单个 `u64` 的哈希碰撞概率约为 $1/2^{64}$，对多数题目足够，但**可以被恶意构造**。
- 没有提供「拼接两个哈希」或「求最长公共前缀」的接口，需自行用二分 + `equal`。

## 相关文档

- [SuffixArray](suffix_array.md) —— 无碰撞风险的后缀比较与 LCP 查询
- [Z 算法](z_algo.md)、[KMP](kmp.md) —— 精确匹配
- [Manacher](manacher.md) —— 回文
