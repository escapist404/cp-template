#ifndef SUFFIX_AUTOMATON_HPP
#define SUFFIX_AUTOMATON_HPP

#include <algorithm>
#include <functional>
#include <map>
#include <vector>

// 【后缀自动机 Suffix Automaton / SAM (suffix_automaton.hpp)】
// - 在线构建字符串的所有子串状态图，状态数 <= 2n - 1，转移数 <= 3n - 4，构建复杂度 O(n log sigma)
// - Node 字段 : length（最长子串长度）、link（后缀链接，根 0 的 link = -1）、first_position（首次出现的结束位置）、terminals（endpos 大小）
// - extend(c) : 在末尾追加一个字符 c
// - accumulate(): 按长度降序沿后缀链接树自底向上累加 terminals，调用后 nodes[u].terminals 即为状态 u 对应子串在原串中的出现次数
// - distinct_substrings(): O(状态数) 返回原串本质不同非空子串的总数 sum(length[u] - length[link[u]])

template <typename Symbol = char, typename Compare = std::less<Symbol>>
class SuffixAutomaton {
public:
    struct Node {
        int length = 0, link = -1, first_position = -1;
        std::map<Symbol, int, Compare> next;
        long long terminals = 0;
    };

private:
    std::vector<Node> nodes{Node{}};
    int last = 0;

public:
    int add(const Symbol& symbol) {
        int cur = nodes.size();
        nodes.push_back(Node{});
        nodes[cur].length = nodes[last].length + 1;
        nodes[cur].first_position = nodes[cur].length - 1;
        nodes[cur].terminals = 1;
        int p = last;
        while (p != -1 && !nodes[p].next.count(symbol)) {
            nodes[p].next[symbol] = cur;
            p = nodes[p].link;
        }
        if (p == -1) {
            nodes[cur].link = 0;
        } else {
            int q = nodes[p].next[symbol];
            if (nodes[p].length + 1 == nodes[q].length) {
                nodes[cur].link = q;
            } else {
                int clone = nodes.size();
                Node copy = nodes[q];
                copy.length = nodes[p].length + 1;
                copy.terminals = 0;
                nodes.push_back(std::move(copy));
                while (p != -1) {
                    auto it = nodes[p].next.find(symbol);
                    if (it == nodes[p].next.end() || it->second != q) {
                        break;
                    }
                    it->second = clone;
                    p = nodes[p].link;
                }
                nodes[q].link = nodes[cur].link = clone;
            }
        }
        last = cur;
        return cur;
    }

    template <typename Sequence>
    void build(const Sequence& text) {
        nodes.assign(1, Node{});
        last = 0;
        for (const auto& symbol : text) {
            add(symbol);
        }
    }

    template <typename Sequence>
    bool contains(const Sequence& text) const {
        int node = 0;
        for (const auto& symbol : text) {
            auto it = nodes[node].next.find(symbol);
            if (it == nodes[node].next.end()) {
                return false;
            }
            node = it->second;
        }
        return true;
    }

    long long distinct_substrings() const {
        long long result = 0;
        for (int i = 1; i < (int)nodes.size(); ++i) {
            result += nodes[i].length - nodes[nodes[i].link].length;
        }
        return result;
    }

    // A fresh vector each call, so propagation is repeatable after more adds.
    std::vector<long long> occurrences() const {
        std::vector<int> order(nodes.size());
        std::vector<long long> result;
        for (int i = 0; i < (int)nodes.size(); ++i) {
            order[i] = i;
            result.push_back(nodes[i].terminals);
        }
        std::sort(order.begin(), order.end(), [&](int a, int b) {
            return nodes[a].length > nodes[b].length;
        });
        for (int u : order) {
            if (nodes[u].link != -1) {
                result[nodes[u].link] += result[u];
            }
        }
        return result;
    }

    const std::vector<Node>& states() const {
        return nodes;
    }
};
#endif
