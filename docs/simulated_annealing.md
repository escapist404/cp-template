# SimulatedAnnealing

随机搜索并最大化评分函数。

## 接口

```cpp
template <typename State, typename Score, typename Neighbor>
State simulated_annealing(
    State initial, Score score, Neighbor neighbor,
    Real initial_temperature = 1E3, Real cooling = 0.995,
    int iterations = 100000, u64 seed = 0);
```

`score(state)` 返回越大越好的分数；`neighbor(state, rng)` 返回候选状态。随机数生成器由模板内部按 `seed` 创建并传给 `neighbor`。

## 使用示例

```cpp
#include "simulated_annealing.hpp"
#include <vector>

int main() {
    std::vector<int> state = {1, 2, 3};
    auto score = [](const std::vector<int>& x) { return x[0] + x[1] + x[2]; };
    auto neighbor = [](const std::vector<int>& x, auto& rng) {
        (void)rng;
        return x;
    };
    auto best = simulated_annealing(state, score, neighbor, 100.0, 0.99, 1000, 42);
    (void)best;
}
```

算法不保证全局最优。`initial_temperature > 0`、`0 < cooling < 1` 且迭代次数非负；复杂度为 `iterations` 次评分和邻域生成。
