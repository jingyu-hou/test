#!/usr/bin/env bash
# Build solver only for all package
# Usage: drag this file into Ubuntu terminal
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR/solver"

echo "=== Checking build tools ==="
for tool in gcc gfortran make; do
    if ! command -v "$tool" >/dev/null 2>&1; then
        echo "MISSING: $tool - installing..."
        sudo apt-get update -qq && sudo apt-get install -y build-essential gfortran make
    fi
done

echo "=== Building solver ==="
make clean 2>/dev/null || true
make -j$(nproc)

echo ""
echo "BUILD SUCCESS"
echo "Solver binary: $(pwd)/solver"
echo "Test: ./solver -i case.inp"
