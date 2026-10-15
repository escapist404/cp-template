#ifndef HASH_HPP
#define HASH_HPP

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <utility>

// ============================================================================
// 【防卡哈希仿函数 CustomHash (hash.hpp)】
// ----------------------------------------------------------------------------
// 结构体签名：struct CustomHash
//   结合 SplitMix64 位混淆器与运行期 high_resolution_clock 纳秒级随机种子，防止针对固定哈希函数的哈希碰撞攻击。
// 成员函数签名与详解：
//   - static std::uint64_t mix(std::uint64_t x)
//     对 64 位无符号整数执行 SplitMix64 黄金比例加法与三次异或移位乘法雪崩混淆。时间复杂度 O(1)。
//   - static std::uint64_t seed()
//     首次调用时获取当前高精度时钟计数并静态缓存，返回全局唯一运行期随机种子。时间复杂度 O(1)。
//   - template <class T> std::size_t operator()(T value) const
//     计算任意可显式转换为 uint64_t 的标量类型（整数、枚举、指针等）的防卡哈希值。时间复杂度 O(1)。
//   - template <class A, class B> std::size_t operator()(const std::pair<A, B>& value) const
//     对二元组 std::pair<A, B> 分别混淆 first 与 second 后异或组合，使 unordered_map<pair<int,int>, V, CustomHash> 直接可用。O(1)。
// ============================================================================

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
