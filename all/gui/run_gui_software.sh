#!/usr/bin/env bash
set -e

cd "$(dirname "$0")"
source /mnt/d/AESgui_for_linux/env_qt4.sh

export LIBGL_ALWAYS_SOFTWARE=1
export GALLIUM_DRIVER=llvmpipe
export MESA_LOADER_DRIVER_OVERRIDE=llvmpipe
export QT_XCB_FORCE_SOFTWARE_OPENGL=1

exec ./GUI "$@"
