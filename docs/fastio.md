# FastIO

`OptimizedIO<Len>` 是基于 `FILE*` 缓冲的输入输出类，不提供 `fastio` 命名空间或全局 `read/write` 函数。

## 接口

```cpp
template <int Len>
class OptimizedIO;

OptimizedIO(FILE* file_in = stdin, FILE* file_out = stdout);
OptimizedIO& operator>>(T& value);
OptimizedIO& operator<<(T value);
void flush_in();
void flush_out();
```

支持整数、字符和 C 字符串。析构时会自动刷新输出缓冲，也可以显式调用 `flush_out()`。

## 使用示例

```cpp
#include "fastio.hpp"

int main() {
    OptimizedIO<1 << 16> io;
    int n;
    io >> n;
    while (n--) {
        long long x;
        io >> x;
        io << x * 2 << '\n';
    }
    io.flush_out();
}
```

不要与 `std::cin` / `std::cout` 混用；输入类型必须是模板支持的整数、字符或字符串类型。
