#ifndef BINARY_TRIE_HPP
#define BINARY_TRIE_HPP

#include <cassert>
#include <cstdint>
#include <type_traits>
#include <vector>

// ============================================================================
// 【定长无符号整数 0-1 字典树 Binary Trie (binary_trie.hpp)】
// ----------------------------------------------------------------------------
// 模板类签名：template <typename T = std::uint64_t, int Bits = sizeof(T) * 8> class BinaryTrie
//   【模板约束】T 必须为无符号整型（如 uint64_t, uint32_t），0 < Bits <= sizeof(T) * 8；若 Bits 小于位宽，所有传入值的 >= Bits 高位必须为 0。
// 成员函数签名详解（单次操作时间复杂度均为 O(Bits)）：
//   - int size()
//     功能：返回当前字典树中存储的元素总数（含重复元素）。O(1)。
//   - void insert(T value)
//     功能：向多重集合中插入一个无符号整数 value。时间复杂度 O(Bits)。
//   - void erase(T value)
//     功能：从多重集合中删除一个 value 副本（【前置条件】必须保证 count(value) > 0，否则触发 assert）。O(Bits)。
//   - int count(T value)
//     功能：查询整数 value 在当前多重集合中的出现次数。O(Bits)。
//   - T max_xor(T value)
//     功能：要求 size() > 0；返回集合中某个元素 x 与 value 的最大异或结果 (x ^ value) 本身。O(Bits)。
//   - T min_xor(T value)
//     功能：要求 size() > 0；返回集合中某个元素 x 与 value 的最小异或结果 (x ^ value) 本身。O(Bits)。
// ============================================================================

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

	// void insert(T value) — 功能：向多重集合中插入一个无符号整数 value。时间复杂度 O(Bits)。
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

	// void erase(T value) — 功能：从多重集合中删除一个 value 副本（【前置条件】必须保证 count(value) > 0，否则触发 assert）。O(Bits)。
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

	// int count(T value) — 功能：查询整数 value 在当前多重集合中的出现次数。O(Bits)。
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

	// T max_xor(T value) — 功能：要求 size() > 0；返回集合中某个元素 x 与 value 的最大异或结果 (x ^ value) 本身。O(Bits)。
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

	// T min_xor(T value) — 功能：要求 size() > 0；返回集合中某个元素 x 与 value 的最小异或结果 (x ^ value) 本身。O(Bits)。
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
