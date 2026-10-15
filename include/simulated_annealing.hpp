#ifndef SIMULATED_ANNEALING_HPP
#define SIMULATED_ANNEALING_HPP

#include <cassert>
#include <cmath>
#include <cstdint>
#include <random>
#include <utility>

#include "types.hpp"

// ============================================================================
// 【模拟退火启发式搜索 Simulated Annealing (simulated_annealing.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   template <typename State, typename Score, typename Neighbor>
//   State simulated_annealing(
//       State initial, Score score, Neighbor neighbor,
//       Real initial_temperature = 1E3, Real cooling = 0.995,
//       int iterations = 100000, u64 seed = 0)
// 回调签名与方向约定（固定为【最大化】score(state)）：
//   - `ScoreType score(const State& s)`                         ：评估状态得分（越大越优；若求最小化请返回负代价）；
//   - `State neighbor(const State& cur, std::mt19937_64& rng)`  ：生成随机邻近状态。
//   返回整个退火过程中访问过的最高分状态 best。
// ============================================================================

// template <typename State, typename Score, typename Neighbor> State simulated_annealing( — 功能：State simulated_annealing(
template <typename State, typename Score, typename Neighbor>
State simulated_annealing(State initial, Score score, Neighbor neighbor,
                          Real initial_temperature = 1E3, Real cooling = 0.995,
                          int iterations = 100000, u64 seed = 0) {
    assert(initial_temperature > 0 && 0 < cooling && cooling < 1 && iterations >= 0);

    std::mt19937_64 rng(seed);
    std::uniform_real_distribution<Real> uniform(0.0, 1.0);

    State current = initial;
    State best = initial;

    auto current_score = score(current);
    auto best_score = current_score;

    for (double temperature = initial_temperature;
         iterations-- > 0;
         temperature *= cooling) {
        State candidate = neighbor(current, rng);
        auto candidate_score = score(candidate);
        const Real delta = (Real)(candidate_score) -
                             (Real)(current_score);

        if (delta >= 0 || uniform(rng) < std::exp(delta / temperature)) {
            current = std::move(candidate);
            current_score = candidate_score;
        }

        if (current_score > best_score) {
            best = current;
            best_score = current_score;
        }
    }
    return best;
}

#endif
