#!/bin/bash
# 递归运行 C++ 测试，也可指定一个分类目录或单个文件。
cd "$(dirname "$0")" || exit 1
TEST_PATH=${1:-tests}
BUILD_DIR=$(mktemp -d) || exit 1
trap 'rm -rf "$BUILD_DIR"' EXIT
PASSED=0
FAILED=0
TOTAL=0
while IFS= read -r test_file; do
    TOTAL=$((TOTAL + 1))
    printf '[%d] %s ... ' "$TOTAL" "$test_file"
    if ! "${CXX:-g++}" -std=c++17 -O2 -Wall -Wextra -fsanitize=address,undefined "$test_file" -o "$BUILD_DIR/test" >"$BUILD_DIR/log" 2>&1; then
        echo "✗ 编译失败"; cat "$BUILD_DIR/log"; FAILED=$((FAILED + 1))
    elif ! "$BUILD_DIR/test" >"$BUILD_DIR/log" 2>&1; then
        echo "✗ 运行失败"; cat "$BUILD_DIR/log"; FAILED=$((FAILED + 1))
    else
        echo "✓ 通过"; PASSED=$((PASSED + 1))
    fi
done < <(find "$TEST_PATH" -type f -name 'test_*.cpp' | sort)
echo "total=$TOTAL passed=$PASSED failed=$FAILED"
[ "$TOTAL" -gt 0 ] && [ "$FAILED" -eq 0 ]
