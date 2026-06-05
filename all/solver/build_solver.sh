#!/bin/bash
# build_solver.sh — AESim-FM solver official build entry point
#
# Architecture:
#   AESim-FM        = WeICME.o + solver/*.o (override layer) + libcalculix_base.a + numerical libs
#   AESim-FM_split  = WeICME.o + libaesim_solver_ext.a       + libcalculix_base.a + numerical libs
#
# Both produce identical DAT output; split build makes the open-source boundary explicit.
#
# Usage:
#   ./build_solver.sh              # both traditional + split (default)
#   ./build_solver.sh build        # traditional only
#   ./build_solver.sh split        # split only
#   ./build_solver.sh clean        # remove all build artifacts
#   ./build_solver.sh check        # check prerequisites only, no build

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

CALCULIX_BASE="$SCRIPT_DIR/../components/open_source/calculix/libcalculix_base.a"
OUT_DIR="$SCRIPT_DIR/../components/open_source/calculix"

# CalculiX 2.15 source: D:\ccx\src
# In WSL: /mnt/d/ccx/src  (Ubuntu default mount)
# In Git Bash/Cygwin: /d/ccx/src
find_ccx_src() {
    for path in "/mnt/d/ccx/src" "/d/ccx/src"; do
        if [ -d "$path" ] && [ -f "$path/WeICME.h" ]; then
            echo "$path"
            return 0
        fi
    done
    echo ""
    return 1
}

# ── prerequisite check ──────────────────────────────────────────

check_prereqs() {
    local ok=0

    if ! command -v gfortran &>/dev/null; then
        echo "  MISSING: gfortran"
        ok=1
    else
        echo "  gfortran $(gfortran -dumpversion)"
    fi

    if ! command -v cc &>/dev/null; then
        echo "  MISSING: cc"
        ok=1
    else
        echo "  cc ($(which cc))"
    fi

    if ! command -v make &>/dev/null; then
        echo "  MISSING: make"
        ok=1
    else
        echo "  make ($(make --version 2>&1 | head -1))"
    fi

    if ! command -v ar &>/dev/null; then
        echo "  MISSING: ar (required for static library)"
        ok=1
    else
        echo "  ar available"
    fi

    return $ok
}

# ── CalculiX base library ───────────────────────────────────────

build_calculix_base() {
    mkdir -p "$OUT_DIR"

    if [ -f "$CALCULIX_BASE" ]; then
        local sz
        sz=$(stat -c%s "$CALCULIX_BASE" 2>/dev/null || echo 0)
        if [ "$sz" -gt 1000000 ]; then
            echo "  libcalculix_base.a exists ($sz bytes), skip build"
            return 0
        fi
        echo "  libcalculix_base.a exists but too small ($sz bytes), rebuilding..."
    fi

    local CCX_SRC
    CCX_SRC=$(find_ccx_src)
    if [ -z "$CCX_SRC" ]; then
        echo ""
        echo "ERROR: CalculiX 2.15 source not found."
        echo "  Expected at: D:\\ccx\\src (WSL: /mnt/d/ccx/src)"
        echo ""
        echo "  To build manually:"
        echo "    cd D:\\ccx\\src"
        echo "    make -f Makefile_MT WeICME_MT.a"
        echo "    cp WeICME_MT.a  <path>/components/open_source/calculix/libcalculix_base.a"
        echo ""
        return 1
    fi

    echo "  Building libcalculix_base.a from ${CCX_SRC} ..."
    cd "$CCX_SRC"
    make -f Makefile_MT WeICME_MT.a
    cp WeICME_MT.a "$CALCULIX_BASE"
    cd "$SCRIPT_DIR"

    local sz
    sz=$(stat -c%s "$CALCULIX_BASE" 2>/dev/null || echo 0)
    echo "  libcalculix_base.a built: $sz bytes"
}

# ── build targets ───────────────────────────────────────────────

do_build() {
    echo ""
    echo "=== Traditional build (AESim-FM) ==="
    echo "    Link: WeICME.o + solver/*.o + libcalculix_base.a + numerical libs"
    make AESim-FM
    echo "    -> $(ls -lh AESim-FM | awk '{print $5, $NF}')"
}

do_split() {
    echo ""
    echo "=== Split build (AESim-FM_split) ==="
    echo "    Pack: solver/*.o -> libaesim_solver_ext.a"
    echo "    Link: WeICME.o + libaesim_solver_ext.a + libcalculix_base.a + numerical libs"
    make all_split
    echo "    -> $(ls -lh AESim-FM_split | awk '{print $5, $NF}')"
    echo "    -> $(ls -lh libaesim_solver_ext.a | awk '{print $5, $NF}')"
}

do_clean() {
    echo "Cleaning build artifacts..."
    make clean
    echo "Clean complete."
}

do_check() {
    echo "Build environment check:"
    check_prereqs
    echo ""

    if [ -f "$CALCULIX_BASE" ]; then
        local sz
        sz=$(stat -c%s "$CALCULIX_BASE" 2>/dev/null || echo 0)
        echo "  libcalculix_base.a : present ($sz bytes)"
    else
        local CCX_SRC
        CCX_SRC=$(find_ccx_src)
        if [ -n "$CCX_SRC" ]; then
            echo "  libcalculix_base.a : NOT FOUND (can build from $CCX_SRC)"
        else
            echo "  libcalculix_base.a : NOT FOUND (CalculiX source not found)"
        fi
    fi

    for lib in libspooles.so libspoolesMT.so libarpack.so liblapack.so libblas.so; do
        local lp="$SCRIPT_DIR/../components/open_source"
        local found
        found=$(find "$lp" -name "$lib" 2>/dev/null | head -1)
        if [ -n "$found" ]; then
            echo "  $lib : $found"
        else
            echo "  $lib : NOT FOUND"
        fi
    done
}

# ── print test commands ─────────────────────────────────────────

print_test_hint() {
    echo ""
    echo "── Quick verification ──"
    echo "  cd $(pwd)"
    echo "  ./AESim-FM -i unit_c3d4"
    echo "  ./AESim-FM_split -i unit_c3d4"
    echo "  diff <(./AESim-FM -i unit_c3d8 2>&1 >/dev/null; cat unit_c3d8.dat) \\"
    echo "       <(./AESim-FM_split -i unit_c3d8 2>&1 >/dev/null; cat unit_c3d8.dat)"
    echo "  # DAT output should be identical"
    echo ""
}

# ── main ────────────────────────────────────────────────────────

echo "AESim-FM Solver Build"
echo "  CalculiX 2.15 baseline (Copyright 1998-2018 Guido Dhondt, GPL v2+)"
echo ""

case "${1:-}" in
    clean)
        do_clean
        ;;
    check)
        do_check
        ;;
    split)
        echo "--- Prerequisites ---"
        check_prereqs
        echo "--- CalculiX base library ---"
        build_calculix_base
        do_split
        print_test_hint
        ;;
    build|traditional)
        echo "--- Prerequisites ---"
        check_prereqs
        echo "--- CalculiX base library ---"
        build_calculix_base
        do_build
        print_test_hint
        ;;
    *)
        # Default: both builds
        echo "--- Prerequisites ---"
        check_prereqs
        echo "--- CalculiX base library ---"
        build_calculix_base
        do_build
        do_split
        print_test_hint
        ;;
esac
