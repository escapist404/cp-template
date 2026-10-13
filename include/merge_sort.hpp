#ifndef MERGE_SORT_HPP
#define MERGE_SORT_HPP
#include <vector>

// 0 index

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
