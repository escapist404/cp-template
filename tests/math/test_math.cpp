
#include "../../include/stirling.hpp"

#include "../../include/bigint.hpp"
#include "../../include/euler.hpp"
#include "../../include/miller_rabin.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <numeric>
#include <vector>
#include "../../include/sieve.hpp"
#include "../../include/bsgs.hpp"
#include "../../include/comb.hpp"
#include "../../include/crt.hpp"
#include "../../include/determinant.hpp"
#include "../../include/exgcd.hpp"
#include "../../include/floor_sum.hpp"
#include "../../include/formal_power_series.hpp"
#include "../../include/gaussian.hpp"
#include "../../include/lgv.hpp"
#include "../../include/linear_recurrence.hpp"
#include "../../include/lagrange.hpp"
#include "../../include/lucas.hpp"
#include "../../include/matrix.hpp"
#include "../../include/matrix_tree.hpp"
#include "../../include/min25.hpp"
#include "../../include/modint.hpp"
#include "../../include/modular_arithmetic.hpp"
#include "../../include/modular_roots.hpp"
#include "../../include/pollard_rho.hpp"
#include "../../include/polynomial.hpp"

template <typename T>
T power(T base, int exponent) {
    T result = 1;
    while (exponent > 0) {
        if (exponent & 1) {
            result *= base;
        }
        base *= base;
        exponent >>= 1;
    }
    return result;
}

long long choose_mod(int n, int k, long long modulus) {
    if (k < 0 || k > n) {
        return 0;
    }
    k = std::min(k, n - k);
    __int128 result = 1;
    for (int i = 1; i <= k; ++i) {
        result = result * (n - k + i) / i;
    }
    return (i64)(result % modulus);
}

int mobius_brute(int n) {
    int factors = 0;
    for (int p = 2; p * p <= n; ++p) {
        if (n % p != 0) {
            continue;
        }
        n /= p;
        ++factors;
        if (n % p == 0) {
            return 0;
        }
    }
    if (n > 1) {
        ++factors;
    }
    return factors & 1 ? -1 : 1;
}

void test_modular_arithmetic() {
    using Z = ModInt<int, 998244353>;
    using DynamicZ = ModInt<long long>;
    DynamicZ::set_mod(11);
    assert((DynamicZ(5) + DynamicZ(8)).data() == 2);
    DynamicZ::set_mod(13);
    assert((DynamicZ(5) + DynamicZ(8)).data() == 0);
    Combinatorics<Z> combinations(100);
    std::vector<std::vector<int>> pascal(101, std::vector<int>(101));
    for (int n = 0; n <= 100; ++n) {
        pascal[n][0] = pascal[n][n] = 1;
        for (int k = 1; k < n; ++k) {
            pascal[n][k] = (pascal[n - 1][k - 1] + pascal[n - 1][k]) % 998244353;
        }
        for (int k = 0; k <= n; ++k) {
            assert(combinations(n, k).data() == pascal[n][k]);
        }
    }
    Polynomial<int, 998244353> polynomial({1, 2, 3});
    Polynomial<int, 998244353> inverse = polynomial.inverse(6);
    Polynomial<int, 998244353> product = (polynomial * inverse).truncated(6);
    assert(product[0] == Z(1));
    for (int i = 1; i < product.size(); ++i) {
        assert(product[i] == Z(0));
    }
    Matrix<int, 2, 2> left{{1, 2, 3, 4}};
    Matrix<int, 2, 2> right{{5, 6, 7, 8}};
    const auto result = left * right;
	assert(result[0][0] == 19 && result[1][1] == 50);
	Matrix<int, 2, 2> static_matrix{{1, 2, 3, 4}};
	assert(determinant(static_matrix) == -2);
	Matrix<int, 0, 0> dynamic_matrix(2, 2, {0, 1, 1, 0});
	assert(determinant(dynamic_matrix) == -1);
	assert(lagrange_evaluate(std::vector<Z>{0, 1, 2},
			std::vector<Z>{1, 3, 7}, Z(3)) == Z(13));
	assert(lagrange_evaluate_consecutive(std::vector<Z>{1, 3, 7}, Z(3)) == Z(13));
}

