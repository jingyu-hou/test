#!/bin/bash
# build_solver.sh — Build AESim-FM solver with CalculiX open-source boundary split
# Usage:
#   ./build_solver.sh              # traditional build (override .o + libcalculix_base.a)
#   ./build_solver.sh split        # split build (libaesim_solver_ext.a + libcalculix_base.a)
#   ./build_solver.sh clean        # clean build artifacts

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

CALCULIX_BASE="$SCRIPT_DIR/../components/open_source/calculix/libcalculix_base.a"
CALCULIX_SRC="D:/ccx/src"   # WSL path: /mnt/d/ccx/src

# Check prerequisites
check_prereqs() {
    if ! command -v gfortran &>/dev/null; then
        echo "ERROR: gfortran not found"
        exit 1
    fi
    if ! command -v cc &>/dev/null; then
        echo "ERROR: cc not found"
        exit 1
    fi
    echo "Prerequisites OK: gfortran $(gfortran -dumpversion), cc available"
}

# Build CalculiX base library from D:\ccx\src source
build_calculix_base() {
    local WSL_SRC="/mnt/d/ccx/src"

    if [ -f "$CALCULIX_BASE" ]; then
        echo "CalculiX base library exists: $CALCULIX_BASE"
        return 0
    fi

    echo "Building libcalculix_base.a from $WSL_SRC ..."
    if [ ! -d "$WSL_SRC" ]; then
        echo "ERROR: CalculiX source not found at $WSL_SRC"
        echo "       Source should be at D:\\ccx\\src (WSL: /mnt/d/ccx/src)"
        exit 1
    fi

    local OUT_DIR="$SCRIPT_DIR/../components/open_source/calculix"
    mkdir -p "$OUT_DIR"

    cd "$WSL_SRC"
    make -f Makefile_MT WeICME_MT.a
    cp WeICME_MT.a "$OUT_DIR/libcalculix_base.a"
    cd "$SCRIPT_DIR"

    echo "libcalculix_base.a built successfully ($(stat -c%s "$CALCULIX_BASE") bytes)"
}

# Build solver
case "${1:-}" in
    clean)
        echo "Cleaning build artifacts..."
        make clean
        echo "Clean complete."
        ;;
    split)
        check_prereqs
        build_calculix_base
        echo "Building AESim-FM (split mode)..."
        make all_split
        echo "Build complete: AESim-FM_split"
        ;;
    all|both)
        check_prereqs
        build_calculix_base
        echo "Building AESim-FM (traditional mode)..."
        make AESim-FM
        echo "Building AESim-FM (split mode)..."
        make all_split
        echo "Both builds complete: AESim-FM, AESim-FM_split"
        ;;
    *)
        check_prereqs
        build_calculix_base
        echo "Building AESim-FM (traditional mode)..."
        make AESim-FM
        echo "Build complete: AESim-FM"
        ;;
esac
