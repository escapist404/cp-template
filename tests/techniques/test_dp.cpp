#include <algorithm>
#include <cassert>
#include <functional>
#include <limits>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include "../../include/divide_conquer_dp.hpp"
#include "../../include/wqs_binary_search.hpp"

std::mt19937 rng(424242);
int random_int(int n) { return rng() % n; }

void test_dp() {
    long long inf=std::numeric_limits<long long>::max();
    for(int n=0;n<=30;++n)for(int groups=0;groups<=n+1;++groups) {
        std::vector<long long> p(n+1);
        for(int i=0;i<n;++i)p[i+1]=p[i]+random_int(10);
        auto cost=[&](int l,int r){return (p[r]-p[l])*(p[r]-p[l]);};
        auto fast=divide_conquer_dp<long long>(groups,n,cost,inf);
        std::vector<long long> dp(n+1,inf);dp[0]=0;
        for(int g=1;g<=groups;++g) {
            std::vector<long long> ndp(n+1,inf);
            for(int i=1;i<=n;++i)for(int j=0;j<i;++j)if(dp[j]!=inf)
                ndp[i]=std::min(ndp[i],dp[j]+cost(j,i));
            dp=ndp;
        }
        assert(fast==dp);
    }
}

void test_wqs() {
    // Select k independent items: sorted marginal costs are discrete convex.
    for(int n=1;n<=40;++n) {
        std::vector<long long> a(n);for(auto& x:a)x=random_int(41)-20;
        std::sort(a.begin(),a.end());
        auto solve=[&](long long p) {
            std::pair<int,long long> result{0,0};
            for(auto x:a)if(x+p<=0)++result.first,result.second+=x+p;
            return result;
        };
        for(int k=0;k<=n;++k) {
            auto got=wqs_binary_search<long long>(-21,21,k,solve);
            assert(got==std::accumulate(a.begin(),a.begin()+k,0LL));
        }
    }
}

int main() {
    test_dp();
    test_wqs();
}
