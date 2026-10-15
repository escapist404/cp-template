#ifndef FORMAL_POWER_SERIES_HPP
#define FORMAL_POWER_SERIES_HPP

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <utility>
#include <vector>

#include "polynomial.hpp"

// ============================================================================
// 【形式幂级数 / 多项式全家桶 Formal Power Series (formal_power_series.hpp)】
// ----------------------------------------------------------------------------
// 模板类签名：
//   template <typename T, T mod, T root = primitive_root_constexpr(mod)> class Polynomial
//   - 类型别名：using Z = ModInt<T, mod>; using Self = Polynomial<T, mod, root>;
//   - 维护模 NTT 素数 mod 下的多项式 / 形式幂级数，自动去除最高位多余的零系数（零多项式为空向量，degree() == -1）。
// 构造函数与基础访问签名：
//   - Polynomial() / Polynomial(const std::vector<Z>&) / Polynomial(std::initializer_list<Z>)
//   - int degree() const / bool empty() const / int size() const
//   - Z& operator[](int index) / const Z& operator[](int index) const（只读越界安全返回 0）
//   - const std::vector<Z>& data() const / void resize(int n) / Self truncated(int n) const（截取前 n 项 mod x^n）
// 核心幂级数运算成员函数签名详解（所有传参 n 的函数均返回 mod x^n 的前 n 项结果，复杂度均为 O(n log n)）：
//   - Self derivative() const
//     功能：形式求导 f'(x)。时间复杂度 O(n)。
//   - Self integral() const
//     功能：形式不定积分 int_0^x f(t) dt（常数项为 0，要求 n <= mod）。时间复杂度 O(n)。
//   - Self inverse(int n) const
//     功能：牛顿迭代求模 x^n 乘法逆元 f^(-1)(x)（【前置条件】常数项 f[0] != 0）。O(n log n)。
//   - Self logarithm(int n) const
//     功能：求形式自然对数 ln f(x) = int (f'(x) * f^(-1)(x)) dx mod x^n（【前置条件】常数项 f[0] == 1）。O(n log n)。
//   - Self exponential(int n) const
//     功能：牛顿迭代求形式指数函数 exp f(x) mod x^n（【前置条件】常数项 f[0] == 0）。O(n log n)。
//   - Self power(i64 exponent, int n) const
//     功能：求非负整数次幂 (f(x))^exponent mod x^n（要求 exponent >= 0；自动提取最低非零项平移，不要求 f[0] == 1）。O(n log n)。
//   - Self square_root(int n) const
//     功能：求模 x^n 下的多项式平方根 g(x) 满足 g^2(x) === f(x) (mod x^n)
//     （【前置条件】最低非零项次数必须为偶数，且其系数在模 mod 下为二次剩余；否则触发 assert）。O(n log n)。
//   - std::pair<Self, Self> divmod(Self divisor) const
//     功能：多项式带余除法，返回 {商多项式 Q, 余多项式 R}，满足 *this == Q * divisor + R 且 deg(R) < deg(divisor)。
//   - 算术运算符：operator+, operator-, operator*, operator/, operator% 及其复合赋值。
// ============================================================================

template <typename T, T mod, T root = primitive_root_constexpr(mod)>
class Polynomial {
public:
	using Z = ModInt<T, mod>;
	using Self = Polynomial<T, mod, root>;

private:
	std::vector<Z> coefficient;

	void normalize() {
		while (!coefficient.empty() && coefficient.back() == Z()) {
			coefficient.pop_back();
		}
	}

public:
	Polynomial() = default;

	Polynomial(const std::vector<Z>& coefficient)
		: coefficient(coefficient) {
		normalize();
	}

	Polynomial(std::vector<Z>&& coefficient)
		: coefficient(std::move(coefficient)) {
		normalize();
	}

	Polynomial(std::initializer_list<Z> coefficient)
		: coefficient(coefficient) {
		normalize();
	}

	int degree() const {
		return (int)(coefficient.size()) - 1;
	}

	bool empty() const {
		return coefficient.empty();
	}

	int size() const {
		return (int)(coefficient.size());
	}

	Z& operator[](int index) {
		return coefficient[index];
	}

	const Z& operator[](int index) const {
		return coefficient[index];
	}

	const std::vector<Z>& data() const {
		return coefficient;
	}

	void resize(int n) {
		coefficient.resize(n);
		normalize();
	}

	Self truncated(int n) const {
		if (n <= 0) {
			return {};
		}
		std::vector<Z> result(
			coefficient.begin(),
			coefficient.begin() + std::min(n, size()));
		return Self(std::move(result));
	}

