#ifndef BIGINT_HPP
#define BIGINT_HPP

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>

#include "binary_search.hpp"
#include "polynomial.hpp"
#include "types.hpp"

// Arbitrary-precision signed integer (base 10^9) with NTT-accelerated multiplication.

class BigInt {
public:
	static constexpr int base = 1'000'000'000;
	static constexpr int width = 9;

	BigInt(i64 value = 0) { *this = value; }
	BigInt(const std::string& value) { read(value); }

	BigInt& operator=(i64 value) {
		sign = value < 0 ? -1 : 1;
		u64 magnitude = value < 0
			? (u64)(-(value + 1)) + 1
			: (u64)(value);
		digits.clear();
		while (magnitude) {
			digits.push_back((int)(magnitude % base));
			magnitude /= base;
		}
		return *this;
	}

	BigInt& read(const std::string& value) {
		digits.clear();
		sign = 1;
		int begin = 0;
		if (!value.empty() && (value[0] == '-' || value[0] == '+')) {
			sign = value[0] == '-' ? -1 : 1;
			begin = 1;
		}
		for (int end = (int)(value.size()); end > begin; end -= width) {
			const int start = std::max(begin, end - width);
			digits.push_back(std::stoi(value.substr(start, end - start)));
		}
		trim();
		return *this;
	}

	bool zero() const { return digits.empty(); }
	i64 to_i64() const {
		u64 magnitude = 0;
		for (int i = (int)(digits.size()) - 1; i >= 0; --i) {
			const u64 limit = sign < 0
				? (u64)(std::numeric_limits<i64>::max()) + 1
				: (u64)(std::numeric_limits<i64>::max());
			assert(magnitude <= (limit - digits[i]) / base);
			magnitude = magnitude * base + digits[i];
		}
		const u64 limit = sign < 0
			? (u64)(std::numeric_limits<i64>::max()) + 1
			: (u64)(std::numeric_limits<i64>::max());
		assert(magnitude <= limit);
		if (sign < 0 && magnitude == limit) return std::numeric_limits<i64>::min();
		return sign < 0 ? -(i64)(magnitude) : (i64)(magnitude);
	}
	BigInt abs() const {
		BigInt result = *this;
		result.sign = 1;
		return result;
	}

	BigInt operator-() const {
		BigInt result = *this;
		if (!result.zero()) result.sign = -result.sign;
		return result;
	}

	BigInt& operator+=(const BigInt& other) {
		if (sign == other.sign) {
			add_abs(other);
		} else if (compare_abs(other) >= 0) {
			sub_abs(other);
		} else {
			BigInt result = other;
			result.sub_abs(*this);
			*this = result;
		}
		return *this;
	}

	BigInt& operator-=(const BigInt& other) { return *this += -other; }

	BigInt& operator*=(const BigInt& other) {
		if (digits.empty() || other.digits.empty()) return *this = BigInt();
		BigInt result;
		result.sign = sign * other.sign;
		if (std::min(digits.size(), other.digits.size()) >= 64 &&
				ceil_pow2((int)(digits.size() + other.digits.size() - 1)) <= (1 << 21)) {
			result.digits = multiply_ntt(digits, other.digits);
			result.trim();
			return *this = result;
		}
		result.digits.assign(digits.size() + other.digits.size(), 0);
		for (std::size_t i = 0; i < digits.size(); ++i) {
			i64 carry = 0;
			for (std::size_t j = 0; j < other.digits.size() || carry; ++j) {
				const i64 current = result.digits[i + j] + carry +
						(i64)(digits[i]) * (j < other.digits.size() ? other.digits[j] : 0);
				result.digits[i + j] = (int)(current % base);
				carry = current / base;
			}
		}
		result.trim();
		return *this = result;
	}

	BigInt& operator*=(i64 value) { return *this *= BigInt(value); }

	friend BigInt operator+(BigInt lhs, const BigInt& rhs) { return lhs += rhs; }
	friend BigInt operator-(BigInt lhs, const BigInt& rhs) { return lhs -= rhs; }
	friend BigInt operator*(BigInt lhs, const BigInt& rhs) { return lhs *= rhs; }
	friend BigInt operator*(BigInt lhs, i64 rhs) { return lhs *= rhs; }

	friend bool operator==(const BigInt& lhs, const BigInt& rhs) {
		return lhs.sign == rhs.sign && lhs.digits == rhs.digits;
	}
	friend bool operator!=(const BigInt& lhs, const BigInt& rhs) { return !(lhs == rhs); }
	friend bool operator<(const BigInt& lhs, const BigInt& rhs) {
		if (lhs.sign != rhs.sign) return lhs.sign < rhs.sign;
		const int comparison = lhs.compare_abs(rhs);
		return lhs.sign == 1 ? comparison < 0 : comparison > 0;
	}
	friend bool operator>(const BigInt& lhs, const BigInt& rhs) { return rhs < lhs; }
	friend bool operator<=(const BigInt& lhs, const BigInt& rhs) { return !(rhs < lhs); }
	friend bool operator>=(const BigInt& lhs, const BigInt& rhs) { return !(lhs < rhs); }

