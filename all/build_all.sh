#!/usr/bin/env bash
# Build complete software (GUI + Solver) for all package
# Usage: drag this file into Ubuntu terminal
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

echo "============================================"
echo "  Building AESim-FM (Complete)"
echo "============================================"

# --- Step 1: Build Solver ---
echo ""
echo "=== Step 1/2: Building Solver ==="
cd "$SCRIPT_DIR/solver"

for tool in gcc gfortran make; do
    if ! command -v "$tool" >/dev/null 2>&1; then
        echo "Installing build tools..."
        sudo apt-get update -qq && sudo apt-get install -y build-essential gfortran make
        break
    fi
done

make clean 2>/dev/null || true
make -j$(nproc) SOLVER_NAME=AESim-FM
echo "Solver built: $(pwd)/AESim-FM"

# --- Step 2: Build GUI ---
echo ""
echo "=== Step 2/2: Building GUI ==="
cd "$SCRIPT_DIR/gui"

echo "=== Loading Qt4 environment ==="
if [ -f /mnt/d/AESgui_for_linux/env_qt4.sh ]; then
    source /mnt/d/AESgui_for_linux/env_qt4.sh
elif [ -f /mnt/c/code/AESgui_for_linux/env_qt4.sh ]; then
    source /mnt/c/code/AESgui_for_linux/env_qt4.sh
fi

if ! command -v qmake >/dev/null 2>&1; then
    echo "ERROR: qmake not found. Source env_qt4.sh first."
    exit 1
fi

make clean 2>/dev/null || true
qmake QProject_x64.pro
make -j$(nproc)
echo "GUI built: $(pwd)/WeICME"

# --- Done ---
echo ""
echo "============================================"
echo "  BUILD COMPLETE"
echo "============================================"
echo "  Solver: $SCRIPT_DIR/solver/AESim-FM"
echo "  GUI:    $SCRIPT_DIR/gui/WeICME"
echo ""
echo "  Run: cd $SCRIPT_DIR/gui && ./WeICME"
echo "============================================"
