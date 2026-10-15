#ifndef FWT_HPP
#define FWT_HPP

#include <cassert>
#include <vector>

// 【快速沃尔什变换 FWT 位运算卷积 (fwt.hpp)】
// - 在 O(n log n) 时间内计算长度为 2 次幂的按位异或 (XOR)、按位或 (OR)、按位与 (AND) 下标卷积：
// - fwt_xor(a, inv) / fwt_or(a, inv) / fwt_and(a, inv): 原地正/逆变换（要求 a.size() 为非零 2 次幂）
// - convolution_xor(a, b) / convolution_or(a, b) / convolution_and(a, b): 自动补齐至 2 次幂并返回位运算卷积结果

template <typename T, typename Update>
void fwt_transform(std::vector<T>& a, Update update) {
	int n = a.size();
	assert(!n || (n & (n - 1)) == 0);
	for (int len = 1; len < n; len <<= 1)
		for (int i = 0; i < n; i += len << 1)
			for (int j = 0; j < len; ++j)
				update(a[i + j], a[i + j + len]);
}


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

template <typename T>
void fwt_or(std::vector<T>& a, bool inverse = false) {
	fwt_transform(a, [inverse](T& x, T& y) {
		if (!inverse) y += x;
		else y -= x;
	});
}

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
