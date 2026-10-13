#ifndef HASH_HPP
#define HASH_HPP

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <utility>

// Usage: unordered_map<long long, int, CustomHash> mp;
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
