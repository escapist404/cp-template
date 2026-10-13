# LinearRecurrence

提供 Berlekamp–Massey 和线性递推第 `n` 项计算：

```cpp
auto recurrence = berlekamp_massey(sequence);
auto value = linear_recurrence_nth(initial, recurrence, n);
```

常用于从前若干项恢复线性递推并快速计算大下标项。典型复杂度为 $O(k^2\log n)$。
