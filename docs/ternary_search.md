# TernarySearch

`real_ternary_search_max` 和 `real_ternary_search_min` 对单峰实函数进行固定次数三分搜索。

函数必须在搜索区间上单调增加后单调减少，或单调减少后单调增加。默认迭代 80 次，适合 `double` 精度范围内的答案。
