#ifndef MODINT_HPP
#define MODINT_HPP

#include <cassert>
#include <iostream>
#include <type_traits>

#include "types.hpp"

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

	static constexpr T mod() {
		if constexpr (modulo == 0) return dynamic_modulo;
		else return modulo;
	}

	static void set_mod(T value) {
		static_assert(modulo == 0, "set_mod is only available for dynamic ModInt");
		assert(value > 0);
		dynamic_modulo = value;
	}

	static constexpr T norm(T x) {
		assert(mod() > 0);
		x %= mod();
		if (x < 0) x += mod();
		return x;
	}

	ModInt() : number(0) {}
	template <typename U>
	ModInt(U x) : number(norm((T)((Wide)x % mod()))) {}

	T data() const { return number; }
	explicit operator T() const { return number; }
	S operator-() const {
		auto res = ModInt();
		res.number = norm(mod() - number);
		return res;
	}
	S& operator*=(S rhs) {
		return number = (T)((Wide)number * rhs.number % mod()), *this;
	}
	S& operator+=(S rhs) {
		return number = norm(number + rhs.number), *this;
	}
	S& operator-=(S rhs) {
		return number = norm(number - rhs.number), *this;
	}
	S& operator/=(S rhs) {
		assert(rhs.number != 0);
		// WARNING: requires prime modulus
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
