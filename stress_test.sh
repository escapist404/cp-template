#!/bin/bash
# 算法竞赛模板库对拍脚本

set -e

cd "$(dirname "$0")"

echo "================================"
echo "算法模板对拍测试"
echo "================================"
echo ""

# 创建临时目录
STRESS_DIR="/tmp/cp_stress"
mkdir -p "$STRESS_DIR"

# 颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# 对拍单个算法
stress_test() {
    local name=$1
    local iterations=${2:-100}

    echo -n "对拍 $name ($iterations 次)... "

    if [ ! -f "$STRESS_DIR/${name}_gen.cpp" ]; then
        echo -e "${YELLOW}跳过（无生成器）${NC}"
        return
    fi

    g++ -std=c++17 -O2 "$STRESS_DIR/${name}_gen.cpp" -o "$STRESS_DIR/${name}_gen" 2>/dev/null || {
        echo -e "${RED}生成器编译失败${NC}"
        return 1
    }

    g++ -std=c++17 -O2 -I"$PWD/include" "$STRESS_DIR/${name}_my.cpp" -o "$STRESS_DIR/${name}_my" 2>/dev/null || {
        echo -e "${RED}我的实现编译失败${NC}"
        return 1
    }

    g++ -std=c++17 -O2 "$STRESS_DIR/${name}_std.cpp" -o "$STRESS_DIR/${name}_std" 2>/dev/null || {
        echo -e "${RED}标准实现编译失败${NC}"
        return 1
    }

    local failed=0
    for i in $(seq 1 $iterations); do
        "$STRESS_DIR/${name}_gen" > "$STRESS_DIR/input.txt"

        "$STRESS_DIR/${name}_my" < "$STRESS_DIR/input.txt" > "$STRESS_DIR/output_my.txt" 2>/dev/null || {
            echo -e "${RED}✗ 第 $i 次测试 - 我的实现运行错误${NC}"
            failed=1
            break
        }

        "$STRESS_DIR/${name}_std" < "$STRESS_DIR/input.txt" > "$STRESS_DIR/output_std.txt" 2>/dev/null || {
            echo -e "${RED}✗ 第 $i 次测试 - 标准实现运行错误${NC}"
            failed=1
            break
        }

        if ! diff -q "$STRESS_DIR/output_my.txt" "$STRESS_DIR/output_std.txt" >/dev/null; then
            echo -e "${RED}✗ 第 $i 次测试失败${NC}"
            echo "输入:"
            cat "$STRESS_DIR/input.txt"
            echo "我的输出:"
            cat "$STRESS_DIR/output_my.txt"
            echo "标准输出:"
            cat "$STRESS_DIR/output_std.txt"
            failed=1
            break
        fi
    done

    if [ $failed -eq 0 ]; then
        echo -e "${GREEN}✓ 通过${NC}"
        return 0
    else
        return 1
    fi
}

# DSU 对拍测试
echo "=== 1. DSU (并查集) ==="
cat > "$STRESS_DIR/dsu_gen.cpp" << 'EOF'
#include <iostream>
#include <random>
using namespace std;
int main() {
    mt19937 rng(random_device{}());
    int n = uniform_int_distribution<>(5, 20)(rng);
    int q = uniform_int_distribution<>(10, 30)(rng);
    cout << n << " " << q << "\n";
    for (int i = 0; i < q; i++) {
        int op = uniform_int_distribution<>(0, 1)(rng);
        int u = uniform_int_distribution<>(0, n-1)(rng);
        int v = uniform_int_distribution<>(0, n-1)(rng);
        cout << op << " " << u << " " << v << "\n";
    }
}
EOF

cat > "$STRESS_DIR/dsu_my.cpp" << 'EOF'
#include "dsu.hpp"
#include <iostream>
using namespace std;
int main() {
    int n, q;
    cin >> n >> q;
    DisjointSetUnion dsu(n);
    while (q--) {
        int op, u, v;
        cin >> op >> u >> v;
        if (op == 0) dsu.merge(u, v);
        else cout << dsu.same(u, v) << "\n";
    }
}
EOF

cat > "$STRESS_DIR/dsu_std.cpp" << 'EOF'
#include <iostream>
#include <vector>
#include <numeric>
using namespace std;
int main() {
    int n, q;
    cin >> n >> q;
    vector<int> par(n);
    iota(par.begin(), par.end(), 0);
    auto find = [&](auto&& self, int x) -> int {
        return par[x] == x ? x : par[x] = self(self, par[x]);
    };
    while (q--) {
        int op, u, v;
        cin >> op >> u >> v;
        if (op == 0) {
            u = find(find, u);
            v = find(find, v);
            if (u != v) par[u] = v;
        } else {
            cout << (find(find, u) == find(find, v)) << "\n";
        }
    }
}
EOF