	// Self derivative() const — 功能：形式求导 f'(x)。时间复杂度 O(n)。
	Self derivative() const {
		if (size() <= 1) {
			return {};
		}

		std::vector<Z> result(size() - 1);
		for (int i = 1; i < size(); ++i) {
			result[i - 1] = coefficient[i] * Z(i);
		}
		return result;
	}

	// Self integral() const — 功能：形式不定积分 int_0^x f(t) dt（常数项为 0，要求 n <= mod）。时间复杂度 O(n)。
	Self integral() const {
		std::vector<Z> result(size() + 1);
		for (int i = 0; i < size(); ++i) {
			result[i + 1] = coefficient[i] / Z(i + 1);
		}
		return result;
	}

	// Self logarithm(int n) const — 功能：求形式自然对数 ln f(x) = int (f'(x) * f^(-1)(x)) dx mod x^n（【前置条件】常数项 f[0] == 1）。O(n log n)。
	Self logarithm(int n) const {
		assert(n >= 0 && !empty() && coefficient[0] == Z(1));
		if (!n) return {};
		Self result = (derivative() * inverse(n)).truncated(n - 1).integral();
		result.coefficient.resize(n);
		return result;
	}

	// Self square_root(int n) const — 功能：求模 x^n 下的多项式平方根 g(x) 满足 g^2(x) === f(x) (mod x^n)
	static Z square_root(Z value) {
		const T p = Z::mod();
		assert(p > 1);
		if (value == Z(0)) return Z(0);
		if (p == 2) return value;
		Z power_value = value;
		assert(::power(Z(1), power_value, (p - 1) / 2) == Z(1));
		if (p % 4 == 3) return ::power(Z(1), power_value, (p + 1) / 4);
		T q = p - 1;
		int s = 0;
		while (!(q & 1)) q >>= 1, ++s;
		Z non_residue(2);
		while (::power(Z(1), non_residue, (p - 1) / 2) == Z(1)) ++non_residue.number;
		Z c = ::power(Z(1), non_residue, q);
		Z x = ::power(Z(1), power_value, (q + 1) / 2);
		Z t = ::power(Z(1), power_value, q);
		int m = s;
		while (t != Z(1)) {
			int i = 1;
			Z square = t * t;
			while (square != Z(1)) square *= square, ++i;
			Z b = ::power(Z(1), c, (i64)1 << (m - i - 1));
			x *= b;
			c = b * b;
			t *= c;
			m = i;
		}
		Z other = -x;
		return x.data() < other.data() ? x : other;
	}

	// Self exponential(int n) const — 功能：牛顿迭代求形式指数函数 exp f(x) mod x^n（【前置条件】常数项 f[0] == 0）。O(n log n)。
	Self exponential(int n) const {
		assert(n >= 0 && (empty() || coefficient[0] == Z(0)));
		if (!n) return {};
		Self result({Z(1)});
		for (int length = 1; length < n; length <<= 1) {
			const int next = std::min(length << 1, n);
			Self delta = truncated(next) - result.logarithm(next);
			delta.coefficient.resize(next);
			delta.coefficient[0] += Z(1);
			result = (result * delta).truncated(next);
			result.coefficient.resize(next);
		}
		result.coefficient.resize(n);
		return result;
	}

	// Self power(i64 exponent, int n) const — 功能：求非负整数次幂 (f(x))^exponent mod x^n（要求 exponent >= 0；自动提取最低非零项平移，不要求 f[0] == 1）。O(n log n)。
	Self power(i64 exponent, int n) const {
		assert(exponent >= 0 && n >= 0);
		if (!n) return {};
		if (!exponent) {
			Self result({Z(1)});
			result.coefficient.resize(n);
			return result;
		}
		int shift = 0;
		while (shift < size() && coefficient[shift] == Z(0)) ++shift;
		if (shift == size() || (i128)shift * exponent >= n) {
			Self result;
			result.coefficient.resize(n);
			return result;
		}
		const int length = n - (int)((i128)shift * exponent);
		Self normalized(std::vector<Z>(coefficient.begin() + shift, coefficient.end()));
		for (int i = 0; i < normalized.size(); ++i) normalized[i] /= coefficient[shift];
		Self result = (normalized.logarithm(length) * Z(exponent)).exponential(length);
		const Z scale = ::power(Z(1), coefficient[shift], exponent);
		for (int i = 0; i < result.size(); ++i) result[i] *= scale;
		Self answer;
		answer.coefficient.assign((int)((i128)shift * exponent), Z(0));
		answer.coefficient.insert(answer.coefficient.end(), result.coefficient.begin(), result.coefficient.end());
		answer.coefficient.resize(n);
		return answer;
	}

