#include <cassert>
#include <cstdio>
#include "../../include/modint.hpp"

using mint = ModInt<int, 998244353>;
using dmint = ModInt<long long, 0>;

int main() {
    // Test 1: Basic arithmetic
    {
        mint a(5), b(10);
        assert((a + b).data() == 15);
        assert((b - a).data() == 5);
        assert((a * b).data() == 50);
    }

    // Test 2: Long long construction (fixed bug)
    {
        mint a(3000000000LL);
        assert(a.data() == 5266941);

        mint b(-3000000000LL);
        assert((a + b).data() == 0);
    }

    // Test 3: Division (prime modulus)
    {
        mint a(10), b(2);
        assert((a / b).data() == 5);

        mint c(7), d(2);
        mint e = c / d;
        assert((e * d).data() == c.data());
    }

    // Test 4: Large operations
    {
        mint a(998244352);
        assert((a + mint(1)).data() == 0);
        assert((a * mint(2)).data() == 998244351);
    }

    // Test 5: Dynamic modulus
    {
        dmint::set_mod(1000000007);
        dmint a(1000000006);
        dmint b(1);
        assert((a + b).data() == 0);
    }

    // Test 6: Zero handling
    {
        mint zero(0), one(1);
        assert((zero + zero).data() == 0);
        assert((zero * one).data() == 0);
        assert((one - one).data() == 0);
        assert(mint(-5).data() == 998244353 - 5);
    }

    printf("All ModInt tests passed!\n");
    return 0;
}
