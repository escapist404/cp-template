#ifndef FASTIO_HPP
#define FASTIO_HPP

#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <string>
#include <type_traits>

// Buffered fread/fwrite fast I/O. Do not mix input with std::cin or scanf.

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
    OptimizedIO(FILE *file_in = stdin, FILE *file_out = stdout)
        : pos_in(0), size_in(0), pos_out(0), eof(false), file_in(file_in), file_out(file_out) {
        flush_in();
    }
    ~OptimizedIO() {
        flush_out();
    }
};

#endif
