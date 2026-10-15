#ifndef DYNAMIC_BITSET_HPP
#define DYNAMIC_BITSET_HPP

#include <algorithm>
#include <cstdint>
#include <vector>

#include "bit.hpp"

// ============================================================================
// 【64 位分块动态位集 Dynamic Bitset (dynamic_bitset.hpp)】
// ----------------------------------------------------------------------------
// 类签名：class DynamicBitset
//   按 uint64_t（64 位）分块压缩存储的 0-based 动态长度位集，始终保持最高块多余高位清零。
// 构造函数与成员函数签名详解：
//   - DynamicBitset() / DynamicBitset(std::size_t n, bool value = false)
//     功能：构造空位集，或长度为 n、初始全部比特为 value 的位集。时间复杂度 O(n / 64)。
//   - std::size_t size() const / bool empty() const
//     功能：返回当前逻辑比特位数 n / 判断是否为空（n == 0）。O(1)。
//   - void resize(std::size_t new_size, bool value = false)
//     功能：调整位集长度为 new_size，新增的高位比特填充为 value。O(new_size / 64)。
//   - void push_back(bool value)
//     功能：在末尾追加一个比特 value（下标为原 size()）。均摊 O(1)。
//   - void set(std::size_t p, bool value = true)
//     功能：将第 p 位设为 value；【注意】若 p >= size()，会自动扩容至 p + 1 并中间补 0。
//   - void reset(std::size_t p) / void reset()
//     功能：将第 p 位清零（p >= size() 时不扩容无操作） / 将所有现有位全部清零。
//   - bool test(std::size_t p) const / bool operator[](std::size_t p) const
//     功能：只读查询第 p 位布尔值（若 p >= size() 安全返回 false，不扩容）。O(1)。
//   - std::size_t count() const / bool any() const / bool none() const
//     功能：统计二进制 1 的总个数 / 是否存在至少一个 1 / 是否全为 0。时间复杂度 O(n / 64)。
//   - DynamicBitset& operator|=(const DynamicBitset& other)
//   - DynamicBitset& operator^=(const DynamicBitset& other)
//     功能：按位或 / 按位异或复合赋值，结果长度自动扩展为 max(size(), other.size())。O(n / 64)。
//   - DynamicBitset& operator&=(const DynamicBitset& other)
//     功能：按位与复合赋值，结果长度自动收缩为 min(size(), other.size())。O(n / 64)。
//   - friend DynamicBitset operator|(lhs, rhs) / operator&(lhs, rhs) / operator^(lhs, rhs)
//   - friend bool operator==(lhs, rhs) / operator!=(lhs, rhs)
// ============================================================================

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
	// DynamicBitset() — 功能：构造空位集，或长度为 n、初始全部比特为 value 的位集。时间复杂度 O(n / 64)。
	DynamicBitset() = default;
	// DynamicBitset(std::size_t n, bool value = false) — 功能：构造空位集，或长度为 n、初始全部比特为 value 的位集。时间复杂度 O(n / 64)。
	DynamicBitset(std::size_t n, bool value = false) : n(n), data(blocks(n), value ? ~Block() : Block()) {
		trim();
	}

	// std::size_t size() const — 功能：返回当前逻辑比特位数 n / 判断是否为空（n == 0）。O(1)。
	std::size_t size() const {
		return n;
	}

	// bool empty() const — 功能：返回当前逻辑比特位数 n / 判断是否为空（n == 0）。O(1)。
	bool empty() const {
		return n == 0;
	}

	// void resize(std::size_t new_size, bool value = false) — 功能：调整位集长度为 new_size，新增的高位比特填充为 value。O(new_size / 64)。
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

	// void push_back(bool value) — 功能：在末尾追加一个比特 value（下标为原 size()）。均摊 O(1)。
	void push_back(bool value) {
		const std::size_t p = n;
		resize(n + 1);
		set(p, value);
	}

	// void set(std::size_t p, bool value = true) — 功能：将第 p 位设为 value；【注意】若 p >= size()，会自动扩容至 p + 1 并中间补 0。
	void set(std::size_t p, bool value = true) {
		ensure(p);
		const Block mask = Block(1) << (p % block_bits);
		if (value) {
			data[p / block_bits] |= mask;
		} else {
			data[p / block_bits] &= ~mask;
		}
	}

	// void reset(std::size_t p) — 功能：将第 p 位清零（p >= size() 时不扩容无操作） / 将所有现有位全部清零。
	void reset(std::size_t p) {
		if (p < n) {
			data[p / block_bits] &= ~(Block(1) << (p % block_bits));
		}
	}

	// void reset() — 功能：将第 p 位清零（p >= size() 时不扩容无操作） / 将所有现有位全部清零。
	void reset() {
		std::fill(data.begin(), data.end(), Block());
	}

	// bool test(std::size_t p) const — 功能：只读查询第 p 位布尔值（若 p >= size() 安全返回 false，不扩容）。O(1)。
	bool test(std::size_t p) const {
		return p < n && (data[p / block_bits] >> (p % block_bits) & 1);
	}

	// bool operator[](std::size_t p) const — 功能：只读查询第 p 位布尔值（若 p >= size() 安全返回 false，不扩容）。O(1)。
	bool operator[](std::size_t p) const {
		return test(p);
	}

	// std::size_t count() const — 功能：统计二进制 1 的总个数 / 是否存在至少一个 1 / 是否全为 0。时间复杂度 O(n / 64)。
	std::size_t count() const {
		std::size_t result = 0;
		for (const Block block : data) {
			result += popcount(block);
		}
		return result;
	}

	// bool any() const — 功能：统计二进制 1 的总个数 / 是否存在至少一个 1 / 是否全为 0。时间复杂度 O(n / 64)。
	bool any() const {
		return count() != 0;
	}
	
	// bool none() const — 功能：统计二进制 1 的总个数 / 是否存在至少一个 1 / 是否全为 0。时间复杂度 O(n / 64)。
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

	// DynamicBitset& operator&=(const DynamicBitset& other) — 功能：按位与复合赋值，结果长度自动收缩为 min(size(), other.size())。O(n / 64)。
	DynamicBitset& operator&=(const DynamicBitset& other) {
		const std::size_t limit = std::min(n, other.n);
		resize(limit);
		for (std::size_t i = 0; i < data.size(); ++i) {
			data[i] &= other.data[i];
		}
		return *this;
	}

	// DynamicBitset& operator^=(const DynamicBitset& other) — 功能：按位或 / 按位异或复合赋值，结果长度自动扩展为 max(size(), other.size())。O(n / 64)。
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