void test_sieves_and_congruences() {
	assert(floor_sqrt(0LL) == 0 && floor_sqrt(10LL) == 3);
	assert(ceil_sqrt(0LL) == 0 && ceil_sqrt(10LL) == 4);
	assert(sieve_prime(30).size() == 10);
	auto arithmetic_result = sieve_arithmetic(500);
	const auto &arithmetic_primes = std::get<0>(arithmetic_result);
	const auto &arithmetic_phi = std::get<1>(arithmetic_result);
	const auto &arithmetic_mu = std::get<2>(arithmetic_result);
	auto phi_result = sieve_phi(500);
	auto mu_result = sieve_mu(500);
	assert(phi_result.first == arithmetic_primes && phi_result.second == arithmetic_phi);
	assert(mu_result.first == arithmetic_primes && mu_result.second == arithmetic_mu);
	assert(arithmetic_primes == sieve_prime(500));
	auto factor_result = sieve_factor(500);
	const auto &factor_primes = factor_result.first;
	const auto &factor_min = factor_result.second;
	assert(factor_primes == arithmetic_primes);
	auto divisor_result = sieve_divisor(500);
	const auto &divisor_tau = std::get<1>(divisor_result);
	const auto &divisor_sigma = std::get<2>(divisor_result);
    for (int n = 1; n <= 500; ++n) {
        int phi = 0;
		int tau = 0;
		long long sigma = 0;
        for (int i = 1; i <= n; ++i) {
            phi += std::gcd(i, n) == 1;
			if (n % i == 0) {
				++tau;
				sigma += i;
			}
        }
		assert(arithmetic_phi[n] == phi);
		assert(arithmetic_mu[n] == mobius_brute(n));
		assert(divisor_tau[n] == tau && divisor_sigma[n] == sigma);
		int value = n;
		while (value > 1) {
			const int prime = factor_min[value];
			assert(prime >= 2 && value % prime == 0);
			while (value % prime == 0) value /= prime;
		}
        int reconstructed = 1;
		value = n;
		while (value > 1) {
			const int prime = factor_min[value];
			int count = 0;
			while (value % prime == 0) {
				value /= prime;
				++count;
			}
			reconstructed *= power(prime, count);
        }
        assert(reconstructed == n);
    }
    for (long long a = -50; a <= 50; ++a) {
        for (long long b = -50; b <= 50; ++b) {
            long long x, y;
            const long long g = extended_gcd(a, b, x, y);
            assert(g >= 0 && a * x + b * y == g);
            if (g == 1 && b > 1) {
                assert(((a % b + b) % b) * mod_inverse(a, b) % b == 1);
            }
        }
    }
    for (int modulus = 1; modulus <= 30; ++modulus) {
        for (int residue = 0; residue < modulus; ++residue) {
            const auto result = crt({{residue, modulus}});
            assert(result.first == residue && result.second == modulus);
        }
    }
}

