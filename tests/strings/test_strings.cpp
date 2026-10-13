#include "../../include/exkmp.hpp"
#include "../../include/lyndon.hpp"

#include "../../include/minimal_rotation.hpp"

#include <cassert>
#include <string>
#include "../../include/ac_automaton.hpp"
#include "../../include/kmp.hpp"
#include "../../include/manacher.hpp"
#include "../../include/palindromic_tree.hpp"
#include "../../include/rolling_hash.hpp"
#include "../../include/suffix_array.hpp"
#include "../../include/suffix_automaton.hpp"
#include "../../include/trie.hpp"
#include "../../include/z_algo.hpp"

int main() {
    assert((kmp("aba", "ababa") == std::vector<int>{0, 2}));
    assert(kmp("", "abc").empty());
    AhoCorasick ac;
    const int first = ac.insert("he");
    const int second = ac.insert("she");
    ac.build();
    const auto matches = ac.query("she");
    assert(matches[first] == 1 && matches[second] == 1);
    ac.build();
    assert(z_algorithm("").empty());
    assert(z_algorithm("aaaa").size() == 4 && z_algorithm("aaaa")[1] == 3);
    assert(manacher("abacaba").size() == 15);
    RollingHash hash("abcabc");
    assert(hash.equal(0, 3, 3, 6));
    SuffixArray suffix_array(std::string("banana"));
    assert(suffix_array.array().front() == 5);
    assert(suffix_array.lcp(1, 3) == 3);
    SuffixAutomaton<char> sam;
    sam.build(std::string("ababa"));
    assert(sam.contains(std::string("bab")));
    PalindromicTree<char> palindromes;
    palindromes.build(std::string("ababa"));
    assert(palindromes.distinct_palindromes() == 5);
    Trie trie;
    trie.insert("abc");
    trie.insert("abd");
    assert(trie.query("abc"));

    {
	assert(minimal_rotation_string("bbaaccaadd") == "aaccaaddbb");
	assert(maximal_rotation_string("bbaaccaadd") == "ddbbaaccaa");


    }

    {
    const auto lcp = exkmp(std::string("aba"), std::string("abacaba"));
    assert((lcp == std::vector<int>{3, 0, 1, 0, 3, 0, 1}));
    const std::string value = "ababbab";
    std::string rebuilt;
    for (auto [l, r] : lyndon_factorization(value)) rebuilt += value.substr(l, r - l);
    assert(rebuilt == value);
    }
}
