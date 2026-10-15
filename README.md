# CP Template Library

竞赛编程算法模板库，包含常用的数据结构和算法实现。

## 概述

本库提供了 116 个常用算法和数据结构的 C++ 实现，并配有 117 篇 Markdown 文档，适用于算法竞赛（如 Codeforces、AtCoder、ICPC 等）。所有代码均为头文件形式，可以直接复制到提交代码中。

## 特性

- **即插即用**：所有模块均为单文件头文件，无需编译
- **高效实现**：经过优化的算法实现，注重常数优化
- **类型安全**：充分利用 C++ 模板特性
- **清晰接口**：简洁一致的 API 设计

## 快速开始

### 打印赛场模板

```bash
python3 -m pip install pygments reportlab
make pdf
```

常用命令：

```bash
make test          # 运行测试
make stress        # 运行对拍
make pdf           # 生成包含全部文档与代码的黑白打印版 (codebook.pdf)
make pdf-colored   # 生成包含全部文档与代码的彩色版 (codebook.pdf)
make pdf-code      # 仅生成纯代码版 (codebook-code.pdf)
make pdf-docs      # 仅生成纯文档版 (codebook-docs.pdf)
make test-code2pdf # 测试 PDF 生成器
make clean         # 删除生成的 PDF
```

默认 `make pdf` 会按 `codebook.conf` 的分类顺序，把每个模块的 `docs/<name>.md` 文档与 `include/<name>.hpp` 源码穿插合订生成带封面的黑白 A4 纸质版 `codebook.pdf`：等宽代码、中文字体支持、双栏目录、罗马目录页码和左右交替的双面页码。按 **100% / 实际大小** 打印，避免打印机再次缩小字号；双面打印选择长边翻转。详细选项见 [打印指南](docs/PRINT_GUIDE.md)。

测试按算法类别放在 `tests/`，详见 [测试说明](tests/README.md)。

### 基本用法

直接包含需要的头文件：

```cpp
#include "dsu.hpp"
#include "segtree.hpp"

int main() {
    DisjointSetUnion dsu(n);
    dsu.merge(u, v);
    // ...
}
```

### 类型定义

本库使用以下类型别名（定义在 `types.hpp`）：

```cpp
using i64 = long long;
using u64 = unsigned long long;
using i128 = __int128_t;
using u128 = __uint128_t;
using Real = double;
using Complex = std::complex<Real>;
```

### 基础工具

- **[Bit Utilities](docs/bit.md)** - 常用整数位运算
- **[CustomHash](docs/hash.md)** - 防止 `unordered_map` 被构造数据卡哈希

## 模块分类

### 数据结构

#### 基础数据结构
- **[DSU](docs/dsu.md)** - 并查集
- **[WeightedDSU](docs/weighted_dsu.md)** - 带势能并查集
- **[Fenwick](docs/fenwick.md)** - 树状数组（Binary Indexed Tree）
- **[Fenwick2D](docs/fenwick2d.md)** - 二维树状数组
- **[SegmentTree](docs/segtree.md)** - 线段树
- **[LazySegmentTree](docs/lazy_segtree.md)** - 懒标记线段树
- **[SegmentTreeBeats](docs/segment_tree_beats.md)** - 区间取上下界、加法、求和与最值
- **[SparseTable](docs/sparse_table.md)** - 静态区间最值表（同文件包含 DisjointSparseTable，支持一般结合运算）
- **[DynamicSegmentTree](docs/dynamic_segtree.md)** - 动态开点线段树
- **[PersistentSegmentTree](docs/persistent_segtree.md)** - 可持久化线段树

#### 高级数据结构
- **[Treap](docs/treap.md)** - 树堆
- **[ImplicitTreap](docs/implicit_treap.md)** - 序列维护、翻转与区间移动
- **[ScapegoatTree](docs/scapegoat.md)** - 替罪羊树
- **[LeftistHeap](docs/leftist_heap.md)** - 左偏树
- **[WaveletTree](docs/wavelet_tree.md)** - 小波树
- **[ODT](docs/old_driver_tree.md)** - 珂朵莉树（颜色段均摊）
- **[cartesian_tree](docs/cartesian_tree.md)** - 笛卡尔树
- **[LinearBasis](docs/linear_basis.md)** - 线性基
- **[BinaryTrie](docs/binary_trie.md)** - 01 字典树、异或最值
- **[MonotoneQueue](docs/monotone_queue.md)** - 单调队列
- **[coordinate_compress](docs/coordinate_compression.md)** - 坐标压缩
- **[LinkCutTree](docs/link_cut_tree.md)** - Link-Cut Tree
- **[RollbackDSU](docs/rollback_dsu.md)** - 可回滚并查集

