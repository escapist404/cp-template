#ifndef MERGE_SORT_HPP
#define MERGE_SORT_HPP
#include <vector>

// ============================================================================
// 【稳定归并排序 Merge Sort (merge_sort.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   template <typename T>
//   void merge_sort(std::vector<T>& arr, int l, int r)
// 功能与复杂度：对 0-based 左闭右开区间 arr[l .. r) 原地执行稳定升序归并排序。时间 O((r - l) log(r - l))，空间 O(r - l)。
// ============================================================================

// template <typename T> void merge_sort(std::vector<T>& arr, int l, int r) — 功能：功能与复杂度：对 0-based 左闭右开区间 arr[l .. r) 原地执行稳定升序归并排序。时间 O((r - l) log(r - l))，空间 O(r - l)。
template <typename T>
void merge_sort(std::vector<T>& arr, int l, int r) {
	if (r - l <= 1) return;
	int mid = (l + r) >> 1;
	merge_sort(arr, l, mid), merge_sort(arr, mid, r);

	int i = l, j = mid;
	std::vector<T> temp;
	temp.reserve(r - l);
	while (i < mid && j < r)
		temp.push_back(arr[i] <= arr[j] ? arr[i++] : arr[j++]);
	while (i < mid)
		temp.push_back(arr[i++]);
	while (j < r)
		temp.push_back(arr[j++]);
	for (i = 0; l + i < r; ++i)
		arr[l + i] = temp[i];
}

#endif
