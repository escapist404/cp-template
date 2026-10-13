#ifndef BINARY_TRIE_HPP
#define BINARY_TRIE_HPP

#include <cassert>
#include <cstdint>
#include <type_traits>
#include <vector>

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
