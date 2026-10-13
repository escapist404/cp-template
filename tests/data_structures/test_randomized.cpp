#include <algorithm>
#include <cassert>
#include <functional>
#include <limits>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include "../../include/sparse_table.hpp"
#include "../../include/segment_tree_beats.hpp"
#include "../../include/weighted_dsu.hpp"
#include "../../include/implicit_treap.hpp"
#include "../../include/binary_trie.hpp"

std::mt19937 rng(424242);
int random_int(int n) { return rng() % n; }

void test_sparse() {
    auto minimum = [](int a, int b) { return std::min(a, b); };
    auto gcd = [](int a, int b) { return std::gcd(a, b); };
    for (int n = 1; n <= 65; ++n) {
        std::vector<int> a(n);
        std::vector<std::string> words(n);
        for (int i = 0; i < n; ++i) a[i] = random_int(100), words[i] = char('a' + i % 26);
        SparseTable<int> mx(a);
        SparseTable<int, decltype(minimum)> mn(a, minimum);
        SparseTable<int, decltype(gcd)> gc(a, gcd);
        DisjointSparseTable<int, std::plus<int>> sum(a);
        DisjointSparseTable<std::string, std::plus<std::string>> text(words);
        for (int l = 0; l < n; ++l) for (int r = l + 1; r <= n; ++r) {
            assert(mx.query(l, r) == *std::max_element(a.begin()+l, a.begin()+r));
            assert(mn.query(l, r) == *std::min_element(a.begin()+l, a.begin()+r));
            int g = 0;
            for (int i = l; i < r; ++i) g = std::gcd(g, a[i]);
            assert(gc.query(l, r) == g);
            assert(sum.query(l, r) == std::accumulate(a.begin()+l, a.begin()+r, 0));
            assert(text.query(l, r) == std::accumulate(words.begin()+l, words.begin()+r, std::string()));
        }
    }
    SparseTable<int> empty({});
    DisjointSparseTable<int> empty2({});
}

void test_beats() {
    SegmentTreeBeats<long long> empty({});
    empty.add(0, 0, 3); empty.chmin(0, 0, 5); empty.chmax(0, 0, -2);
    assert(empty.sum(0, 0) == 0);
    for (int n = 1; n <= 40; ++n) {
        std::vector<long long> a(n);
        for (auto& x : a) x = random_int(31) - 15;
        SegmentTreeBeats<long long> st(a);
        for (int q = 0; q < 1200; ++q) {
            int l = random_int(n + 1), r = random_int(n + 1);
            if (l > r) std::swap(l, r);
            long long x = random_int(101) - 50;
            int op = random_int(4);
            if (op == 0) {
                st.add(l,r,x);
                for (int i=l;i<r;++i) a[i]+=x;
            } else if (op == 1) {
                st.chmin(l,r,x);
                for (int i=l;i<r;++i) a[i]=std::min(a[i],x);
            } else if (op == 2) {
                st.chmax(l,r,x);
                for (int i=l;i<r;++i) a[i]=std::max(a[i],x);
            }
            assert(st.sum(l,r) == std::accumulate(a.begin()+l,a.begin()+r,0LL));
            if (l<r) {
                assert(st.min(l,r)==*std::min_element(a.begin()+l,a.begin()+r));
                assert(st.max(l,r)==*std::max_element(a.begin()+l,a.begin()+r));
            }
        }
    }
    SegmentTreeBeats<long long> high(std::vector<long long>{std::numeric_limits<long long>::max()});
    high.chmax(0,1,std::numeric_limits<long long>::max());
    assert(high.max(0,1)==std::numeric_limits<long long>::max());
}

