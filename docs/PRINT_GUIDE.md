# 使用配置文件生成定制化打印版

## 配置文件说明

`codebook.conf` 文件控制要打印的文件及其顺序。

### 格式规则

- 每行一个文件路径（相对于 `include/` 目录）
- 以 `#` 开头的行是注释
- 空行会被忽略
- 文件按照配置中的顺序打印

### 示例

```
# 基础工具
types.hpp
fastio.hpp

# 数据结构
dsu.hpp
fenwick.hpp
segtree.hpp

# 图论
dijkstra.hpp
max_flow.hpp
```

## 使用方法

### 基本用法

默认按纸张阅读生成：封面、黑白 A4、8.5 pt 等宽正文、11 pt 行距和 36 pt 页边距。目录使用小写罗马页码，正文从 1 重新编号；双面打印时页码会按左右页边交替。

打印时选 **100% / 实际大小**，双面选 **长边翻转**。

```bash
# 使用纸质阅读默认配置：黑白、较大正文、行号、双栏目录
python3 tools/code2pdf.py include/ -o codebook.pdf --config codebook.conf

# 简写形式
python3 tools/code2pdf.py include/ -o codebook.pdf -c codebook.conf
```

### 选项组合

```bash
# 黑白打印（默认已经是黑白）
python3 tools/code2pdf.py include/ -o codebook.pdf -c codebook.conf --black-and-white

# 页数优先的紧凑版
python3 tools/code2pdf.py include/ -o codebook.pdf -c codebook.conf --compact

# 彩色屏幕预览
python3 tools/code2pdf.py include/ -o codebook.pdf -c codebook.conf --color

# 不显示行号
python3 tools/code2pdf.py include/ -o codebook.pdf -c codebook.conf --no-line-numbers

# 横向打印
python3 tools/code2pdf.py include/ -o codebook.pdf -c codebook.conf --orientation landscape

# ICPC 核心预设：双栏连续排版，生成前检查总页数不超过 25
make pdf-icpc

# 也可以直接调用预设配置
python3 tools/code2pdf.py include/ -o codebook-icpc.pdf \
  -c codebook.icpc.conf --compact --font-size 6 --line-height 7 \
  --columns 2 --max-pages 25 --no-line-numbers \
  --black-and-white --title "ICPC Core Codebook"

# 自定义封面标题和作者
python3 tools/code2pdf.py include/ -o codebook.pdf -c codebook.conf \
  --title "Team ABC Codebook" --author "Team ABC"
```

### 不使用配置文件

如果不指定 `--config`，会自动扫描所有文件并按字母顺序排列：

```bash
# 自动扫描全部文件
python3 tools/code2pdf.py include/ -o codebook.pdf
```

## 配置策略建议

### 1. 基础包（必打印）

适合日常比赛，包含最常用的 20-30 个模块：

```
types.hpp
dsu.hpp
fenwick.hpp
segtree.hpp
modint.hpp
dijkstra.hpp
kmp.hpp
```

### 2. 图论专场

针对图论题目：

```
dsu.hpp
dijkstra.hpp
hld.hpp
max_flow.hpp
min_cost_max_flow.hpp
scc.hpp
tarjan_lca.hpp
```

### 3. 字符串专场

针对字符串题目：

```
kmp.hpp
z_algo.hpp
suffix_array.hpp
ac_automaton.hpp
manacher.hpp
```

### 4. 数学专场

针对数论和数学题目：

```
modint.hpp
exgcd.hpp
crt.hpp
lucas.hpp
miller_rabin.hpp
matrix.hpp
polynomial.hpp
```

### ICPC 25 页核心预设

仓库提供 `codebook.icpc.conf`，只包含常用基础、图论、字符串、数论和搜索模板及其本地头文件依赖。`make pdf-icpc` 使用 6 pt 字号、7 pt 行距、双栏连续排版并隐藏行号。`--max-pages 25` 会统计封面、空白背页、目录和正文；超限时退出并保留已有 PDF。新增模块后请重新生成，比赛规则以当届官方要求为准。

## 实战建议

1. **赛前准备**：根据比赛风格调整配置文件
2. **轻量打印**：只包含自己熟悉的模块（减少翻页时间）
3. **分包打印**：准备多个配置文件，按需打印

## 当前配置

默认的 `codebook.conf` 包含完整的 116 个头文件；`codebook.icpc.conf` 是精简的赛场核心预设。你可以：

- 注释掉不需要的模块（在行首加 `#`）
- 添加更多模块
- 调整顺序（把最常用的放前面）

## 验证配置

生成 PDF 后，检查第一页的目录，确认：
- 所有需要的文件都包含了
- 顺序符合预期
- 页码连续

---

更多信息参见 `tools/code2pdf.py --help`
