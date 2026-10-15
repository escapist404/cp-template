#ifndef BINARY_TRIE_HPP
#define BINARY_TRIE_HPP

#include <cassert>
#include <cstdint>
#include <type_traits>
#include <vector>

// 【0-1 字典树 (binary_trie.hpp)】
// - 固定位宽的多重集合 0-1 Trie，模板参数 T 必须为无符号整型（如 uint64_t / uint32_t），单次操作 O(Bits)
// - insert(val) / erase(val): 插入一个 val / 删除一个已存在的 val（erase 要求 count(val) > 0）
// - count(val) / size()     : 查询 val 的出现次数 / 集合总元素数
// - max_xor(val) / min_xor(val): 返回集合元素与 val 异或的最大结果 / 最小结果本身（要求 size() > 0）

template <typename T = std::uint64_t, int Bits = sizeof(T) * 8>
class BinaryTrie {
	static_assert(std::is_unsigned<T>::value && 0 < Bits && Bits <= sizeof(T) * 8);
	using U = std::make_unsigned_t<T>;
	struct Node {
		int child[2] = {}, count = 0;
	};
	std::vector<Node> tree{{}};

public:
	int size() { return tree[0].count; }

	void insert(T value) {
		U x = (U)value;
		if constexpr (Bits < sizeof(T) * 8) assert((x >> Bits) == 0);
		int cur = 0;
		++tree[cur].count;
		for (int bit = Bits - 1; bit >= 0; --bit) {
			int b = (x >> bit) & 1;
			if (!tree[cur].child[b]) {
				tree[cur].child[b] = tree.size();
				tree.push_back(Node());
			}
			cur = tree[cur].child[b];
			++tree[cur].count;
		}
	}

	void erase(T value) {
		U x = (U)value;
		if constexpr (Bits < sizeof(T) * 8) assert((x >> Bits) == 0);
		assert(count(value) > 0);
		int cur = 0;
		--tree[cur].count;
		for (int bit = Bits - 1; bit >= 0; --bit) {
			cur = tree[cur].child[(x >> bit) & 1];
			--tree[cur].count;
		}
	}

	int count(T value) {
		U x = (U)value;
		if constexpr (Bits < sizeof(T) * 8) assert((x >> Bits) == 0);
		int cur = 0;
		for (int bit = Bits - 1; bit >= 0; --bit) {
			cur = tree[cur].child[(x >> bit) & 1];
			if (!cur) return 0;
		}
		return cur ? tree[cur].count : 0;
	}

	T max_xor(T value) {
		assert(size());
		U x = (U)value, result = 0;
		if constexpr (Bits < sizeof(T) * 8) assert((x >> Bits) == 0);
		int cur = 0;
		for (int bit = Bits - 1; bit >= 0; --bit) {
			int b = (x >> bit) & 1;
			int want = b ^ 1;
			if (tree[cur].child[want] && tree[tree[cur].child[want]].count)
				cur = tree[cur].child[want], result |= (U)1 << bit;
			else
				cur = tree[cur].child[b];
		}
		return (T)result;
	}

	T min_xor(T value) {
		assert(size());
		U x = (U)value, result = 0;
		if constexpr (Bits < sizeof(T) * 8) assert((x >> Bits) == 0);
		int cur = 0;
		for (int bit = Bits - 1; bit >= 0; --bit) {
			int b = (x >> bit) & 1;
			if (tree[cur].child[b] && tree[tree[cur].child[b]].count)
				cur = tree[cur].child[b];
			else
				cur = tree[cur].child[b ^ 1], result |= (U)1 << bit;
		}
		return (T)result;
	}
};

#endif