### 图论

#### 最短路与生成树
- **[Dijkstra](docs/dijkstra.md)** - 最短路算法
- **[ZeroOneBFS](docs/zero_one_bfs.md)** - 0-1 BFS
- **[Kruskal](docs/kruskal.md)** - 最小生成树
- **[TopologicalSort](docs/toposort.md)** - 拓扑排序
- **[FunctionalGraph](docs/functional_graph.md)** - 函数图分解与跳转
- **[difference_constraints](docs/difference_constraints.md)** - 差分约束
- **[TreeDiameter](docs/tree_diameter.md)** - 树的直径

#### 网络流
- **[MaxFlow](docs/max_flow.md)** - 最大流（ACL Dinic）
- **[MinCostMaxFlow](docs/min_cost_max_flow.md)** - 最小费用最大流（ACL）

#### 匹配
- **[bipartite_matching](docs/bipartite_matching.md)** - 二分图最大匹配
- **[GeneralMatching](docs/blossom.md)** - Blossom 一般图最大匹配
- **[KM](docs/km.md)** - KM 算法（二分图最大权匹配）

#### 连通性
- **[SCC](docs/scc.md)** - 强连通分量（Tarjan）
- **[EDCC](docs/edcc.md)** - 边双连通分量
- **[VDCC](docs/vdcc.md)** - 点双连通分量
- **[TwoSat](docs/two_sat.md)** - 2-SAT

#### 树算法
- **[HLD](docs/hld.md)** - 树链剖分
- **[BinaryLiftingLCA](docs/binary_lifting_lca.md)** - 倍增 LCA
- **[TarjanLCA](docs/tarjan_lca.md)** - Tarjan LCA
- **[DSUOnTree](docs/dsu_on_tree.md)** - 树上启发式合并
- **[TreeHash](docs/tree_hash.md)** - 树哈希
- **[CentroidDecomposition](docs/centroid_decomposition.md)** - 重心分解
- **[VirtualTree](docs/virtual_tree.md)** - 虚树

#### 其他
- **[euler_path](docs/euler_path.md)** - 欧拉路径
- **[Prufer](docs/prufer.md)** - Prufer 序列

### 字符串

- **[KMP](docs/kmp.md)** - KMP 字符串匹配
- **[ZAlgorithm](docs/z_algo.md)** - Z 算法
- **[ExtendedKMP](docs/exkmp.md)** - 扩展 KMP
- **[Lyndon](docs/lyndon.md)** - Lyndon 分解
- **[Manacher](docs/manacher.md)** - 马拉车算法（最长回文子串）
- **[RollingHash](docs/rolling_hash.md)** - 字符串哈希
- **[Trie](docs/trie.md)** - 字典树
- **[AhoCorasick](docs/ac_automaton.md)** - AC 自动机
- **[SuffixArray](docs/suffix_array.md)** - 后缀数组
- **[SuffixAutomaton](docs/suffix_automaton.md)** - 后缀自动机
- **[PalindromicTree](docs/palindromic_tree.md)** - 回文树
- **[MinimalRotation](docs/minimal_rotation.md)** - 最小表示法

### 数学

#### 数论
- **[ModInt](docs/modint.md)** - 模运算整数类
- **[Sieve](docs/sieve.md)** - 线性筛及常见积性函数筛法
- **[Min25Sieve](docs/min25.md)** - Min_25 筛
- **[ExGCD](docs/exgcd.md)** - 扩展欧几里得
- **[CRT](docs/crt.md)** - 中国剩余定理
- **[BSGS](docs/bsgs.md)** - 大步小步算法（现有实现同时支持非互质模数）
- **[MillerRabin](docs/miller_rabin.md)** - Miller-Rabin 素性测试
- **[PollardRho](docs/pollard_rho.md)** - Pollard-Rho 因数分解
- **[Euler](docs/euler.md)** - 欧拉函数
- **[Lucas](docs/lucas.md)** - Lucas 定理
- **[ModularRoots](docs/modular_roots.md)** - 模意义原根与离散对数
- **[ModularArithmetic](docs/modular_arithmetic.md)** - 防溢出模乘与快速幂

