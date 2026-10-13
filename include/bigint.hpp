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
#include "types.hpp"

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
		BigInt result;
		result.sign = sign * other.sign;
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
