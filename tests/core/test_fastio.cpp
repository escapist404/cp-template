#include <cassert>
#include <climits>
#include <cstdio>
#include <cstring>
#include "../../include/fastio.hpp"
int main() {
    FILE* f = tmpfile(); assert(f);
    { OptimizedIO<1024> out(f, f); out << INT_MIN << ' ' << INT_MAX << ' ' << 0 << ' ' << -1 << ' ' << 42; }
    rewind(f);
    { OptimizedIO<1024> in(f, stdout); int a,b,c,d,e; in >> a >> b >> c >> d >> e; assert(a==INT_MIN&&b==INT_MAX&&c==0&&d==-1&&e==42); }
    fclose(f);
    f = tmpfile(); assert(f);
    { OptimizedIO<1024> out(f, f); out << 1000000000 << ' ' << -1000000000; }
    rewind(f);
    { OptimizedIO<1024> in(f, stdout); int a,b; in >> a >> b; assert(a==1000000000&&b==-1000000000); }
    fclose(f);
    f = tmpfile(); assert(f);
    { OptimizedIO<1024> out(f, f); out << "Hello" << ' ' << "World"; }
    rewind(f); char buf[32]; assert(fgets(buf,sizeof(buf),f)); assert(strcmp(buf,"Hello World")==0); fclose(f);
    f = tmpfile(); assert(f); fprintf(f,"123 -456 789 0\n"); rewind(f);
    { OptimizedIO<1024> in(f, stdout); int a,b,c,d; in >> a >> b >> c >> d; assert(a==123&&b==-456&&c==789&&d==0); }
    fclose(f);
}