void test_number_theory() {
    for (int n = 0; n <= 20; ++n) {
        for (int m = 1; m <= 20; ++m) {
            for (int a = -20; a <= 20; ++a) {
                for (int b = -20; b <= 20; ++b) {
                    long long expected = 0;
                    for (int i = 0; i < n; ++i) {
                        const long long value = 1LL * a * i + b;
                        expected += value / m - (value % m < 0);
                    }
                    assert(floor_sum(0, n, m, a, b) == expected);
                }
            }
        }
    }
    for (int modulus = 1; modulus <= 100; ++modulus) {
        for (int a = -modulus; a <= modulus; ++a) {
            for (int b = 0; b < modulus; ++b) {
                long long expected = -1;
                long long value = 1 % modulus;
                for (int exponent = 0; exponent <= modulus + 2; ++exponent) {
                    if (value == b) {
                        expected = exponent;
                        break;
                    }
                    value = value * ((a % modulus + modulus) % modulus) % modulus;
                }
                assert(bsgs(a, b, modulus) == expected);
            }
        }
    }
    for (int n = 0; n < 100; ++n) {
        for (int k = 0; k <= n; ++k) {
            assert(Lucas(7).choose(n, k) == choose_mod(n, k, 7));
        }
    }
    for (int modulus = 2; modulus <= 100; ++modulus) {
        for (int n = 0; n <= 30; ++n) {
            for (int k = 0; k <= n; ++k) {
                assert(exlucas(n, k, modulus) == choose_mod(n, k, modulus));
            }
        }
    }
    static_assert(power_mod(2, 10, 1000) == 24);
    assert(power_mod(-2LL, 3LL, 5LL) == 2);
    assert(power_mod(7ULL, 0ULL, 1ULL) == 0);
    assert(multiply_mod(-2LL, 3LL, 5LL) == 4);
    const u64 large_mod = ~0ULL;
    assert(multiply_mod(large_mod - 1, large_mod - 1, large_mod) == 1);
    assert(is_prime_mr(2305843009213693951LL));
    assert(!is_prime_mr(341550071728321LL));
    assert(factorize(1LL << 60) == std::vector<i64>(60, 2));
    Min25 sieve(100);
    i64 prime_count = 0;
    i64 prime_sum = 0;
    for (int n = 2; n <= 100; ++n) {
        if (is_prime_mr(n)) {
            ++prime_count;
            prime_sum += n;
        }
    }
    assert(sieve.prime_count() == prime_count);
    assert(sieve.prime_sum() == prime_sum);
}

void test_linear_algebra() {
	Matrix<double, 0, 0> matrix(2, 3, {1, 1, 3, 1, -1, 1});
	auto matrix_result = gaussian_elimination(matrix, 2);
	assert(matrix_result.consistent && std::fabs(matrix_result.particular[0] - 2) < 1E-9);
    auto unique = gaussian_elimination<double>({{1, 1, 3}, {1, -1, 1}}, 2);
    assert(unique.consistent && unique.rank == 2);
    assert(std::fabs(unique.particular[0] - 2) < 1E-9);
    auto infinite = gaussian_elimination<double>({{1, 1, 2}, {2, 2, 4}}, 2);
    assert(infinite.consistent && infinite.nullspace.size() == 1);
    assert(!gaussian_elimination<double>({{1, 1, 2}, {1, 1, 3}}, 2).consistent);
    using Z = ModInt<int, 998244353>;
    std::vector<Z> sequence(40);
    sequence[1] = 1;
    for (int i = 2; i < (int)sequence.size(); ++i) {
        sequence[i] = sequence[i - 1] + sequence[i - 2];
    }
    const auto recurrence = berlekamp_massey(sequence);
    assert(recurrence.size() == 2);
    std::vector<Z> expected(101);
    expected[1] = 1;
    for (int i = 2; i <= 100; ++i) {
        expected[i] = expected[i - 1] + expected[i - 2];
    }
    assert(linear_recurrence_nth<Z>({0, 1}, {1, 1}, 100) == expected[100]);
    for (int prime : {3, 5, 7, 11, 998244353}) {
        const long long root = primitive_root_prime(prime);
        assert(root >= 1);
        for (int x = 0; x < std::min(prime, 100); ++x) {
            const long long square = (i64)((i128)x * x % prime);
            const long long result = modular_sqrt(square, prime);
            assert(result >= 0 && (i64)((i128)result * result % prime) == square);
        }
    }
}

void test_matrix_tree() {
	using Edge = MatrixTreeEdge<long long>;
	assert(matrix_tree(1, std::vector<std::pair<int, int>>{}) == 1);
	assert(matrix_tree(3, std::vector<std::pair<int, int>>{{0, 1}, {1, 2}, {0, 2}}) == 3);
	assert(matrix_tree(3, std::vector<std::pair<int, int>>{{0, 1}}) == 0);
	assert(matrix_tree(2, std::vector<Edge>{{0, 0, 7}, {0, 1, 2}}) == 2);
	assert(matrix_tree(3, std::vector<Edge>{{0, 1, 2}, {1, 2, 3}, {0, 2, 5}}) == 31);
	using Z = ModInt<int, 998244353>;
	assert(matrix_tree(3, std::vector<MatrixTreeEdge<Z>>{{0, 1, Z(2)}, {1, 2, Z(3)}, {0, 2, Z(5)}}).data() == 31);
}

