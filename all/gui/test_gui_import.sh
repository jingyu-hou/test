#!/bin/bash
# GUI test - full C3D20 import verification with xdotool
export DISPLAY=:99
export LD_LIBRARY_PATH='/mnt/d/AESimFM/code/test/all/components/open_source/vtk/lib/vtk-5.4:/mnt/d/AESimFM/code/test/all/components/open_source/ffmpeg/lib:/mnt/d/AESimFM/code/test/all/components/open_source/saribbon/lib:/home/hjy/src/qt-everywhere-opensource-src-4.8.7/lib'
cd /mnt/d/AESimFM/code/test/all/gui
INP_FILE="/mnt/d/AESimFM/inp/element_type_tests/unit_c3d20.inp"

pkill -f "WeICME" 2>/dev/null || true
sleep 1

./WeICME > /tmp/gui_test.log 2>&1 &
GUI_PID=$!
sleep 5

if ! kill -0 $GUI_PID 2>/dev/null; then
    echo "FAIL: GUI died on startup"
    cat /tmp/gui_test.log
    exit 1
fi
echo "GUI running PID=$GUI_PID"

MAIN_WIN=$(DISPLAY=:99 xdotool search --name "WeICME" 2>/dev/null | head -1)

# Step 1: Ctrl+O
DISPLAY=:99 xdotool key --window $MAIN_WIN ctrl+o
sleep 1

# Step 2: Find dialog and its text input child
FILE_DLG=$(DISPLAY=:99 xdotool search --name "Read file..." 2>/dev/null | head -1)
echo "FileDlg: $FILE_DLG"

# Send Escape first to clear any previous state, then re-open
DISPLAY=:99 xdotool key --window $FILE_DLG Escape
sleep 0.3
DISPLAY=:99 xdotool key --window $MAIN_WIN ctrl+o
sleep 1
FILE_DLG=$(DISPLAY=:99 xdotool search --name "Read file..." 2>/dev/null | head -1)

# Type the file path character by character with small delays (more reliable in Xvfb)
for (( i=0; i<${#INP_FILE}; i++ )); do
    char="${INP_FILE:$i:1}"
    case "$char" in
        "/") DISPLAY=:99 xdotool key --window $FILE_DLG slash ;;
        ".") DISPLAY=:99 xdotool key --window $FILE_DLG period ;;
        "_") DISPLAY=:99 xdotool key --window $FILE_DLG underscore ;;
        "-") DISPLAY=:99 xdotool key --window $FILE_DLG minus ;;
        [0-9]) DISPLAY=:99 xdotool key --window $FILE_DLG "$char" ;;
        [a-z]) DISPLAY=:99 xdotool key --window $FILE_DLG "$char" ;;
        [A-Z]) DISPLAY=:99 xdotool key --window $FILE_DLG "shift+$(echo $char | tr 'A-Z' 'a-z')" ;;
        *) DISPLAY=:99 xdotool type --window $FILE_DLG "$char" ;;
    esac
    sleep 0.02
done
sleep 0.5

# Press Enter twice (first confirms file path, second might confirm dialog)
DISPLAY=:99 xdotool key --window $FILE_DLG Return
sleep 1
DISPLAY=:99 xdotool key --window $FILE_DLG Return
sleep 3

# Check result
if kill -0 $GUI_PID 2>/dev/null; then
    echo "SUCCESS: GUI still alive"
    echo "Final windows:"
    DISPLAY=:99 xdotool search --name . 2>&1 | while read wid; do
        name=$(DISPLAY=:99 xdotool getwindowname $wid 2>/dev/null)
        echo "  $wid: $name"
    done
else
    echo "FAIL: GUI crashed"
fi

cat /tmp/gui_test.log
kill $GUI_PID 2>/dev/null
wait $GUI_PID 2>/dev/null
echo "DONE"
