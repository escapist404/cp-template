# FastIO (`fastio.hpp`)

`fastio.hpp` 提供基于固定大小 `fread` / `fwrite` 缓冲区的快速输入输出类 `OptimizedIO<Len>`，适用于输入输出规模较大（如 $10^6$ 以上）的算法竞赛题目。

## 接口说明

### 类模板与构造

```cpp
template <int Len>
class OptimizedIO {
public:
    OptimizedIO(FILE *file_in = stdin, FILE *file_out = stdout);
    ~OptimizedIO();
    void flush_in();
    void flush_out();
};
```

- `Len`：输入缓冲与输出缓冲各自的字节大小（通常设为 `1 << 20`，即 1 MB）。由于缓冲区 `buf_in[Len]` 和 `buf_out[Len]` 是类的直接成员数组，若 `Len` 较大且在函数栈上实例化，需留意栈空间限制（也可定义为全局或 `static` 对象）。
- 构造函数会立即调用一次 `flush_in()` 预读输入流；析构函数会自动调用 `flush_out()` 将剩余输出缓冲写入目标文件流。
- 使用 `OptimizedIO` 读取输入时，底层直接成块消费 `stdin`，**同一程序内切勿再混用 `std::cin`、`scanf` 或 `getchar` 读取输入**。输出侧如果不与 `OptimizedIO` 的 `<<` 混用，仍可单独使用 `std::cout`。

### 输入操作符 `operator>>`

```cpp
OptimizedIO& operator>>(char& ch);
OptimizedIO& operator>>(std::string& value);

template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
OptimizedIO& operator>>(T& value);

template <typename T, std::enable_if_t<std::is_floating_point<T>::value, int> = 0>
OptimizedIO& operator>>(T& value);

template <typename T, std::enable_if_t<!std::is_integral<T>::value && !std::is_floating_point<T>::value, int> = 0>
OptimizedIO& operator>>(T& value);
```

- `operator>>(char& ch)`：直接读取缓冲区的**下一个原始字节**（**不会跳过空格或换行符**）；若已到达文件末尾（EOF），将 `ch` 置为 `'\0'`。
- `operator>>(std::string& value)`：自动跳过前导空白字符（`isspace`），读取连续的非空白字符直到遇到下一个空白字符或 EOF。
- 整型重载（`std::is_integral<T>`）：自动跳过非数字字符并识别紧邻数字前的负号 `'-'`，支持所有有符号/无符号整型及 `__int128_t` / `__uint128_t`。
- 浮点型重载（`std::is_floating_point<T>`）：先按 `std::string` 读取一个空白分隔的词元，再通过 `std::strtod` 转换为目标浮点类型 `T`。
- 自定义类型兜底重载：先读取一个 `long long` 整数 `number`，再通过显式转换 `value = T(number)` 赋值，因此可直接支持 `io >> mint_val`（如 [`ModInt`](modint.md)）。

### 输出操作符 `operator<<`

```cpp
OptimizedIO& operator<<(char ch);
OptimizedIO& operator<<(const char *str);
OptimizedIO& operator<<(char *str);
template <typename T> OptimizedIO& operator<<(T value);
```

- 支持单个字符 `char`、以 `'\0'` 结尾的 C 风格字符串，以及任意有符号/无符号整数类型（包括 `i128`）。
- 注意输出重载未单独特化 `std::string` 或浮点数，输出 `std::string` 时请传入 `s.c_str()`。

## 复杂度

- 单次字符读入/输出：均摊 $O(1)$。
- 整数/字符串读入与输出：$O(\text{字符长度})$。
- 空间复杂度：对象自身占用 $O(\text{Len})$ 字节缓冲区。

## 使用示例

```cpp
#include "fastio.hpp"
#include "types.hpp"
#include <string>

int main() {
    static OptimizedIO<1 << 20> io;
    int n = 3;
    i64 sum = 0;
    for (int i = 1; i <= n; ++i) {
        sum += i * 10;
    }
    std::string msg = "sum = ";
    io << msg.c_str() << sum << '\n';
    return 0;
}
```

## 相关文档

- [Types](types.md) —— 基础整数与 `i128` 类型定义
- [ModInt](modint.md) —— 可直接通过 `OptimizedIO` 读入的模整数类
