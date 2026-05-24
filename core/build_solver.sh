#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/solver/src"
make -B -j"${JOBS:-2}"
