#ifndef PALINDROMIC_TREE_HPP
#define PALINDROMIC_TREE_HPP

#include <algorithm>
#include <functional>
#include <map>
#include <vector>

// 【回文树 / 回文自动机 Palindromic Tree / Eertree (palindromic_tree.hpp)】
// - 维护字符串的所有本质不同回文子串，状态数 <= n + 2，构建复杂度 O(n log sigma)
// - 双根约定 : 节点 0 为奇根（length = -1, link = 0），节点 1 为偶根（length = 0, link = 0）；真实回文串状态从下标 2 开始
// - extend(c): 在末尾追加字符并返回当前最长回文后缀所在的状态编号
// - accumulate(): 沿后缀链接自底向上累加 occurrences，调用后 nodes[u].occurrences 即为该回文串在原串中的总出现次数
// - distinct_palindromes(): 返回本质不同非空回文子串个数 (nodes.size() - 2)

template <typename Symbol = char, typename Compare = std::less<Symbol>>
class PalindromicTree {
public:
    struct Node {
        int length = 0, link = 0, parent = -1;
        std::map<Symbol, int, Compare> next;
        long long terminals = 0;
    };

private:
    std::vector<Node> nodes;
    std::vector<Symbol> text;
    Compare compare;
    int last = 1;

    int suffix(int node) const {
        int p = text.size() - 1;
        while (true) {
            int left = p - nodes[node].length - 1;
            if (left >= 0 && !compare(text[left], text[p]) && !compare(text[p], text[left])) {
                return node;
            }
            node = nodes[node].link;
        }
    }

public:
    PalindromicTree() : nodes(2) {
        nodes[0].length = -1;
        nodes[1].link = 0;
    }

    int add(const Symbol& symbol) {
        text.push_back(symbol);
        int p = suffix(last);
        auto it = nodes[p].next.find(symbol);
        if (it != nodes[p].next.end()) {
            last = it->second;
        } else {
            int cur = nodes.size();
            Node node;
            node.length = nodes[p].length + 2;
            node.parent = p;
            if (node.length == 1) {
                node.link = 1;
            } else {
                node.link = nodes[suffix(nodes[p].link)].next.at(symbol);
            }
            nodes.push_back(std::move(node));
            nodes[p].next[symbol] = cur;
            last = cur;
        }
        ++nodes[last].terminals;
        return last;
    }

    template <typename Sequence>
    void build(const Sequence& sequence) {
        *this = PalindromicTree{};
        for (const auto& symbol : sequence) {
            add(symbol);
        }
    }

    int distinct_palindromes() const {
        return nodes.size() - 2;
    }

    int longest_suffix() const {
        return nodes[last].length;
    }

    std::vector<long long> occurrences() const {
        std::vector<int> order;
        std::vector<long long> result;
        for (int i = 0; i < (int)nodes.size(); ++i) {
            result.push_back(nodes[i].terminals);
            if (i >= 2) {
                order.push_back(i);
            }
        }
        std::sort(order.begin(), order.end(), [&](int a, int b) {
            return nodes[a].length > nodes[b].length;
        });
        for (int u : order) {
            result[nodes[u].link] += result[u];
        }
        return result;
    }

    const std::vector<Node>& states() const {
        return nodes;
    }
};
#endif
