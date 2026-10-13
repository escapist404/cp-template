# MonotoneQueue

单调队列，用于在线维护滑动窗口最值。

```cpp
MonotoneQueue<T, Compare> q;
q.push(index, value);
q.pop_before(first_index);
q.front();
```

默认 `std::less<T>` 保留最大值；传入 `std::greater<T>` 保留最小值。`sliding_window(values, width)` 返回每个窗口的最大值。

每个元素最多进出队一次，复杂度为 $O(n)$。

## 注意

- `index` 必须随时间递增。
- `pop_before(x)` 删除下标小于 `x` 的元素。
- 窗口宽度必须大于 0。
