This file provides guidance to agents when working with code in this repository.

Repository Overview

This is a competitive programming template library (cp-template) containing 116 algorithm implementations in C++ header files with detailed Chinese interface and complexity comments embedded directly in each header. The library is designed for contest use, prioritizing hand-writing speed and simplicity over engineering practices.

Key Design Principles

Contest-Optimized Code Style:
- Minimal boilerplate: avoid excessive const, constexpr, explicit, static_cast
- Short, clear code that's fast to type by hand
- C-style casts (T) instead of static_cast<T>
- Pass small types by value, not const&
- No unnecessary inline or const member functions
- Single-file, header-only modules

Target Environment:
- C++17 standard
- Print-friendly: designed to be printed and hand-copied during contests
- 0-indexed or 1-indexed clearly documented per module
- No namespaces

Project Structure

cp-template/
├── include/           # 116 algorithm header files (.hpp)
├── tools/            # PDF generation tool
│   └── code2pdf.py  # Generates printable PDF with table of contents
├── tests/            # Test files
├── codebook.conf     # Controls which files to print and their order
└── README.md         # Main documentation with module catalog

Core Categories

Data Structures (23 files): DSU, Fenwick, SegmentTree, LazySegmentTree, Treap, ScapegoatTree, ODT, LinearBasis, WaveletTree, etc.

Graph Algorithms (25 files): SCC, EDCC, VDCC, Dijkstra, MaxFlow, MCMF, HLD, BipartiteMatching, KM, TreeHash, etc.

String Algorithms (12 files): KMP, SuffixArray, AhoCorasick, Manacher, RollingHash, PalindromicTree, etc.

Number Theory (15 files): ModInt, MillerRabin, PollardRho, CRT, Lucas, Combinatorics, etc.

Math (12 files): Matrix, Polynomial, Geometry (10 sub-modules), BigInt, FloorSum, LGV, etc.

Algorithm Frameworks (12 files): CDQ, Mo's Algorithm, Binary Search variants, LiChao Tree, etc.

Common Commands

Generate Printable PDF

# Generate combined Docs + Code PDF with all 116 modules (uses codebook.conf)
make pdf
python3 tools/code2pdf.py include/ -o codebook.pdf -c codebook.conf --black-and-white

# Without config (auto-discovers all files alphabetically)
python3 tools/code2pdf.py include/ -o codebook.pdf

Testing

# Run all tests
./run_tests.sh

# Stress test
./stress_test.sh

Documentation

All .hpp files have corresponding .md documentation in docs/ following ac-library style:
- Interface signatures with parameter constraints
- Complexity analysis
- Usage examples
- Known pitfalls ("使用坑")
- Cross-references

Code Simplification Guidelines

When modifying competitive programming code in this repository:

DO simplify:
- Remove const from function parameters (except string literals)
- Remove constexpr from functions
- Remove explicit from constructors
- Replace static_cast<T>(x) with (T)x
- Remove unnecessary const member functions
- Pass small types (int, i64, pointers) by value, not const&

DO NOT change:
- Algorithm correctness or semantics
- Index conventions (0-based vs 1-based must stay consistent)
- Public API function signatures (names, parameter order)
- Dependencies between modules

Example transformation:
// Before (engineering style)
constexpr S& operator+=(const S& rhs) & {
    return number = static_cast<T>(value), *this;
}

// After (contest style)
S& operator+=(S rhs) {
    return number = (T)value, *this;
}

Important Files

codebook.conf

Controls PDF generation: which files to include and their order. Format:
- [Section Name] for category headers
- One file path per line (relative to include/)
- # for comments
- Supports 17 logical categories covering all 116 configured modules

types.hpp

Base type definitions used across many modules:
using i64 = long long;
using u64 = unsigned long long;
using i128 = __int128_t;
using u128 = __uint128_t;
using Real = double;
using Complex = std::complex<Real>;

Module-Specific Notes

ModInt (modint.hpp): Division only works with prime modulus. Uses Fermat's little theorem for inverse.

Fenwick (fenwick.hpp): 1-indexed internally (users pass 0-indexed, converted to 1-indexed).

LazySegmentTree (lazy_segtree.hpp): Tag semantics are value += lazy * segment_length.

SuffixArray (suffix_array.hpp): lcp(i, j) computes LCP of positions i and j, not ranks.

KMP (kmp.hpp): Parameter order is kmp(pattern, text), not kmp(text, pattern).

Trie (trie.hpp): Only supports 26 lowercase English letters.

Z-Algorithm (z_algo.hpp): z[0] = n (differs from some textbooks that use z[0] = 0).

PalindromicTree (palindromic_tree.hpp): Two roots - node 0 (odd root, length=-1) and node 1 (even root, length=0).

CRT (crt.hpp): Returns {remainder, modulus}. If modulus == 0, no solution exists.

Git Workflow

Always commit as escapist404 <123872320+escapist404@users.noreply.github.com>, for both author and committer. Push this repository to escapist404/cp-template.

This is a personal template library. Commits should:
- Group related simplifications together
- Note which files were modified
- Explain what type of bloat was removed (e.g., "Remove const parameters and constexpr")

Do not commit:
- Generated PDFs (.pdf files are gitignored)
- Temporary files in tmp/
- Test binaries

Testing Coverage

Current test suite: 20 test files under tests/, run with ASan/UBSan. Per-module coverage is not tracked as a reliable percentage because several tests exercise multiple headers.

When adding new modules or modifying existing ones, add corresponding tests if the module doesn't have coverage yet.
