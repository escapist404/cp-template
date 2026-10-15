#ifndef PALINDROMIC_TREE_HPP
#define PALINDROMIC_TREE_HPP

#include <algorithm>
#include <functional>
#include <map>
#include <vector>

// ============================================================================
// 【回文树 / 回文自动机 Palindromic Tree / Eertree (palindromic_tree.hpp)】
// ----------------------------------------------------------------------------
// 模板类签名：template <typename Symbol = char, typename Compare = std::less<Symbol>> class PalindromicTree
//   维护序列中所有本质不同回文子串的状态图（本质不同非空回文子串个数 <= n）。
//   【双根节点编号约定】
//   - 0 号节点：奇根（长度 length = -1，失配指针 link = 0）；
//   - 1 号节点：偶根（长度 length = 0，失配指针 link = 0）；
//   - >= 2 号节点：真实本质不同非空回文子串状态（总数恰好为 nodes.size() - 2）。
// 公开状态节点结构体签名：
//   struct Node {
//       std::map<Symbol, int, Compare> next; // 两端同时扩展字符 symbol 的转移表
//       int link = 0;                        // 指向当前回文串最长真回文后缀的状态编号
//       int length = 0;                      // 当前状态代表的回文串长度
//       int count = 0;                       // 该状态包含的不同回文后缀个数（即 fail 树深度）
//       int terminals = 0;                   // 作为新增前缀的最长回文后缀出现的次数
//   };
// 构造函数与成员函数签名详解：
//   - PalindromicTree()
//     功能：构造只含 0 号奇根与 1 号偶根的空回文树。O(1)。
//   - int add(const Symbol& symbol)
//     功能：在线追加单个字符 symbol，返回新增前缀的最长回文后缀对应的状态编号 last。均摊 O(log |Sigma|)。
//   - template <typename Sequence> void build(const Sequence& sequence)
//     功能：在当前回文树上依次追加序列 sequence 中的所有字符。时间复杂度 O(|sequence| log |Sigma|)。
//   - int distinct_palindromes() const
//     功能：返回当前已插入序列中【本质不同非空回文子串】的总数（即 nodes.size() - 2）。O(1)。
//   - int longest_suffix() const
//     功能：返回当前整个已插入序列的最长回文后缀长度（即 nodes[last].length）。O(1)。
//   - std::vector<long long> occurrences() const
//     功能：按回文长度从大到小向失配指针 link 累加 terminals，返回与 states() 等长的每个回文状态在原串中的总出现次数数组。
//   - const std::vector<Node>& states() const
//     功能：返回全部节点数组 nodes 的常引用（真实回文子串状态对应下标 i >= 2）。O(1)。
// ============================================================================

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
    // PalindromicTree() — 功能：构造只含 0 号奇根与 1 号偶根的空回文树。O(1)。
    PalindromicTree() : nodes(2) {
        nodes[0].length = -1;
        nodes[1].link = 0;
    }

    // int add(const Symbol& symbol) — 功能：在线追加单个字符 symbol，返回新增前缀的最长回文后缀对应的状态编号 last。均摊 O(log |Sigma|)。
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

    // template <typename Sequence> void build(const Sequence& sequence) — 功能：在当前回文树上依次追加序列 sequence 中的所有字符。时间复杂度 O(|sequence| log |Sigma|)。
    template <typename Sequence>
    void build(const Sequence& sequence) {
        *this = PalindromicTree{};
        for (const auto& symbol : sequence) {
            add(symbol);
        }
    }

    // int distinct_palindromes() const — 功能：返回当前已插入序列中【本质不同非空回文子串】的总数（即 nodes.size() - 2）。O(1)。
    int distinct_palindromes() const {
        return nodes.size() - 2;
    }

    // int longest_suffix() const — 功能：返回当前整个已插入序列的最长回文后缀长度（即 nodes[last].length）。O(1)。
    int longest_suffix() const {
        return nodes[last].length;
    }

    // std::vector<long long> occurrences() const — 功能：按回文长度从大到小向失配指针 link 累加 terminals，返回与 states() 等长的每个回文状态在原串中的总出现次数数组。
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

    // const std::vector<Node>& states() const — 功能：返回全部节点数组 nodes 的常引用（真实回文子串状态对应下标 i >= 2）。O(1)。
    const std::vector<Node>& states() const {
        return nodes;
    }
};
#endif
