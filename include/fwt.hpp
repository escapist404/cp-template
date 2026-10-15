#ifndef FWT_HPP
#define FWT_HPP

#include <cassert>
#include <vector>

// ============================================================================
// 【快速沃尔什变换 Fast Walsh-Hadamard Transform / FWT 位运算卷积 (fwt.hpp)】
// ----------------------------------------------------------------------------
// 函数签名与接口详解（所有输入向量长度 n 必须为 0 或 2 的整数次幂，时间复杂度均为 O(n log n)）：
//   1. template <typename T, typename Update>
//      void fwt_transform(std::vector<T>& a, Update update)
//      功能：通用蝴蝶变换骨架，对长度 n = 2^k 的数组 a 按位分层调用 update(low_half, high_half)。
//   2. template <typename T> void fwt_xor(std::vector<T>& a, bool inverse = false)
//      功能：按位异或 (XOR) 正/逆变换（逆变换需除以 n，故 T 为 ModInt 时要求 n 在模意义下可逆）。
//   3. template <typename T> void fwt_or(std::vector<T>& a, bool inverse = false)
//      功能：按位或 (OR / 子集前缀和) 正/逆变换（仅含加减法，无需除法）。
//   4. template <typename T> void fwt_and(std::vector<T>& a, bool inverse = false)
//      功能：按位与 (AND / 超集后缀和) 正/逆变换（仅含加减法，无需除法）。
//   5. template <typename T, void (*Transform)(std::vector<T>&, bool)>
//      std::vector<T> fwt_convolution(std::vector<T> a, std::vector<T> b)
//      功能：计算等长 2 次幂数组 a 与 b 在指定变换 Transform（传入 fwt_xor<T>、fwt_or<T> 或 fwt_and<T>）下的位运算下标卷积：
//            c[k] = sum_{i op j == k} a[i] * b[j]。时间复杂度 O(n log n)。
// ============================================================================

// template <typename T, typename Update> void fwt_transform(std::vector<T>& a, Update update) — 功能：通用蝴蝶变换骨架，对长度 n = 2^k 的数组 a 按位分层调用 update(low_half, high_half)。
template <typename T, typename Update>
void fwt_transform(std::vector<T>& a, Update update) {
	int n = a.size();
	assert(!n || (n & (n - 1)) == 0);
	for (int len = 1; len < n; len <<= 1)
		for (int i = 0; i < n; i += len << 1)
			for (int j = 0; j < len; ++j)
				update(a[i + j], a[i + j + len]);
}


// template <typename T> void fwt_xor(std::vector<T>& a, bool inverse = false) — 功能：按位异或 (XOR) 正/逆变换（逆变换需除以 n，故 T 为 ModInt 时要求 n 在模意义下可逆）。
template <typename T>
void fwt_xor(std::vector<T>& a, bool inverse = false) {
	int n = a.size();
	fwt_transform(a, [](T& x, T& y) {
		T left = x, right = y;
		x = left + right;
		y = left - right;
	});
	if (inverse) for (T& x : a) x /= n;
}

// template <typename T> void fwt_or(std::vector<T>& a, bool inverse = false) — 功能：按位或 (OR / 子集前缀和) 正/逆变换（仅含加减法，无需除法）。
template <typename T>
void fwt_or(std::vector<T>& a, bool inverse = false) {
	fwt_transform(a, [inverse](T& x, T& y) {
		if (!inverse) y += x;
		else y -= x;
	});
}

// template <typename T> void fwt_and(std::vector<T>& a, bool inverse = false) — 功能：按位与 (AND / 超集后缀和) 正/逆变换（仅含加减法，无需除法）。
template <typename T>
void fwt_and(std::vector<T>& a, bool inverse = false) {
	fwt_transform(a, [inverse](T& x, T& y) {
		if (!inverse) x += y;
		else x -= y;
	});
}

template <typename T, void (*Transform)(std::vector<T>&, bool)>
std::vector<T> fwt_convolution(std::vector<T> a, std::vector<T> b) {
	assert(a.size() == b.size() && (a.empty() || (a.size() & (a.size() - 1)) == 0));
	Transform(a, false);
	Transform(b, false);
	for (int i = 0; i < (int)a.size(); ++i) a[i] *= b[i];
	Transform(a, true);
	return a;
}

#endif
