# CP Template Library

竞赛编程算法模板库，包含常用的数据结构和算法实现。

## 概述

本库提供了 116 个常用算法和数据结构的 C++ 实现，所有头文件均内嵌详尽的中文接口、参数约定与复杂度注释，适用于算法竞赛（如 Codeforces、AtCoder、ICPC 等）。所有代码均为头文件形式，可以直接复制到提交代码中。

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
make pdf           # 生成带中文目录与详细中文注释的黑白打印版 (codebook.pdf)
make pdf-colored   # 生成带中文目录与详细中文注释的彩色版 (codebook.pdf)
make test-code2pdf # 测试 PDF 生成器
make clean         # 删除生成的 PDF
```

默认 `make pdf` 会按 `codebook.conf` 的分类顺序，把 `include/` 下所有带详尽中文注释的头文件生成带中文封面与中文双栏目录的黑白 A4 纸质版 `codebook.pdf`：等宽代码、中文字体支持、双栏目录、罗马目录页码和左右交替的双面页码。按 **100% / 实际大小** 打印，避免打印机再次缩小字号；双面打印选择长边翻转。详细选项见 [打印指南](include/PRINT_GUIDE.hpp)。

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

- **[Bit Utilities](include/bit.hpp)** - 常用整数位运算
- **[CustomHash](include/hash.hpp)** - 防止 `unordered_map` 被构造数据卡哈希

## 模块分类

### 数据结构

#### 基础数据结构
- **[DSU](include/dsu.hpp)** - 并查集
- **[WeightedDSU](include/weighted_dsu.hpp)** - 带势能并查集
- **[Fenwick](include/fenwick.hpp)** - 树状数组（Binary Indexed Tree）
- **[Fenwick2D](include/fenwick2d.hpp)** - 二维树状数组
- **[SegmentTree](include/segtree.hpp)** - 线段树
- **[LazySegmentTree](include/lazy_segtree.hpp)** - 懒标记线段树
- **[SegmentTreeBeats](include/segment_tree_beats.hpp)** - 区间取上下界、加法、求和与最值
- **[SparseTable](include/sparse_table.hpp)** - 静态区间最值表（同文件包含 DisjointSparseTable，支持一般结合运算）
- **[DynamicSegmentTree](include/dynamic_segtree.hpp)** - 动态开点线段树
- **[PersistentSegmentTree](include/persistent_segtree.hpp)** - 可持久化线段树

#### 高级数据结构
- **[Treap](include/treap.hpp)** - 树堆
- **[ImplicitTreap](include/implicit_treap.hpp)** - 序列维护、翻转与区间移动
- **[ScapegoatTree](include/scapegoat.hpp)** - 替罪羊树
- **[LeftistHeap](include/leftist_heap.hpp)** - 左偏树
- **[WaveletTree](include/wavelet_tree.hpp)** - 小波树
- **[ODT](include/old_driver_tree.hpp)** - 珂朵莉树（颜色段均摊）
- **[cartesian_tree](include/cartesian_tree.hpp)** - 笛卡尔树
- **[LinearBasis](include/linear_basis.hpp)** - 线性基
- **[BinaryTrie](include/binary_trie.hpp)** - 01 字典树、异或最值
- **[MonotoneQueue](include/monotone_queue.hpp)** - 单调队列
- **[coordinate_compress](include/coordinate_compression.hpp)** - 坐标压缩
- **[LinkCutTree](include/link_cut_tree.hpp)** - Link-Cut Tree
- **[RollbackDSU](include/rollback_dsu.hpp)** - 可回滚并查集

### 图论

#### 最短路与生成树
- **[Dijkstra](include/dijkstra.hpp)** - 最短路算法
- **[ZeroOneBFS](include/zero_one_bfs.hpp)** - 0-1 BFS
- **[Kruskal](include/kruskal.hpp)** - 最小生成树
- **[TopologicalSort](include/toposort.hpp)** - 拓扑排序
- **[FunctionalGraph](include/functional_graph.hpp)** - 函数图分解与跳转
- **[difference_constraints](include/difference_constraints.hpp)** - 差分约束
- **[TreeDiameter](include/tree_diameter.hpp)** - 树的直径

#### 网络流
- **[MaxFlow](include/max_flow.hpp)** - 最大流（ACL Dinic）
- **[MinCostMaxFlow](include/min_cost_max_flow.hpp)** - 最小费用最大流（ACL）

#### 匹配
- **[bipartite_matching](include/bipartite_matching.hpp)** - 二分图最大匹配
- **[GeneralMatching](include/blossom.hpp)** - Blossom 一般图最大匹配
- **[KM](include/km.hpp)** - KM 算法（二分图最大权匹配）

#### 连通性
- **[SCC](include/scc.hpp)** - 强连通分量（Tarjan）
- **[EDCC](include/edcc.hpp)** - 边双连通分量
- **[VDCC](include/vdcc.hpp)** - 点双连通分量
- **[TwoSat](include/two_sat.hpp)** - 2-SAT

#### 树算法
- **[HLD](include/hld.hpp)** - 树链剖分
- **[BinaryLiftingLCA](include/binary_lifting_lca.hpp)** - 倍增 LCA
- **[TarjanLCA](include/tarjan_lca.hpp)** - Tarjan LCA
- **[DSUOnTree](include/dsu_on_tree.hpp)** - 树上启发式合并
- **[TreeHash](include/tree_hash.hpp)** - 树哈希
- **[CentroidDecomposition](include/centroid_decomposition.hpp)** - 重心分解
- **[VirtualTree](include/virtual_tree.hpp)** - 虚树

#### 其他
- **[euler_path](include/euler_path.hpp)** - 欧拉路径
- **[Prufer](include/prufer.hpp)** - Prufer 序列

### 字符串

- **[KMP](include/kmp.hpp)** - KMP 字符串匹配
- **[ZAlgorithm](include/z_algo.hpp)** - Z 算法
- **[ExtendedKMP](include/exkmp.hpp)** - 扩展 KMP
- **[Lyndon](include/lyndon.hpp)** - Lyndon 分解
- **[Manacher](include/manacher.hpp)** - 马拉车算法（最长回文子串）
- **[RollingHash](include/rolling_hash.hpp)** - 字符串哈希
- **[Trie](include/trie.hpp)** - 字典树
- **[AhoCorasick](include/ac_automaton.hpp)** - AC 自动机
- **[SuffixArray](include/suffix_array.hpp)** - 后缀数组
- **[SuffixAutomaton](include/suffix_automaton.hpp)** - 后缀自动机
- **[PalindromicTree](include/palindromic_tree.hpp)** - 回文树
- **[MinimalRotation](include/minimal_rotation.hpp)** - 最小表示法

### 数学

#### 数论
- **[ModInt](include/modint.hpp)** - 模运算整数类
- **[Sieve](include/sieve.hpp)** - 线性筛及常见积性函数筛法
- **[Min25Sieve](include/min25.hpp)** - Min_25 筛
- **[ExGCD](include/exgcd.hpp)** - 扩展欧几里得
- **[CRT](include/crt.hpp)** - 中国剩余定理
- **[BSGS](include/bsgs.hpp)** - 大步小步算法（现有实现同时支持非互质模数）
- **[MillerRabin](include/miller_rabin.hpp)** - Miller-Rabin 素性测试
- **[PollardRho](include/pollard_rho.hpp)** - Pollard-Rho 因数分解
- **[Euler](include/euler.hpp)** - 欧拉函数
- **[Lucas](include/lucas.hpp)** - Lucas 定理
- **[ModularRoots](include/modular_roots.hpp)** - 模意义原根与离散对数
- **[ModularArithmetic](include/modular_arithmetic.hpp)** - 防溢出模乘与快速幂

#### 多项式与矩阵
- **[Polynomial](include/polynomial.hpp)** - 多项式运算（NTT/FFT）
- **[FWT](include/fwt.hpp)** - XOR / OR / AND 卷积
- **[FormalPowerSeries](include/formal_power_series.hpp)** - 形式幂级数（逆、对数、指数、幂、平方根）
- **[Matrix](include/matrix.hpp)** - 矩阵类
- **[Gaussian](include/gaussian.hpp)** - 高斯消元
- **[Determinant](include/determinant.hpp)** - 行列式

#### 组合数学
- **[Comb](include/comb.hpp)** - 组合数
- **[Stirling](include/stirling.hpp)** - 斯特林数
- **[Lagrange](include/lagrange.hpp)** - 拉格朗日插值
- **[LinearRecurrence](include/linear_recurrence.hpp)** - 线性递推
- **[MatrixTree](include/matrix_tree.hpp)** - 矩阵树定理
- **[LGV](include/lgv.hpp)** - LGV 引理
- **[FloorSum](include/floor_sum.hpp)** - 类欧几里得算法

### 几何

- **[Geometry](include/geometry.hpp)** - 计算几何综合
  - **基础** - 点、向量运算
  - **凸包** - Andrew 算法
  - **旋转卡壳** - 最远点对等
  - **圆** - 圆的交、并等
  - **多边形** - 面积、包含关系等
  - **半平面交** - 半平面交算法
  - **闵可夫斯基和** - 凸包的闵可夫斯基和
- **[MinimumEnclosingCircle](include/minimum_enclosing_circle.hpp)** - 最小圆覆盖
- **[Geometry3D](include/geometry3d.hpp)** - 三维点、线、面、球

### 其他算法

#### 分治与分块
- **[CDQ](include/cdq.hpp)** - CDQ 分治
- **[Mo](include/mo.hpp)** - 莫队算法
- **[MergeSort](include/merge_sort.hpp)** - 归并排序（稳定排序）

#### 搜索与优化
- **[BinarySearch](include/binary_search.hpp)** - 二分搜索
- **[TernarySearch](include/ternary_search.hpp)** - 三分搜索
- **[ParallelBinarySearch](include/parallel_binary_search.hpp)** - 整体二分
- **[OverallBinarySearch](include/overall_binary_search.hpp)** - 整体二分（事件版）
- **[SimulatedAnnealing](include/simulated_annealing.hpp)** - 模拟退火
- **[simplex](include/simplex.hpp)** - 线性规划

#### 动态规划优化
- **[LiChao](include/lichao.hpp)** - 李超线段树（维护线段/直线）
- **[ConvexHullTrick](include/convex_hull_trick.hpp)** - 凸包优化（斜率优化）
- **[DivideConquerDP](include/divide_conquer_dp.hpp)** - 决策单调的分治 DP 优化
- **[WQSBinarySearch](include/wqs_binary_search.hpp)** - WQS 二分（Aliens trick）
- **[MonotoneQueue](include/monotone_queue.hpp)** - 单调队列优化

#### 其他
- **[FastIO](include/fastio.hpp)** - 快速输入输出
- **[DynamicBitset](include/dynamic_bitset.hpp)** - 动态 bitset
- **[BigInt](include/bigint.hpp)** - 大整数

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

**所有接口说明与使用约定均直接内嵌在 [`include/`](./include/) 各头文件顶部注释中。**
