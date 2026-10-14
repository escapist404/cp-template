#ifndef DYNAMIC_BITSET_HPP
#define DYNAMIC_BITSET_HPP

#include <algorithm>
#include <cstdint>
#include <vector>

#include "bit.hpp"

// Dynamic-length bitset backed by 64-bit blocks. O(n / 64) bitwise operations and popcount.

class DynamicBitset {
	using Block = std::uint64_t;
	static constexpr int block_bits = 64;

	std::size_t n = 0;
	std::vector<Block> data;

	static constexpr std::size_t blocks(std::size_t n) {
		return (n + block_bits - 1) / block_bits;
	}

	void trim() {
		if (n % block_bits) {
			data.back() &= (Block(1) << (n % block_bits)) - 1;
		}
	}

	void ensure(std::size_t p) {
		if (p < n) {
			return;
		}
		resize(p + 1);
	}

public:
	DynamicBitset() = default;
	DynamicBitset(std::size_t n, bool value = false) : n(n), data(blocks(n), value ? ~Block() : Block()) {
		trim();
	}

	std::size_t size() const {
		return n;
	}

	bool empty() const {
		return n == 0;
	}

	void resize(std::size_t new_size, bool value = false) {
		const std::size_t old_size = n;
		data.resize(blocks(new_size));
		if (value && new_size > old_size) {
			for (std::size_t i = old_size; i < new_size; ++i) {
				data[i / block_bits] |= Block(1) << (i % block_bits);
			}
		}
		n = new_size;
		trim();
	}

	void push_back(bool value) {
		const std::size_t p = n;
		resize(n + 1);
		set(p, value);
	}

	void set(std::size_t p, bool value = true) {
		ensure(p);
		const Block mask = Block(1) << (p % block_bits);
		if (value) {
			data[p / block_bits] |= mask;
		} else {
			data[p / block_bits] &= ~mask;
		}
	}

	void reset(std::size_t p) {
		if (p < n) {
			data[p / block_bits] &= ~(Block(1) << (p % block_bits));
		}
	}

	void reset() {
		std::fill(data.begin(), data.end(), Block());
	}

	bool test(std::size_t p) const {
		return p < n && (data[p / block_bits] >> (p % block_bits) & 1);
	}

	bool operator[](std::size_t p) const {
		return test(p);
	}

	std::size_t count() const {
		std::size_t result = 0;
		for (const Block block : data) {
			result += popcount(block);
		}
		return result;
	}

	bool any() const {
		return count() != 0;
	}
	
	bool none() const {
		return !any();
	}

	DynamicBitset& operator|=(const DynamicBitset& other) {
		resize(std::max(n, other.n));
		for (std::size_t i = 0; i < other.data.size(); ++i) {
			data[i] |= other.data[i];
		}
		return *this;
	}

	DynamicBitset& operator&=(const DynamicBitset& other) {
		const std::size_t limit = std::min(n, other.n);
		resize(limit);
		for (std::size_t i = 0; i < data.size(); ++i) {
			data[i] &= other.data[i];
		}
		return *this;
	}

	DynamicBitset& operator^=(const DynamicBitset& other) {
		resize(std::max(n, other.n));
		for (std::size_t i = 0; i < other.data.size(); ++i) {
			data[i] ^= other.data[i];
		}
		return *this;
	}

	friend DynamicBitset operator|(DynamicBitset lhs, const DynamicBitset& rhs) {
		return lhs |= rhs;
	}

	friend DynamicBitset operator&(DynamicBitset lhs, const DynamicBitset& rhs) {
		return lhs &= rhs;
	}
	
	friend DynamicBitset operator^(DynamicBitset lhs, const DynamicBitset& rhs) {
		return lhs ^= rhs;
	}

	friend bool operator==(const DynamicBitset& lhs, const DynamicBitset& rhs) {
		return lhs.n == rhs.n && lhs.data == rhs.data;
	}
	friend bool operator!=(const DynamicBitset& lhs, const DynamicBitset& rhs) {
		return !(lhs == rhs);
	}
};

#endif
