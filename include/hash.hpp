#ifndef HASH_HPP
#define HASH_HPP

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <utility>

// 【防卡哈希仿函数 (hash.hpp)】
// - 结合 64 位 SplitMix64 混淆与高精度运行期时钟随机种子，防止 unordered_map / unordered_set 被构造数据卡至 O(n)
// - 原生支持所有可转为 uint64_t 的标量类型以及 std::pair<A, B>
// - 用法: std::unordered_map<long long, int, CustomHash> mp;

struct CustomHash {
    static std::uint64_t mix(std::uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    static std::uint64_t seed() {
        static const auto value = std::chrono::steady_clock::now().time_since_epoch().count();
        return (std::uint64_t)value;
    }

    template <class T>
    std::size_t operator()(T value) const {
        return (std::size_t)mix((std::uint64_t)value + seed());
    }

    template <class A, class B>
    std::size_t operator()(const std::pair<A, B>& value) const {
        return (std::size_t)mix((std::uint64_t)value.first + seed()) ^
               (std::size_t)mix((std::uint64_t)value.second + 0x9e3779b97f4a7c15);
    }
};

#endif
