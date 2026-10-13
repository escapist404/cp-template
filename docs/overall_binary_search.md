# OverallBinarySearch

在一串按时间排列的事件上执行整体二分。每个事件会被路由到左半或右半，回滚回调负责恢复临时状态。

## 接口

```cpp
template <typename Event, typename Integer, typename Route,
          typename Rollback, typename Finish>
void overall_binary_search(std::vector<Event> events, Integer low, Integer high,
                           Route route, Rollback rollback, Finish finish);
```

`route(event, mid)` 返回是否进入左半；`rollback()` 在每次分区后恢复状态；`finish(event, value)` 记录答案。答案域为闭区间 `[low, high]`。

## 使用示例

```cpp
#include "overall_binary_search.hpp"
#include <vector>

struct Event { int value, answer = -1; };

int main() {
    std::vector<Event> events = {{3}, {5}, {2}};
    int current = 0;
    auto route = [&](Event& event, int mid) {
        current = mid;
        return event.value <= current;
    };
    auto rollback = [&] { current = 0; };
    auto finish = [&](Event& event, int value) { event.answer = value; };
    overall_binary_search(events, 0, 10, route, rollback, finish);
}
```
