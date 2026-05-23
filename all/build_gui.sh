#!/usr/bin/env bash
# Build GUI only for all package
# Usage: drag this file into Ubuntu terminal
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR/gui"

echo "=== Loading Qt4 environment ==="
if [ -f /mnt/d/AESgui_for_linux/env_qt4.sh ]; then
    source /mnt/d/AESgui_for_linux/env_qt4.sh
elif [ -f /mnt/c/code/AESgui_for_linux/env_qt4.sh ]; then
    source /mnt/c/code/AESgui_for_linux/env_qt4.sh
fi

for tool in qmake make; do
    if ! command -v "$tool" >/dev/null 2>&1; then
        echo "MISSING: $tool"
        echo "  Please install Qt4 SDK: source /mnt/d/AESgui_for_linux/env_qt4.sh"
        exit 1
    fi
done

echo "=== Building GUI ==="
make clean 2>/dev/null || true
qmake QProject_x64.pro
if [ -d /home/hjy/src/qt-everywhere-opensource-src-4.8.7 ]; then
    sed -i "s#/share/apps/Qt-4.8.7#/home/hjy/src/qt-everywhere-opensource-src-4.8.7#g" Makefile
fi
make -j$(nproc)

echo ""
echo "BUILD SUCCESS"
echo "GUI binary: $(pwd)/WeICME"
echo "Run: ./WeICME"

