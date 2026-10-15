# code2pdf

把 `include/` 中的 C++ 头文件（内含详细中文注释）整理成带中文标题和中文双栏目录、适合打印装订与赛场翻阅的竖向 A4 PDF：

```sh
python3 -m pip install pygments reportlab
make pdf          # 生成黑白打印版 (codebook.pdf)
make pdf-colored  # 生成彩色预览版 (codebook.pdf)
```

常用选项：

```sh
./tools/code2pdf include/ -c codebook.conf -o codebook.pdf --title "算法竞赛模板库"
./tools/code2pdf include/ -c codebook.conf --compact --columns 2
./tools/code2pdf include/ -c codebook.conf --orientation landscape
./tools/code2pdf include/ -c codebook.conf --color --style friendly
./tools/code2pdf include/ -c codebook.conf --title "Team ABC 模板手册" --author "Team ABC"
```

`codebook.conf` 支持使用 `[中文分类名]` 定义目录章节，并支持 `filename.hpp | 中文模块标题` 语法，使生成的 PDF 在双栏目录与每页页眉中均展示中文标题。