void test_lgv() {
    assert(lgv<long long>(0, [](int, int) -> long long {
        assert(false); // The empty family must not query any path counts.
        return 0;
    }) == 1);

    // Right/up paths from (0, 0), (0, 1) to (2, 1), (2, 2).
    // The three disjoint pairs of move sequences are (RRU, RUR),
    // (RRU, URR), and (RUR, URR); the swapped matching is impossible.
    const std::vector<std::pair<int, int>> sources{{0, 0}, {0, 1}};
    const std::vector<std::pair<int, int>> sinks{{2, 1}, {2, 2}};
    const auto paths = [&](int i, int j) {
        const int dx = sinks[j].first - sources[i].first;
        const int dy = sinks[j].second - sources[i].second;
        if (dx < 0 || dy < 0) return 0LL;
        return choose_mod(dx + dy, dx, 998244353);
    };
    assert(lgv<long long>(2, paths) == 3);
    assert(lgv<long long>(1, [&](int i, int j) { return paths(i, j + 1); }) == 6);
    assert(lgv<long long>(2, [&](int i, int j) { return paths(i, 1 - j); }) == -3);

    // Horizontal edges weigh 2, vertical edges weigh 3. Each disjoint pair
    // has four horizontal edges and two vertical edges: 3 * 2^4 * 3^2 = 432.
    assert(lgv<long long>(2, [&](int i, int j) {
        const int dx = sinks[j].first - sources[i].first;
        const int dy = sinks[j].second - sources[i].second;
        return paths(i, j) * power(2LL, dx) * power(3LL, dy);
    }) == 432);

    // The all-ones path-count matrix has two permutation terms with opposite signs.
    assert(lgv<long long>(2, [](int, int) { return 1LL; }) == 0);
    assert(lgv<long long>(2, [](int, int) { return 0LL; }) == 0);
    using Z = ModInt<int, 998244353>;
    assert(lgv<Z>(2, [&](int i, int j) { return Z(paths(i, j)); }) == Z(3));
    assert(lgv<Z>(2, [&](int i, int j) { return Z(paths(i, 1 - j)); }) == Z(-3));
}

int main() {
    test_modular_arithmetic();
    test_sieves_and_congruences();
    test_number_theory();
    test_linear_algebra();
    test_matrix_tree();
    test_lgv();

    {
    BigInt big("12345678901234567890");
    assert((big + BigInt(10) - BigInt(10)) == big);
    assert((big / BigInt(10)).to_i64() == 1234567890123456789LL);
    assert((big % BigInt(10)).to_i64() == 0);

    assert(euler_phi(12) == 4 && euler_power(2, 10, 9) == 7);
    assert(decimal_mod("123456789", 1000) == 789);
    assert(decimal_at_least("100", 99) && !decimal_at_least("9", 10));

    assert(is_prime_mr(2) && is_prime_mr(1000000007LL) && !is_prime_mr(1000000008LL));

    }

    {
    assert(bsgs(2, 3, 5) == 3);
    using Z = ModInt<int, 998244353>;
    StirlingNumbers<Z> numbers(10);
    assert(numbers.first(4, 2) == Z(11));
    assert(numbers.second(4, 2) == Z(7));
    assert(numbers.first(5, 0) == Z(0));
    }

    {
    using P = Polynomial<int, 998244353>;
    using Z = P::Z;
    P f({Z(1), Z(1)});
    P recovered = f.logarithm(8).exponential(8);
    assert(recovered[0] == Z(1) && recovered[1] == Z(1));
    for (int i = 2; i < 8; ++i) assert(recovered[i] == Z(0));
    P cube = f.power(3, 8);
    assert(cube[0] == Z(1) && cube[1] == Z(3) && cube[2] == Z(3) && cube[3] == Z(1));
    P square({Z(1), Z(2), Z(1)});
    P root = square.square_root(8);
    P product = (root * root).truncated(8);
    assert(product[0] == Z(1) && product[1] == Z(2) && product[2] == Z(1));
    }
}
