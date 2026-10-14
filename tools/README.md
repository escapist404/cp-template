# code2pdf

把源码与配套 Markdown 文档整理成适合打印、装订和赛场翻阅的竖向 A4 PDF：

```sh
python3 -m pip install pygments reportlab
make pdf          # 生成包含全部文档与代码的黑白打印版 (codebook.pdf)
make pdf-colored  # 生成包含全部文档与代码的彩色版 (codebook.pdf)
make pdf-code     # 仅生成纯代码版 (codebook-code.pdf)
make pdf-docs     # 仅生成纯文档版 (codebook-docs.pdf)
```

常用选项：

```sh
./tools/code2pdf include/ -c codebook.conf --with-docs docs/ -o codebook.pdf
./tools/code2pdf include/ -c codebook.conf --docs-only docs/ --no-line-numbers -o codebook-docs.pdf
./tools/code2pdf include/ -c codebook.conf -o codebook-code.pdf
./tools/code2pdf include/ -c codebook.conf --with-docs docs/ --compact --columns 2
./tools/code2pdf --orientation landscape
./tools/code2pdf --color --style friendly
./tools/code2pdf include/ -c codebook.conf --with-docs docs/ --title "Team ABC" --author "Team ABC"
```

默认收集常见 C/C++、Python、Java、Rust、Go、脚本和 SQL 文件；`--ext` 可追加扩展名。
默认生成封面、黑白输出、8.5 pt 正文、11 pt 行距、36 pt 页边距、行号以及双栏目录；目录使用小写罗马页码，正文从 1 重新编号，双面打印时页码按左右页边交替。需要彩色时显式加 `--color`，需要减少页数时加 `--compact`。

`--with-docs [DIR]` 按配置文件顺序在每个 `.hpp` 头文件前穿插对应的 `docs/<stem>.md` 文档（`.md` 自动隐藏行号，`.hpp` 保留行号）；`--docs-only [DIR]` 仅输出对应的 Markdown 文档。`--columns 2` 将正文按左栏、右栏、下一页的顺序连续排版，文件标题标明模块和续页；`--max-pages` 可选限制总页数。`--title` 和 `--author` 同时控制封面与 PDF 元数据。

代码正文的拉丁字符使用真正的等宽字体；中文注释使用 CJK 字体的自然字距，不逐字拉开。标题和目录按中英文分别选用匹配字体，避免混排时出现字距和字重突变。macOS 会优先使用系统等宽字体和中文字体，Linux 会尝试 DejaVu/Noto 字体，找不到文件时回退到 ReportLab 的 `STSong-Light`。