	Self square_root(int n) const {
		assert(n >= 0 && !empty());
		if (!n) return {};
		int shift = 0;
		while (shift < size() && coefficient[shift] == Z(0)) ++shift;
		if (shift == size()) {
			Self result;
			result.coefficient.resize(n);
			return result;
		}
		assert(shift % 2 == 0 && shift / 2 < n);
		const int offset = shift / 2;
		Self normalized(std::vector<Z>(coefficient.begin() + shift, coefficient.end()));
		for (int i = 0; i < normalized.size(); ++i) normalized[i] /= coefficient[shift];
		const Z root_value = square_root(coefficient[shift]);
		const int length = n - offset;
		Self result({Z(1)});
		const Z inverse_two = Z(1) / Z(2);
		for (int current = 1; current < length; current <<= 1) {
			const int next = std::min(current << 1, length);
			result = ((result + normalized.truncated(next) * result.inverse(next)) * inverse_two).truncated(next);
			result.coefficient.resize(next);
		}
		result *= root_value;
		Self answer;
		answer.coefficient.assign(offset, Z(0));
		answer.coefficient.insert(answer.coefficient.end(), result.coefficient.begin(), result.coefficient.end());
		answer.coefficient.resize(n);
		return answer;
	}

	// Self inverse(int n) const — 功能：牛顿迭代求模 x^n 乘法逆元 f^(-1)(x)（【前置条件】常数项 f[0] != 0）。O(n log n)。
	Self inverse(int n) const {
		assert(!empty() && coefficient[0].data() != 0);
		Self result({Z(1) / coefficient[0]});

		for (int length = 1; length < n; length <<= 1) {
			const int next_length = std::min(length << 1, n);
			Self part = truncated(next_length);
			Self product = (part * result).truncated(next_length);
			product.coefficient.resize(next_length);
			product[0] = Z(2) - product[0];
			for (int i = 1; i < next_length; ++i) {
				product[i] = -product[i];
			}
			result = (result * product).truncated(next_length);
		}

		result.coefficient.resize(n);
		return result;
	}

	// std::pair<Self, Self> divmod(Self divisor) const — 功能：多项式带余除法，返回 {商多项式 Q, 余多项式 R}，满足 *this == Q * divisor + R 且 deg(R) < deg(divisor)。
	std::pair<Self, Self> divmod(Self divisor) const {
		assert(!divisor.empty());
		Self remainder = *this;
		Self quotient;
		divisor.normalize();

		if (remainder.degree() < divisor.degree()) {
			return {quotient, remainder};
		}

		quotient.coefficient.resize(remainder.degree() - divisor.degree() + 1);
		const Z inverse_leading = Z(1) / divisor.coefficient.back();
		while (!remainder.empty() && remainder.degree() >= divisor.degree()) {
			const int shift = remainder.degree() - divisor.degree();
			const Z factor = remainder.coefficient.back() * inverse_leading;
			quotient[shift] = factor;

			for (int i = 0; i <= divisor.degree(); ++i) {
				remainder[shift + i] -= factor * divisor[i];
			}
			remainder.normalize();
		}
		quotient.normalize();
		return {quotient, remainder};
	}

	Self& operator+=(const Self& rhs) {
		if (rhs.size() > size()) {
			coefficient.resize(rhs.size());
		}
		for (int i = 0; i < rhs.size(); ++i) {
			coefficient[i] += rhs[i];
		}
		normalize();
		return *this;
	}

	Self& operator-=(const Self& rhs) {
		if (rhs.size() > size()) {
			coefficient.resize(rhs.size());
		}
		for (int i = 0; i < rhs.size(); ++i) {
			coefficient[i] -= rhs[i];
		}
		normalize();
		return *this;
	}

	Self& operator*=(const Self& rhs) {
		coefficient = (convolution<T, mod, root>(coefficient, rhs.coefficient));
		normalize();
		return *this;
	}

	Self& operator*=(Z rhs) {
		for (Z& value : coefficient) value *= rhs;
		normalize();
		return *this;
	}

	Self& operator/=(const Self& rhs) {
		*this = divmod(rhs).first;
		return *this;
	}

	Self& operator%=(const Self& rhs) {
		*this = divmod(rhs).second;
		return *this;
	}

	friend Self operator+(Self lhs, const Self& rhs) {
		lhs += rhs;
		return lhs;
	}

	friend Self operator-(Self lhs, const Self& rhs) {
		lhs -= rhs;
		return lhs;
	}

	friend Self operator*(Self lhs, const Self& rhs) {
		lhs *= rhs;
		return lhs;
	}

	friend Self operator*(Self lhs, Z rhs) {
		lhs *= rhs;
		return lhs;
	}

	friend Self operator/(Self lhs, const Self& rhs) {
		lhs /= rhs;
		return lhs;
	}

	friend Self operator%(Self lhs, const Self& rhs) {
		lhs %= rhs;
		return lhs;
	}
};

#endif
