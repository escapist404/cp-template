#ifndef FASTIO_HPP
#define FASTIO_HPP

#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <string>
#include <type_traits>

// ============================================================================
// 【基于 fread / fwrite 缓冲区的快速输入输出 (fastio.hpp)】
// ----------------------------------------------------------------------------
// 模板类签名：template <int Len> class OptimizedIO
//   - Len：底层输入/输出字节缓冲区大小（竞赛常用 OptimizedIO<1 << 20> io;）。
// 构造函数与析构函数签名：
//   - OptimizedIO(FILE *file_in = stdin, FILE *file_out = stdout)
//     绑定输入与输出流并在构造时立即预读第一块输入缓冲。
//   - ~OptimizedIO()
//     析构时自动调用 flush_out() 将未写出的输出缓冲全部刷入目标文件流。
// 缓冲区刷新签名：
//   - void flush_in()
//     使用 fread 从 file_in 重新填充输入缓冲区 buf_in 并重置读指针 pos_in。
//   - void flush_out()
//     使用 fwrite 将 buf_out[0 .. pos_out) 写入 file_out 并重置写指针 pos_out。
// 输入运算符签名（均返回 OptimizedIO& 以支持 io >> a >> b 链式调用）：
//   - OptimizedIO& operator>>(char& ch)
//     读取下一个原始字节（【注意】不跳过空白字符；到达 EOF 时置 ch = -1, eof = true）。
//   - OptimizedIO& operator>>(std::string& value)
//     跳过前导空白字符，读取一个连续非空白字符串到 value。
//   - template <typename T> OptimizedIO& operator>>(T& value) [有符号/无符号整型，含 i128]
//     跳过非数字字符，识别紧邻的 '-' 号并读取十进制整数（支持负数最小值无溢出读取）。
//   - template <typename T> OptimizedIO& operator>>(T& value) [浮点型 float/double/long double]
//     读取一个空白分隔的词元并通过 std::strtod 转换为浮点数。
//   - template <typename T> OptimizedIO& operator>>(T& value) [含 T::mod() 的 ModInt 类型]
//     先按 64 位有符号整数读入，再构造为对应的模整数 T(number)。
// 输出运算符签名（均返回 OptimizedIO& 以支持 io << a << '\n' 链式调用）：
//   - OptimizedIO& operator<<(char ch)
//     向输出缓冲区写入单个字符，缓冲区满时自动触发 flush_out()。
//   - OptimizedIO& operator<<(const char *str) / OptimizedIO& operator<<(char *str)
//     向输出缓冲区逐字节写入以 '\0' 结尾的 C 风格字符串。
//   - template <typename T> OptimizedIO& operator<<(T value)
//     将有符号或无符号整型（含 i128）按十进制写入输出缓冲（不带末尾换行，需手动输出 '\n'）。
// ============================================================================

template <int Len>
class OptimizedIO {
private:
    int pos_in, size_in, pos_out;
    bool eof;
    char buf_in[Len], buf_out[Len];
    FILE *file_in, *file_out;

public:
    void flush_in() {
        size_in = fread(buf_in, sizeof(char), Len, file_in), pos_in = 0;
        if (size_in == 0) eof = true;
    }
    void flush_out() {
        fwrite(buf_out, pos_out, 1, file_out), pos_out = 0;
    }
    OptimizedIO& operator>>(char& ch) {
        if (pos_in >= size_in)
            flush_in();

        ch = eof ? '\0' : buf_in[pos_in++];
        return *this;
    }
    OptimizedIO& operator>>(std::string& value) {
        value.clear();
        char ch = '\0';
        do {
            *this >> ch;
            if (!ch) return *this;
        } while (isspace((unsigned char)ch));
        while (ch && !isspace((unsigned char)ch)) {
            value.push_back(ch);
            *this >> ch;
        }
        return *this;
    }
    template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
    OptimizedIO& operator>>(T& value) {
        using U = typename std::make_unsigned<T>::type;
        U magnitude = 0;
        bool sign = true;
        char ch = '\0';
        do {
            sign = ch == '-' ? false : sign, *this >> ch;
            if (eof) return *this;
        } while (!isdigit(ch));
        do
            magnitude = (magnitude << 3) + (magnitude << 1) + ch - '0', *this >> ch;
        while (!eof && isdigit(ch));

        if (sign) {
            value = (T)magnitude;
        } else {
            value = (T)(~magnitude + 1);
        }
        return *this;
    }
    template <typename T, std::enable_if_t<std::is_floating_point<T>::value, int> = 0>
    OptimizedIO& operator>>(T& value) {
        std::string token;
        *this >> token;
        value = (T)std::strtod(token.c_str(), nullptr);
        return *this;
    }
    template <typename T,
            std::enable_if_t<!std::is_integral<T>::value &&
                    !std::is_floating_point<T>::value, int> = 0>
    OptimizedIO& operator>>(T& value) {
        long long number;
        *this >> number;
        value = T(number);
        return *this;
    }
    OptimizedIO& operator<<(char ch) {
        if (pos_out == Len)
            flush_out();
        buf_out[pos_out++] = ch;
        return *this;
    }
    OptimizedIO& operator<<(const char *str) {
        int size = strlen(str);
        for (int i = 0; i < size; ++i)
            *this << str[i];
        return *this;
    }
    OptimizedIO& operator<<(char *str) {
        int size = strlen(str);
        for (int i = 0; i < size; ++i)
            *this << str[i];
        return *this;
    }
    template <typename T> OptimizedIO& operator<<(T value) {
        using U = typename std::make_unsigned<T>::type;
        U abs_value;
        if (value < 0) {
            *this << '-';
            abs_value = (U)(-(value + 1)) + 1;
        } else {
            abs_value = (U)value;
        }
        static int stack[50], dep = 0;
        do
            stack[dep++] = abs_value % 10, abs_value /= 10;
        while (abs_value);
        do
            *this << (char)(stack[--dep] + '0');
        while (dep);
        return *this;
    }
    // - OptimizedIO(FILE *file_in = stdin, FILE *file_out = stdout) 绑定输入与输出流并在构造时立即预读第一块输入缓冲。 — 功能：OptimizedIO(FILE *file_in = stdin, FILE *file_out = stdout)
    OptimizedIO(FILE *file_in = stdin, FILE *file_out = stdout)
        : pos_in(0), size_in(0), pos_out(0), eof(false), file_in(file_in), file_out(file_out) {
        flush_in();
    }
    ~OptimizedIO() {
        flush_out();
    }
};

#endif
