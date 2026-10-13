#include <unordered_map>
#include <unordered_set>
#include "../../include/hash.hpp"

#include "../../include/bit.hpp"

#include <cassert>

int main() {
	assert(lowbit(0) == 0);
	assert(lowbit(12) == 4);
	assert(highbit(0) == 0);
	assert(highbit(13) == 8);
	assert(floor_pow2(13) == 8);
	assert(ceil_pow2(0) == 1);
	assert(ceil_pow2(13) == 16);
	assert(floor_log2(16) == 4);
	assert(bit_width(16) == 5);
	assert(ceil_log2(16) == 4);
	assert(ceil_log2(17) == 5);
	assert(is_pow2(1));
	assert(is_pow2(1024));
	assert(!is_pow2(0));
	assert(!is_pow2(12));
	assert(popcount(0b10110100u) == 4);
	assert(bit_reverse(0b0011u, 4) == 0b1100u);

    {
    std::unordered_map<long long, int, CustomHash> hashed;
    hashed[1] = 2;
    hashed[1000000000000LL] = 3;
    std::unordered_set<std::pair<int, int>, CustomHash> pairs;
    pairs.insert({1, 2});
    assert(hashed[1] == 2 && hashed[1000000000000LL] == 3 && pairs.count({1, 2}));


    }
}
