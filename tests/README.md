# 测试

测试按算法对象分类，每个 C++ 文件独立编译运行。

| 目录 | 内容 |
| --- | --- |
| `core/` | 位运算、哈希、快速输入输出、ModInt |
| `data_structures/` | 基础结构、区间查询、线段树与随机对照 |
| `graph/` | 图论、树算法与随机对照 |
| `strings/` | 字符串算法 |
| `math/` | 数论、多项式、FWT、线性规划 |
| `geometry/` | 二维几何、组件接口、三维几何 |
| `techniques/` | 搜索、排序、离线算法、DP 优化 |
| `tools/` | PDF 生成工具的 Python 测试 |

```bash
make test
make test TESTS=tests/graph
./run_tests.sh tests/core/test_modint.cpp
make test-code2pdf
```

脚本递归发现 `test_*.cpp`，编译产物放在临时目录并自动删除。新增测试放入对应分类。
