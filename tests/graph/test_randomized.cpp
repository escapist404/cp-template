#include <algorithm>
#include <cassert>
#include <functional>
#include <limits>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include "../../include/functional_graph.hpp"
#include "../../include/difference_constraints.hpp"
#include "../../include/blossom.hpp"

std::mt19937 rng(424242);
int random_int(int n) { return rng() % n; }

void test_functional() {
    FunctionalGraph empty({});
    for(int n=1;n<=60;++n) {
        std::vector<int> next(n);
        for(auto& x:next)x=random_int(n);
        FunctionalGraph g(next);
        for(int u=0;u<n;++u) {
            std::vector<int> seen(n,-1),path;
            int v=u;
            while(seen[v]==-1) {seen[v]=path.size();path.push_back(v);v=next[v];}
            int start=seen[v],len=path.size()-start;
            assert(g.distance_to_cycle(u)==start && g.cycle_length(u)==len);
            for(unsigned long long steps:{0ULL,1ULL,3ULL,100ULL,~0ULL}) {
                unsigned long long index=steps<path.size()?steps:start+(steps-start)%len;
                assert(g.jump(u,steps)==path[index]);
            }
            for(int j=start;j<(int)path.size();++j) assert(g.component(u)==g.component(path[j]));
        }
    }
}

void test_constraints() {
    assert(difference_constraints<long long>(0, {}).first);
    for(int n=1;n<=10;++n) for(int rep=0;rep<150;++rep) {
        std::vector<DifferenceConstraint<long long>> edges;
        for(int j=0;j<25;++j) {
            edges.push_back({random_int(n), random_int(n), random_int(31) - 15});
        }
        std::vector<long long> d(n);
        bool feasible=true;
        for(int step=0;step<n;++step) {
            bool change=false;
            for(auto e:edges) if(d[e.v]>d[e.u]+e.weight) {d[e.v]=d[e.u]+e.weight;change=true;}
            if(!change)break;
            if(step==n-1)feasible=false;
        }
        const auto result = difference_constraints(n, edges);
        assert(result.first == feasible);
        if(feasible)for(auto e:edges)assert(result.second[e.v]<=result.second[e.u]+e.weight);
    }
    // Many parallel relaxations do not imply a negative cycle.
    std::vector<DifferenceConstraint<int>> parallel;
    for(int w=50;w>=-50;--w)parallel.push_back({0, 1, w});
    assert(difference_constraints(2, parallel).first);
}

int brute_matching(const std::vector<std::vector<bool>>& adj,int mask) {
    if(!mask)return 0;
    int u=__builtin_ctz((unsigned)mask);
    int rest=mask^(1<<u),best=brute_matching(adj,rest);
    for(int v=u+1;v<(int)adj.size();++v)if((rest>>v&1)&&adj[u][v])
        best=std::max(best,1+brute_matching(adj,rest^(1<<v)));
    return best;
}
void test_matching() {
    for(int n=0;n<=10;++n)for(int rep=0;rep<150;++rep) {
        GeneralMatching g(n);
        std::vector<std::vector<bool>> adj(n,std::vector<bool>(n));
        for(int u=0;u<n;++u)for(int v=u+1;v<n;++v)if(random_int(3)) {
            adj[u][v]=adj[v][u]=true;g.add_edge(u,v);
            if(random_int(5)==0)g.add_edge(u,v);
        }
        int expected=brute_matching(adj,(1<<n)-1);
        assert(g.max_matching()==expected);
        assert(g.max_matching()==expected);
        auto match=g.matching();int count=0;
        for(int i=0;i<n;++i)if(match[i]!=-1) {
            assert(match[i]!=i && adj[i][match[i]] && match[match[i]]==i);++count;
        }
        assert(count==2*expected);
    }
}

int main() {
    test_functional();
    test_constraints();
    test_matching();
}
