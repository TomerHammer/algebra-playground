#!/bin/bash

set -e

EXE=./build/bin/prog              # CLI executable built by CMake
UNIT_TEST=./build/bin/matrixTests # Matrix unit test binary
TEST_DIR=tests/cliTests           # Folder containing CLI test files

echo "==============================="
echo "🔧 Building project with CMake..."
echo "==============================="

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release >/dev/null
cmake --build build --target prog matrixTests -j
echo "✅ Build completed successfully"
echo

echo "==============================="
echo "🧪 Running Matrix unit tests..."
echo "==============================="

if "$UNIT_TEST"; then
  echo "✅ All Matrix unit tests passed"
else
  echo "❌ Matrix unit tests failed"
  exit 1
fi
echo

if [ ! -d "$TEST_DIR" ]; then
  echo "❌ Error: Test directory '$TEST_DIR' not found."
  exit 1
fi

# Clean previous outputs
rm -f "$TEST_DIR"/test*.out.txt

# Collect all input files (sorted numerically)
inputs=$(ls "$TEST_DIR"/*.in.txt 2>/dev/null | sort -V)

if [ -z "$inputs" ]; then
  echo "❌ No CLI test input files (*.in.txt) found in $TEST_DIR"
  exit 1
fi

passed=0
total=0

echo "==============================="
echo "🧮 Running CLI integration tests"
echo "==============================="

for file in $inputs; do
  base=$(basename "$file" .in.txt)
  total=$((total + 1))
  echo "▶ Running $base..."

  "$EXE" < "$file" > "$TEST_DIR/${base}.out.txt"

  if diff -q "$TEST_DIR/${base}.out.txt" "$TEST_DIR/${base}.expected.txt" >/dev/null; then
    echo "✅ $base passed"
    passed=$((passed + 1))
  else
    echo "❌ $base failed"
    echo "   Differences:"
    diff "$TEST_DIR/${base}.out.txt" "$TEST_DIR/${base}.expected.txt" || true
  fi
  echo
done

echo "----------------------------------"
if [ "$passed" -eq "$total" ]; then
  echo "🎉 All $passed/$total CLI tests passed!"
else
  echo "⚠️  $passed/$total CLI tests passed."
fi
echo "----------------------------------"
