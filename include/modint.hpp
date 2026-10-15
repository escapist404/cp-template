#ifndef MODINT_HPP
#define MODINT_HPP

#include <cassert>
#include <iostream>
#include <type_traits>

#include "types.hpp"

// ============================================================================
// 【编译期静态模数与运行期动态模数自动取模整数类 ModInt (modint.hpp)】
// ----------------------------------------------------------------------------
// 1. 通用快速幂辅助函数签名：
//    - template <typename S, typename T> S power(S begin, T base, i64 exp)
//      计算 begin * (base ^ exp)（要求 exp >= 0），支持任意定义了 *= 的类型（如 ModInt、Matrix）。时间复杂度 O(log exp)。
// 2. 自动取模整数结构体签名：template <typename T, T modulo = 0> struct ModInt
//    - T：底层有符号整型（如 int 或 i64）；内部乘法自动提升至 Wide（int 提升为 long long，i64 提升为 __int128_t）防止溢出。
//    - modulo > 0 时为【编译期静态模数】（如 ModInt<int, 998244353>）；
//    - modulo == 0 时为【运行期动态模数】（如 ModInt<int, 0>，使用前必须先调用 ModInt<int, 0>::set_mod(p)）。
// 静态函数、构造函数与成员函数签名详解：
//   - static constexpr T mod()
//     功能：返回当前生效的正模数（静态模数返回 modulo，动态模数返回 dynamic_modulo）。O(1)。
//   - static void set_mod(T value)
//     功能：【仅当 modulo == 0 时可用】设置动态模数 dynamic_modulo = value（要求 value > 0）。O(1)。
//   - static constexpr T norm(T x)
//     功能：将整数 x 规范化到标准最小非负剩余系 [0, mod()) 内。O(1)。
//   - ModInt() / template <typename U> ModInt(U x)
//     功能：默认构造为 0，或将任意整型 x 取模规范化后构造 ModInt。O(1)。
//   - T data() const / explicit operator T() const
//     功能：获取底层区间 [0, mod()) 内的代表元整数值。O(1)。
//   - S operator-() const / S& operator+=(S rhs) / S& operator-=(S rhs) / S& operator*=(S rhs)
//     功能：模意义下的取反、加、减、乘复合赋值。O(1)。
//   - S& operator/=(S rhs)
//     功能：基于费马小定理乘以 power(S(1), rhs, mod() - 2)（【前置条件】要求 mod() 为素数且 rhs.data() != 0）。O(log mod)。
//   - 友元运算符：operator+, operator-, operator*, operator/, operator==, operator!=, operator<, operator<<, operator>>。
// ============================================================================

// - template <typename S, typename T> S power(S begin, T base, i64 exp) 计算 begin * (base ^ exp)（要求 exp >= 0），支持任意定义了 *= 的类型（如 ModInt、Matrix）。时间复杂度 O(log exp)。 — 功能：template <typename S, typename T> S power(S begin, T base, i64 exp)
template <typename S, typename T>
S power(S begin, T base, i64 exp) {
	S current = begin;
	while (exp) {
		if (exp & 1) current *= base;
		base *= base;
		exp >>= 1;
	}
	return current;
}

template <typename T, T modulo = 0>
struct ModInt {
	static_assert(std::is_integral<T>::value && std::is_signed<T>::value);
	using S = ModInt<T, modulo>;
	using Wide = std::conditional_t<sizeof(T) <= sizeof(int), long long, __int128_t>;
	inline static T dynamic_modulo = 0;
	T number;

	// static constexpr T mod() — 功能：返回当前生效的正模数（静态模数返回 modulo，动态模数返回 dynamic_modulo）。O(1)。
	static constexpr T mod() {
		if constexpr (modulo == 0) return dynamic_modulo;
		else return modulo;
	}

	// static void set_mod(T value) — 功能：【仅当 modulo == 0 时可用】设置动态模数 dynamic_modulo = value（要求 value > 0）。O(1)。
	static void set_mod(T value) {
		static_assert(modulo == 0, "set_mod is only available for dynamic ModInt");
		assert(value > 0);
		dynamic_modulo = value;
	}

	// static constexpr T norm(T x) — 功能：将整数 x 规范化到标准最小非负剩余系 [0, mod()) 内。O(1)。
	static constexpr T norm(T x) {
		assert(mod() > 0);
		x %= mod();
		if (x < 0) x += mod();
		return x;
	}

	// ModInt() — 功能：默认构造为 0，或将任意整型 x 取模规范化后构造 ModInt。O(1)。
	ModInt() : number(0) {}
	// template <typename U> ModInt(U x) — 功能：默认构造为 0，或将任意整型 x 取模规范化后构造 ModInt。O(1)。
	template <typename U>
	ModInt(U x) : number(norm((T)((Wide)x % mod()))) {}

	T data() const { return number; }
	explicit operator T() const { return number; }
	// S operator-() const — 功能：模意义下的取反、加、减、乘复合赋值。O(1)。
	S operator-() const {
		auto res = ModInt();
		res.number = norm(mod() - number);
		return res;
	}
	// S& operator*=(S rhs) — 功能：模意义下的取反、加、减、乘复合赋值。O(1)。
	S& operator*=(S rhs) {
		return number = (T)((Wide)number * rhs.number % mod()), *this;
	}
	// S& operator+=(S rhs) — 功能：模意义下的取反、加、减、乘复合赋值。O(1)。
	S& operator+=(S rhs) {
		return number = norm(number + rhs.number), *this;
	}
	// S& operator-=(S rhs) — 功能：模意义下的取反、加、减、乘复合赋值。O(1)。
	S& operator-=(S rhs) {
		return number = norm(number - rhs.number), *this;
	}
	// S& operator/=(S rhs) — 功能：基于费马小定理乘以 power(S(1), rhs, mod() - 2)（【前置条件】要求 mod() 为素数且 rhs.data() != 0）。O(log mod)。
	S& operator/=(S rhs) {
		assert(rhs.number != 0);
		// 注意：费马小定理求逆元要求模数 MOD 为素数且 value != 0。
		return *this *= power(S(1), rhs, mod() - 2);
	}

	friend S operator+(S lhs, S rhs) { return lhs += rhs; }
	friend S operator-(S lhs, S rhs) { return lhs -= rhs; }
	friend S operator*(S lhs, S rhs) { return lhs *= rhs; }
	friend S operator/(S lhs, S rhs) { return lhs /= rhs; }
	friend bool operator==(S lhs, S rhs) { return lhs.number == rhs.number; }
	friend bool operator!=(S lhs, S rhs) { return !(lhs == rhs); }

	friend std::ostream& operator<<(std::ostream& out, S a) {
		return out << (T)a;
	}
	friend std::istream& operator>>(std::istream& in, S& a) {
		T v;
		in >> v;
		a = ModInt(v);
		return in;
	}
};

#endif
