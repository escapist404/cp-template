# 使用配置文件生成定制化打印版

## 配置文件说明

`codebook.conf` 文件控制要打印的模块及其章节顺序（共 16 个逻辑分类，涵盖全部 116 个算法头文件与对应文档）。

### 格式规则

- `[Section Name]` 定义目录分类标题
- 每行一个头文件路径（相对于 `include/` 目录）
- 以 `#` 开头的行是注释，空行会被忽略
- 开启 `--with-docs docs/`（`make pdf` 默认开启）时，每个模块会自动先排版 `docs/<name>.md` 接口文档（隐藏行号），紧接着排版 `include/<name>.hpp` 源码（保留行号）

## 使用方法

### 1. 生成包含文档与代码的完整合订大 PDF（默认推荐）

默认按纸张阅读生成：封面、黑白 A4、8.5 pt 等宽正文、11 pt 行距和 36 pt 页边距。目录使用小写罗马页码，正文从 1 重新编号；双面打印时页码按左右页边交替。

打印时选 **100% / 实际大小**，双面选 **长边翻转**。

```bash
# 生成包含全部文档与代码的黑白打印版 codebook.pdf
make pdf

# 生成包含全部文档与代码的彩色预览版 codebook.pdf
make pdf-colored

# 等价脚本调用
python3 tools/code2pdf.py include/ -o codebook.pdf -c codebook.conf --with-docs docs/ --black-and-white --title "CP Template Library & Docs"
```

### 2. 单独打印纯代码或纯文档

如果需要将代码与文档拆成两册装订，可使用：

```bash
# 仅打印纯代码 (codebook-code.pdf)
make pdf-code

# 仅打印纯文档速查手册 (codebook-docs.pdf)
make pdf-docs
```

### 3. 其他排版选项

```bash
# 双栏紧凑版（大幅减少总页数）
python3 tools/code2pdf.py include/ -o codebook.pdf -c codebook.conf --with-docs docs/ --compact --columns 2

# 横向打印
python3 tools/code2pdf.py include/ -o codebook.pdf -c codebook.conf --with-docs docs/ --orientation landscape

# 自定义封面标题和队伍名称
python3 tools/code2pdf.py include/ -o codebook.pdf -c codebook.conf --with-docs docs/ \
  --title "Team ABC Codebook" --author "Team ABC"
```

---

更多信息参见 `tools/code2pdf.py --help`