void test_weighted() {
    for (int n=1;n<=40;++n) {
        WeightedDSU<long long> dsu(n);
        std::vector<int> label(n);
        std::iota(label.begin(),label.end(),0);
        std::vector<long long> potential(n);
        for(auto& x:potential) x=random_int(101)-50;
        for(int q=0;q<400;++q) {
            int u=random_int(n),v=random_int(n);
            long long w=potential[v]-potential[u];
            if (label[u]==label[v]) {
                assert(dsu.difference(u,v)==w);
                assert(!dsu.merge(u,v,w+1));
            } else {
                assert(dsu.merge(u,v,w));
                int old=label[v];
                for(auto& x:label) if(x==old) x=label[u];
            }
            for(int i=0;i<n;++i) assert(dsu.same(u,i)==(label[u]==label[i]));
        }
    }
}

void test_treap() {
    ImplicitTreap<int> sum;
    ImplicitTreap<std::string> text;
    std::vector<int> a;
    std::vector<std::string> b;
    for(int q=0;q<5000;++q) {
        int n=a.size(),op=random_int(6);
        int l=random_int(n+1),r=random_int(n+1);
        if(l>r) std::swap(l,r);
        if(op==0 || !n) {
            int value=random_int(100);
            std::string word(1,'a'+value%26);
            sum.insert(l,value);text.insert(l,word);
            a.insert(a.begin()+l,value);b.insert(b.begin()+l,word);
        } else if(op==1) {
            sum.erase(l,r);text.erase(l,r);
            a.erase(a.begin()+l,a.begin()+r);b.erase(b.begin()+l,b.begin()+r);
        } else if(op==2) {
            sum.reverse(l,r);text.reverse(l,r);
            std::reverse(a.begin()+l,a.begin()+r);std::reverse(b.begin()+l,b.begin()+r);
        } else if(op==3) {
            int p=random_int(n);
            sum.set(p,7);text.set(p,"xx");a[p]=7;b[p]="xx";
        } else if(op==4) {
            int p=random_int(n-(r-l)+1);
            sum.move(l,r,p);text.move(l,r,p);
            std::vector<int> cut(a.begin()+l,a.begin()+r);
            std::vector<std::string> cut2(b.begin()+l,b.begin()+r);
            a.erase(a.begin()+l,a.begin()+r);b.erase(b.begin()+l,b.begin()+r);
            a.insert(a.begin()+p,cut.begin(),cut.end());b.insert(b.begin()+p,cut2.begin(),cut2.end());
        } else {
            assert(sum.query(l,r)==std::accumulate(a.begin()+l,a.begin()+r,0));
            assert(text.query(l,r)==std::accumulate(b.begin()+l,b.begin()+r,std::string()));
        }
        assert(sum.size()==(int)a.size() && text.size()==(int)b.size());
        for(int i=0;i<(int)a.size();++i) assert(sum.get(i)==a[i] && text.get(i)==b[i]);
    }
    auto minimum=[](int x,int y){return std::min(x,y);};
    ImplicitTreap<int,decltype(minimum)> mn({3,1,5},minimum,1000000);
    assert(mn.query(0,3)==1 && mn.query(1,1)==1000000);
}

void test_trie() {
    BinaryTrie<unsigned long long> trie;
    std::vector<unsigned long long> a;
    for(int q=0;q<3000;++q) {
        auto x=((unsigned long long)rng()<<32)|rng();
        if(a.empty() || random_int(3)==0) {trie.insert(x);a.push_back(x);}
        else if(random_int(2)==0) {
            int p=random_int(a.size());trie.erase(a[p]);a.erase(a.begin()+p);
        } else {
            unsigned long long lo=~0ULL,hi=0;
            for(auto y:a) lo=std::min(lo,x^y),hi=std::max(hi,x^y);
            assert(trie.min_xor(x)==lo && trie.max_xor(x)==hi);
        }
        assert(trie.size()==(int)a.size());
        for(auto y:a) assert(trie.count(y)==(int)std::count(a.begin(),a.end(),y));
    }
    BinaryTrie<unsigned,4> small;
    small.insert(0);small.insert(0);small.insert(15);
    assert(small.count(0)==2);small.erase(0);assert(small.count(0)==1);
    assert(small.max_xor(0)==15 && small.min_xor(15)==0);
}

int main() {
    test_sparse();
    test_beats();
    test_weighted();
    test_treap();
    test_trie();
}