stress_test "dsu" 100

# Dijkstra 对拍测试
echo ""
echo "=== 2. Dijkstra (最短路) ==="
cat > "$STRESS_DIR/dijkstra_gen.cpp" << 'EOF'
#include <iostream>
#include <random>
using namespace std;
int main() {
    mt19937 rng(random_device{}());
    int n = uniform_int_distribution<>(5, 15)(rng);
    int m = uniform_int_distribution<>(n, n*2)(rng);
    cout << n << " " << m << "\n";
    for (int i = 0; i < m; i++) {
        int u = uniform_int_distribution<>(0, n-1)(rng);
        int v = uniform_int_distribution<>(0, n-1)(rng);
        int w = uniform_int_distribution<>(1, 100)(rng);
        cout << u << " " << v << " " << w << "\n";
    }
}
EOF

cat > "$STRESS_DIR/dijkstra_my.cpp" << 'EOF'
#include "dijkstra.hpp"
#include <iostream>
#include <vector>
#include <limits>
using namespace std;
int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, long long>>> adj(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
    }

    const long long INF = numeric_limits<long long>::max();
    auto dist = dijkstra<long long>(adj, {0});

    for (int i = 0; i < n; i++) {
        if (dist[i] == INF) cout << "-1";
        else cout << dist[i];
        if (i < n-1) cout << " ";
    }
    cout << "\n";
}
EOF

cat > "$STRESS_DIR/dijkstra_std.cpp" << 'EOF'
#include <iostream>
#include <vector>
#include <limits>
using namespace std;
int main() {
    int n, m;
    cin >> n >> m;
    const long long INF = numeric_limits<long long>::max() / 4;
    vector<vector<long long>> dist(n, vector<long long>(n, INF));
    for (int i = 0; i < n; ++i) dist[i][i] = 0;
    for (int i = 0; i < m; ++i) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        dist[u][v] = min(dist[u][v], w);
    }
    for (int k = 0; k < n; ++k)
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                if (dist[i][k] < INF && dist[k][j] < INF)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
    for (int i = 0; i < n; ++i) {
        if (dist[0][i] >= INF) cout << "-1";
        else cout << dist[0][i];
        if (i + 1 < n) cout << ' ';
    }
    cout << '\n';
}
EOF

stress_test "dijkstra" 100

# Fenwick 对拍测试
echo ""
echo "=== 3. Fenwick Tree (树状数组) ==="
cat > "$STRESS_DIR/fenwick_gen.cpp" << 'EOF'
#include <iostream>
#include <random>
using namespace std;
int main() {
    mt19937 rng(random_device{}());
    int n = uniform_int_distribution<>(10, 50)(rng);
    int q = uniform_int_distribution<>(20, 50)(rng);
    cout << n << " " << q << "\n";
    for (int i = 0; i < n; i++) {
        cout << uniform_int_distribution<>(-100, 100)(rng);
        if (i < n-1) cout << " ";
    }
    cout << "\n";
    for (int i = 0; i < q; i++) {
        int op = uniform_int_distribution<>(0, 1)(rng);
        if (op == 0) {
            int pos = uniform_int_distribution<>(0, n-1)(rng);
            int val = uniform_int_distribution<>(-50, 50)(rng);
            cout << op << " " << pos << " " << val << "\n";
        } else {
            int l = uniform_int_distribution<>(0, n-1)(rng);
            int r = uniform_int_distribution<>(l, n-1)(rng);
            cout << op << " " << l << " " << r << "\n";
        }
    }
}
EOF

cat > "$STRESS_DIR/fenwick_my.cpp" << 'EOF'
#include "fenwick.hpp"
#include <iostream>
#include <vector>
using namespace std;
int main() {
    int n, q;
    cin >> n >> q;
    vector<long long> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    Fenwick<long long> fen(a);
    while (q--) {
        int op;
        cin >> op;
        if (op == 0) {
            int pos, val;
            cin >> pos >> val;
            fen.add(pos, val);
        } else {
            int l, r;
            cin >> l >> r;
            cout << fen.get(l, r+1) << "\n";
        }
    }
}
EOF

cat > "$STRESS_DIR/fenwick_std.cpp" << 'EOF'
#include <iostream>
#include <vector>
using namespace std;
int main() {
    int n, q;
    cin >> n >> q;
    vector<long long> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    while (q--) {
        int op;
        cin >> op;
        if (op == 0) {
            int pos, val;
            cin >> pos >> val;
            a[pos] += val;
        } else {
            int l, r;
            cin >> l >> r;
            long long sum = 0;
            for (int i = l; i <= r; i++) sum += a[i];
            cout << sum << "\n";
        }
    }
}
EOF

stress_test "fenwick" 100

echo ""
echo "================================"
echo "对拍测试完成"
echo "================================"
