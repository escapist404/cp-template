#include <algorithm>
#include <cassert>
#include <functional>
#include <limits>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include "../../include/fwt.hpp"
#include "../../include/modint.hpp"

std::mt19937 rng(424242);
int random_int(int n) { return rng() % n; }

template<typename T>
void check_fwt(std::vector<T> a,std::vector<T> b) {
    int n=a.size();
    std::vector<T> cx(n),co(n),ca(n);
    for(int i=0;i<n;++i)for(int j=0;j<n;++j) {
        cx[i^j]+=a[i]*b[j];co[i|j]+=a[i]*b[j];ca[i&j]+=a[i]*b[j];
    }
    assert((fwt_convolution<T,fwt_xor<T>>(a,b)==cx));
    assert((fwt_convolution<T,fwt_or<T>>(a,b)==co));
    assert((fwt_convolution<T,fwt_and<T>>(a,b)==ca));
    auto x=a;fwt_xor(x);fwt_xor(x,true);assert(x==a);
    x=a;fwt_or(x);fwt_or(x,true);assert(x==a);
    x=a;fwt_and(x);fwt_and(x,true);assert(x==a);
}
void test_fwt() {
    check_fwt<long long>({},{});
    using Z=ModInt<int,998244353>;
    for(int n=1;n<=64;n*=2) {
        std::vector<long long>a(n),b(n);
        for(auto& x:a)x=random_int(31)-15;
        for(auto& x:b)x=random_int(31)-15;
        check_fwt(a,b);
        check_fwt(std::vector<Z>(a.begin(),a.end()),std::vector<Z>(b.begin(),b.end()));
    }
}

int main() {
    test_fwt();
}
