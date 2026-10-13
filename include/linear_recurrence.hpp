#ifndef LINEAR_RECURRENCE_HPP
#define LINEAR_RECURRENCE_HPP

#include <cassert>
#include <vector>
#include "types.hpp"

// Field T. Returns c such that s[n] = c[0]*s[n-1] + ... + c[k-1]*s[n-k].
// Empty c is the zero recurrence. O(sequence.size()^2).
template <typename T>
std::vector<T> berlekamp_massey(const std::vector<T>& sequence) {
    std::vector<T> current{T(1)}, previous{T(1)};
    int length = 0, shift = 1;
    T last = T(1);
    for (int n = 0; n < (int)sequence.size(); ++n) {
        T discrepancy = sequence[n];
        for (int i = 1; i <= length; ++i) {
            discrepancy += current[i] * sequence[n - i];
        }
        if (discrepancy == T{}) {
            ++shift;
            continue;
        }
        auto saved = current;
        T ratio = discrepancy / last;
        if (current.size() < previous.size() + shift) {
            current.resize(previous.size() + shift);
        }
        for (int i = 0; i < (int)previous.size(); ++i) {
            current[i + shift] -= ratio * previous[i];
        }
        if (2 * length <= n) {
            length = n + 1 - length;
            previous = std::move(saved);
            last = discrepancy;
            shift = 1;
        } else {
            ++shift;
        }
    }
    current.resize(length + 1);
    current.erase(current.begin());
    for (auto& value : current) {
        value = -value;
    }
    return current;
}

// Ring T; initial has at least k elements. O(k^2 log n). n is zero-based.
template <typename T>
T linear_recurrence_nth(const std::vector<T>& initial, const std::vector<T>& recurrence, u64 n) {
    int k = recurrence.size();
    assert(initial.size() >= recurrence.size());
    if (n < initial.size()) {
        return initial[n];
    }
    if (!k) {
        return T{};
    }
    auto multiply = [&](const std::vector<T>& a, const std::vector<T>& b) {
        std::vector<T> product(2 * k - 1);
        for (int i = 0; i < k; ++i) {
            for (int j = 0; j < k; ++j) {
                product[i + j] += a[i] * b[j];
            }
        }
        for (int i = 2 * k - 2; i >= k; --i) {
            for (int j = 1; j <= k; ++j) {
                product[i - j] += product[i] * recurrence[j - 1];
            }
        }
        product.resize(k);
        return product;
    };
    std::vector<T> result(k), power(k);
    result[0] = T(1);
    if (k == 1) {
        power[0] = recurrence[0];
    } else {
        power[1] = T(1);
    }
    while (n) {
        if (n & 1) {
            result = multiply(result, power);
        }
        power = multiply(power, power);
        n >>= 1;
    }
    T answer{};
    for (int i = 0; i < k; ++i) {
        answer += result[i] * initial[i];
    }
    return answer;
}
#endif