#### 多项式与矩阵
- **[Polynomial](docs/polynomial.md)** - 多项式运算（NTT/FFT）
- **[FWT](docs/fwt.md)** - XOR / OR / AND 卷积
- **[FormalPowerSeries](docs/formal_power_series.md)** - 形式幂级数（逆、对数、指数、幂、平方根）
- **[Matrix](docs/matrix.md)** - 矩阵类
- **[Gaussian](docs/gaussian.md)** - 高斯消元
- **[Determinant](docs/determinant.md)** - 行列式

#### 组合数学
- **[Comb](docs/comb.md)** - 组合数
- **[Stirling](docs/stirling.md)** - 斯特林数
- **[Lagrange](docs/lagrange.md)** - 拉格朗日插值
- **[LinearRecurrence](docs/linear_recurrence.md)** - 线性递推
- **[MatrixTree](docs/matrix_tree.md)** - 矩阵树定理
- **[LGV](docs/lgv.md)** - LGV 引理
- **[FloorSum](docs/floor_sum.md)** - 类欧几里得算法

### 几何

- **[Geometry](docs/geometry.md)** - 计算几何综合
  - **基础** - 点、向量运算
  - **凸包** - Andrew 算法
  - **旋转卡壳** - 最远点对等
  - **圆** - 圆的交、并等
  - **多边形** - 面积、包含关系等
  - **半平面交** - 半平面交算法
  - **闵可夫斯基和** - 凸包的闵可夫斯基和
- **[MinimumEnclosingCircle](docs/minimum_enclosing_circle.md)** - 最小圆覆盖
- **[Geometry3D](docs/geometry3d.md)** - 三维点、线、面、球

### 其他算法

#### 分治与分块
- **[CDQ](docs/cdq.md)** - CDQ 分治
- **[Mo](docs/mo.md)** - 莫队算法
- **[MergeSort](docs/merge_sort.md)** - 归并排序（稳定排序）

#### 搜索与优化
- **[BinarySearch](docs/binary_search.md)** - 二分搜索
- **[TernarySearch](docs/ternary_search.md)** - 三分搜索
- **[ParallelBinarySearch](docs/parallel_binary_search.md)** - 整体二分
- **[OverallBinarySearch](docs/overall_binary_search.md)** - 整体二分（事件版）
- **[SimulatedAnnealing](docs/simulated_annealing.md)** - 模拟退火
- **[simplex](docs/simplex.md)** - 线性规划

#### 动态规划优化
- **[LiChao](docs/lichao.md)** - 李超线段树（维护线段/直线）
- **[ConvexHullTrick](docs/convex_hull_trick.md)** - 凸包优化（斜率优化）
- **[DivideConquerDP](docs/divide_conquer_dp.md)** - 决策单调的分治 DP 优化
- **[WQSBinarySearch](docs/wqs_binary_search.md)** - WQS 二分（Aliens trick）
- **[MonotoneQueue](docs/monotone_queue.md)** - 单调队列优化

#### 其他
- **[FastIO](docs/fastio.md)** - 快速输入输出
- **[DynamicBitset](docs/dynamic_bitset.md)** - 动态 bitset
- **[BigInt](docs/bigint.md)** - 大整数

## 性能说明

所有算法的时间复杂度在各自的文档中标注。一般情况下：

- 数据结构操作为 $O(\log n)$ 或 $O(1)$
- 图算法根据具体算法而定
- 字符串算法多为 $O(n)$ 或 $O(n \log n)$
- 数学算法根据具体问题而定

## 使用建议

1. **了解复杂度**：使用前确认算法复杂度满足题目要求
2. **注意模数**：使用 ModInt 时需要正确设置模数
3. **类型匹配**：注意整数类型的选择，避免溢出
4. **边界情况**：注意处理 $n=0$ 等边界情况

## 依赖关系

部分模块有依赖关系：
- `modint.hpp` 依赖 `types.hpp`
- `polynomial.hpp` 依赖 `modint.hpp`
- `geometry.hpp` 聚合了多个几何模块
- 等等

使用时请确保包含所有必需的头文件。

## 参考资料

- [AtCoder Library (ACL)](https://github.com/atcoder/ac-library)
- [OI Wiki](https://oi-wiki.org/)
- 各算法竞赛题目及题解

## 许可

本库仅供学习和竞赛使用。

---

**详细文档请查看 [docs](./docs/) 目录。**
