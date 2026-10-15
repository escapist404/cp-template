#ifndef SUFFIX_AUTOMATON_HPP
#define SUFFIX_AUTOMATON_HPP

#include <algorithm>
#include <functional>
#include <map>
#include <vector>

// ============================================================================
// 【后缀自动机 Suffix Automaton / SAM (suffix_automaton.hpp)】
// ----------------------------------------------------------------------------
// 模板类签名：template <typename Symbol = char, typename Compare = std::less<Symbol>> class SuffixAutomaton
//   在线构建一般字符集序列的后缀自动机（状态总数 <= 2n - 1，转移边数 <= 3n - 4；0 号节点为初始根状态，link = -1）。
// 公开状态节点结构体签名：
//   struct Node {
//       std::map<Symbol, int, Compare> next; // 字符转移表
//       int link = -1;                       // 后缀链接（Parent 树父节点编号，根状态 0 的 link 为 -1）
//       int length = 0;                      // 该等价类中最长子串的长度 len(u)（最短子串长度为 len(link) + 1）
//       int terminals = 0;                   // 非克隆前缀状态标记（实建状态为 1，克隆状态与根为 0，用于统计 endpos 大小）
//   };
// 构造函数与成员函数签名详解：
//   - int add(const Symbol& symbol)
//     功能：在线追加单个字符 symbol，更新自动机并返回新的整个前缀所在状态编号 last。均摊 O(log |Sigma|)。
//   - template <typename Sequence> void build(const Sequence& text)
//     功能：重置自动机并依次插入序列 text 中的所有元素。时间复杂度 O(|text| log |Sigma|)，空间 O(|text|)。
//   - template <typename Sequence> bool contains(const Sequence& text) const
//     功能：从根状态 0 出发沿转移边匹配，判断序列 text 是否为原串的子串。时间复杂度 O(|text| log |Sigma|)。
//   - long long distinct_substrings() const
//     功能：统计原串中本质不同非空子串的总个数，即 sum_{u > 0} (nodes[u].length - nodes[nodes[u].link].length)。O(|states|)。
//   - std::vector<long long> occurrences() const
//     功能：按 length 降序沿后缀链接 link 向上累加 terminals，返回每个状态 u 的出现次数（即 |endpos(u)|）数组。O(|states| log |states|)。
//   - const std::vector<Node>& states() const
//     功能：返回内部所有状态节点数组 nodes 的常引用（下标 0 为根状态）。O(1)。
// ============================================================================

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
    // int add(const Symbol& symbol) — 功能：在线追加单个字符 symbol，更新自动机并返回新的整个前缀所在状态编号 last。均摊 O(log |Sigma|)。
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

    // template <typename Sequence> void build(const Sequence& text) — 功能：重置自动机并依次插入序列 text 中的所有元素。时间复杂度 O(|text| log |Sigma|)，空间 O(|text|)。
    template <typename Sequence>
    void build(const Sequence& text) {
        nodes.assign(1, Node{});
        last = 0;
        for (const auto& symbol : text) {
            add(symbol);
        }
    }

    // template <typename Sequence> bool contains(const Sequence& text) const — 功能：从根状态 0 出发沿转移边匹配，判断序列 text 是否为原串的子串。时间复杂度 O(|text| log |Sigma|)。
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

    // long long distinct_substrings() const — 功能：统计原串中本质不同非空子串的总个数，即 sum_{u > 0} (nodes[u].length - nodes[nodes[u].link].length)。O(|states|)。
    long long distinct_substrings() const {
        long long result = 0;
        for (int i = 1; i < (int)nodes.size(); ++i) {
            result += nodes[i].length - nodes[nodes[i].link].length;
        }
        return result;
    }

    // 每次调用返回全新的出现次数数组，支持继续 extend 字符后重复调用。
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

    // const std::vector<Node>& states() const — 功能：返回内部所有状态节点数组 nodes 的常引用（下标 0 为根状态）。O(1)。
    const std::vector<Node>& states() const {
        return nodes;
    }
};
#endif
