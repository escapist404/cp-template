#include <cmath>
#include "../../include/merge_sort.hpp"
#include "../../include/ternary_search.hpp"
#include "../../include/simulated_annealing.hpp"

#include <cassert>
#include <vector>
#include "../../include/binary_search.hpp"
#include "../../include/cdq.hpp"
#include "../../include/mo.hpp"
#include "../../include/overall_binary_search.hpp"
#include "../../include/parallel_binary_search.hpp"

struct Query {
    int l;
    int r;
    int id;
};

int main() {
    assert(first_true(0, 10, [](int x) { return x >= 4; }) == 4);
    assert(last_true(0, 10, [](int x) { return x <= 4; }) == 4);
    std::vector<int> values{1, 2, 1, 3};
    std::vector<Query> queries{{0, 3, 0}, {1, 4, 1}};
    std::vector<int> frequency(4);
    int distinct = 0;
    const auto add = [&](int index, int& state) {
        if (frequency[values[index]]++ == 0) {
            ++state;
        }
    };
    const auto remove = [&](int index, int& state) {
        if (--frequency[values[index]] == 0) {
            --state;
        }
    };
    const auto answer = [](const int& state, const Query&) { return state; };
    assert((mo_algorithm(queries, 4, distinct, add, remove, answer) == std::vector<int>{2, 3}));
    std::vector<int> dp(8, 1);
    cdq_dp(dp, [](auto& data, int left, int middle, int right) {
        for (int i = middle; i < right; ++i) {
            for (int j = left; j < middle; ++j) {
                data[i] += data[j];
            }
        }
    }, [](auto&, int) {});
    for (int i = 0; i < 8; ++i) {
        assert(dp[i] == (1 << i));
    }
    std::vector<int> targets{0, 3, 5};
    int position = 0;
    int sum = 0;
    const auto result = parallel_binary_search(targets, 0, 6,
        [&] { position = 0; sum = 0; },
        [&](int mid) { while (position < mid) { sum += position++; } },
        [&](int target, int) { return sum >= target; });
    assert(result[0] == 0 && result[1] == 3 && result[2] == 4);

    {
    std::vector<int> sorted{4, 1, 3, 2};
    merge_sort(sorted, 0, (int)sorted.size());
    assert(sorted == std::vector<int>({1, 2, 3, 4}));

    assert(std::fabs(real_ternary_search_max(-1, 5,
        [](Real x) { return -(x - 2) * (x - 2); }) - 2) < 1e-6);
    assert(simulated_annealing(7, [](int x) { return x; },
        [](int x, auto&) { return x + 1; }, 1, .5, 0, 1) == 7);
    }
}