	friend std::pair<BigInt, BigInt> divmod(const BigInt& dividend, const BigInt& divisor) {
		assert(!divisor.zero());
		BigInt divisor_abs = divisor.abs();
		BigInt quotient, remainder;
		quotient.sign = dividend.sign * divisor.sign;
		quotient.digits.assign(dividend.digits.size(), 0);
		for (int i = (int)(dividend.digits.size()) - 1; i >= 0; --i) {
			remainder *= base;
			remainder += dividend.digits[i];
			const i64 digit = last_true<i64>(0, base,
				[&](i64 middle) { return divisor_abs * middle <= remainder; });
			quotient.digits[i] = (int)(digit);
			remainder -= divisor_abs * digit;
		}
		quotient.trim();
		remainder.sign = dividend.sign;
		remainder.trim();
		return {quotient, remainder};
	}

	friend BigInt operator/(const BigInt& lhs, const BigInt& rhs) { return divmod(lhs, rhs).first; }
	friend BigInt operator%(const BigInt& lhs, const BigInt& rhs) { return divmod(lhs, rhs).second; }

	friend std::ostream& operator<<(std::ostream& out, const BigInt& value) {
		if (value.sign < 0 && !value.zero()) out << '-';
		if (value.zero()) return out << 0;
		out << value.digits.back();
		const char old_fill = out.fill('0');
		for (int i = (int)(value.digits.size()) - 2; i >= 0; --i) {
			out.width(width);
			out << value.digits[i];
		}
		out.fill(old_fill);
		return out;
	}

	friend std::istream& operator>>(std::istream& in, BigInt& value) {
		std::string input;
		in >> input;
		if (in) value.read(input);
		return in;
	}

private:
	std::vector<int> digits;
	int sign = 1;

	static i64 power_mod(i64 a, i64 e, i64 mod) {
		i64 result = 1;
		while (e) {
			if (e & 1) result = (i64)((i128)result * a % mod);
			a = (i64)((i128)a * a % mod);
			e >>= 1;
		}
		return result;
	}

	static std::vector<int> multiply_ntt(const std::vector<int>& a,
			const std::vector<int>& b) {
		constexpr i64 p1 = 998244353, p2 = 1004535809, p3 = 469762049;
		using Z1 = ModInt<i64, p1>;
		using Z2 = ModInt<i64, p2>;
		using Z3 = ModInt<i64, p3>;
		std::vector<Z1> a1(a.begin(), a.end()), b1(b.begin(), b.end());
		std::vector<Z2> a2(a.begin(), a.end()), b2(b.begin(), b.end());
		std::vector<Z3> a3(a.begin(), a.end()), b3(b.begin(), b.end());
		a1 = convolution<i64, p1, 3>(a1, b1);
		a2 = convolution<i64, p2, 3>(a2, b2);
		a3 = convolution<i64, p3, 3>(a3, b3);
		const i64 inv12 = power_mod((i128)p1 * p2 % p3, p3 - 2, p3);
		const i64 inv1 = power_mod(p1, p2 - 2, p2);
		std::vector<int> result(a1.size());
		i128 carry = 0;
		for (int i = 0; i < (int)result.size(); ++i) {
			i64 r1 = a1[i].data(), r2 = a2[i].data(), r3 = a3[i].data();
			i64 t2 = (i64)((i128)(r2 - r1 + p2) * inv1 % p2);
			i128 x12 = r1 + (i128)p1 * t2;
			i64 t3 = (i64)((i128)((r3 - (i64)(x12 % p3) + p3) % p3) * inv12 % p3);
			i128 value = x12 + (i128)p1 * p2 * t3 + carry;
			result[i] = (int)(value % base);
			carry = value / base;
		}
		while (carry) {
			result.push_back((int)(carry % base));
			carry /= base;
		}
		return result;
	}

	void trim() {
		while (!digits.empty() && digits.back() == 0) digits.pop_back();
		if (digits.empty()) sign = 1;
	}

	int compare_abs(const BigInt& other) const {
		if (digits.size() != other.digits.size()) return digits.size() < other.digits.size() ? -1 : 1;
		for (int i = (int)(digits.size()) - 1; i >= 0; --i) {
			if (digits[i] != other.digits[i]) return digits[i] < other.digits[i] ? -1 : 1;
		}
		return 0;
	}

	void add_abs(const BigInt& other) {
		i64 carry = 0;
		for (std::size_t i = 0; i < std::max(digits.size(), other.digits.size()) || carry; ++i) {
			if (i == digits.size()) digits.push_back(0);
			const i64 value = digits[i] + carry + (i < other.digits.size() ? other.digits[i] : 0);
			digits[i] = (int)(value % base);
			carry = value / base;
		}
	}

	void sub_abs(const BigInt& other) {
		i64 carry = 0;
		for (std::size_t i = 0; i < other.digits.size() || carry; ++i) {
			i64 value = digits[i] - carry - (i < other.digits.size() ? other.digits[i] : 0);
			carry = value < 0;
			if (carry) value += base;
			digits[i] = (int)(value);
		}
		trim();
	}
};

#endif
