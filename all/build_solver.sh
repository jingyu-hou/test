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
make -j$(nproc) SOLVER_NAME=AESim-FM

echo ""
echo "BUILD SUCCESS"
echo "Solver binary: $(pwd)/AESim-FM"
echo "Test: ./AESim-FM -i case.inp"
