# MinimalRotation

```cpp
int start = minimal_rotation(s);
std::string t = minimal_rotation_string(s);
std::string u = maximal_rotation_string(s);
```

使用 Booth 算法在线性时间内求字符串的最小循环表示。`minimal_rotation` 返回起点下标；空字符串返回 0。复杂度为 $O(n)$。
