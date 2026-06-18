#!/usr/bin/env bash
# monitor_solver.sh — 启动求解器,每2s采集CPU/内存,分阶段统计,求解结束后输出报告
# Usage: bash monitor_solver.sh /path/to/solver -i jobname

set -euo pipefail

if [ $# -lt 2 ]; then
    echo "Usage: bash monitor_solver.sh <solver_path> <solver_args...>"
    echo "Example: bash monitor_solver.sh ./solver -i Quenching"
    exit 1
fi

SOLVER="$1"
shift
JOBNAME="${@: -1}"

SAMPLES_FILE="${JOBNAME}_resource_samples.csv"
OUTFILE="${JOBNAME}_resource.txt"
SOLVER_RC_FILE=$(mktemp /tmp/solver_rc_XXXXXX)
TOTAL_MEM_KB=$(awk '/MemTotal/{print $2}' /proc/meminfo 2>/dev/null || echo 16000000)

cleanup() {
    if [ -n "${MONITOR_PID:-}" ]; then
        kill "$MONITOR_PID" 2>/dev/null || true
        wait "$MONITOR_PID" 2>/dev/null || true
    fi

    SOLVER_RC=0
    if [ -f "$SOLVER_RC_FILE" ]; then
        SOLVER_RC=$(cat "$SOLVER_RC_FILE" 2>/dev/null || echo 0)
    fi

    if [ ! -f "$SAMPLES_FILE" ]; then
        echo "[monitor] No samples file found"
        rm -f "$SOLVER_RC_FILE"
        return
    fi

    # Total sample count
    N=$(tail -n +2 "$SAMPLES_FILE" 2>/dev/null | grep -c . || echo 0)
    if [ "$N" -eq 0 ]; then
        echo "[monitor] No data collected"
        rm -f "$SOLVER_RC_FILE"
        return
    fi

    # Split samples into idle (<2% CPU) and active (>=2% CPU)
    # Also compute aggregate stats
    awk -F',' -v N="$N" -v TOTAL_MEM_KB="$TOTAL_MEM_KB" '
    NR == 1 { next }  # skip header
    {
        cpu = $2 + 0; mem = $3 + 0; rss = $4 + 0; vsz = $5 + 0

        # All samples
        all_cpu_sum += cpu; all_mem_sum += mem
        if (cpu > all_cpu_max) all_cpu_max = cpu
        if (mem > all_mem_max) all_mem_max = mem

        # RSS peak
        if (rss > rss_max) { rss_max = rss; rss_peak_time = $1 }
        if (vsz > vsz_max) vsz_max = vsz

        # Active vs idle
        if (cpu >= 2.0) {
            active_cpu_sum += cpu; active_n++
            if (cpu > active_cpu_max) active_cpu_max = cpu
        } else {
            idle_cpu_sum += cpu; idle_n++
        }
    }
    END {
        all_cpu_avg = (N > 0) ? all_cpu_sum / N : 0
        all_mem_avg = (N > 0) ? all_mem_sum / N : 0
        active_cpu_avg = (active_n > 0) ? active_cpu_sum / active_n : 0
        idle_cpu_avg   = (idle_n > 0)   ? idle_cpu_sum / idle_n : 0
        rss_peak_pct   = (rss_max * 100.0) / TOTAL_MEM_KB

        printf "%.2f %.2f %.2f %.2f %d %.2f %.2f %.2f %.2f %d %d %.0f\n",
            all_cpu_avg, all_cpu_max, active_cpu_avg, active_cpu_max, active_n,
            all_mem_avg, all_mem_max, rss_max, rss_peak_pct,
            idle_n, active_n, vsz_max
    }' "$SAMPLES_FILE" > /tmp/monitor_stats_$$.txt

    read -r cpu_all_avg cpu_all_max cpu_active_avg cpu_active_max active_n \
            mem_all_avg mem_all_max rss_max rss_peak_pct \
            idle_n active_n2 vsz_max < /tmp/monitor_stats_$$.txt
    rm -f /tmp/monitor_stats_$$.txt

    rss_max_mb=$(awk "BEGIN {printf \"%.1f\", $rss_max / 1024}")
    vsz_max_mb=$(awk "BEGIN {printf \"%.1f\", $vsz_max / 1024}")
    DURATION=$((N * 2))
    active_pct=$(awk "BEGIN {printf \"%.0f\", ($active_n / $N) * 100}")
    idle_pct=$((100 - active_pct))

    # Metric evaluation
    cpu_pass="PASS"
    if [ "$(awk "BEGIN {print ($cpu_active_avg < 75)}")" = "1" ]; then cpu_pass="WARN"; fi
    mem_pass="PASS"
    if [ "$(awk "BEGIN {print ($rss_peak_pct > 70)}")" = "1" ]; then mem_pass="WARN"; fi

    STATUS_DESC="Completed (RC=$SOLVER_RC)"
    if [ "$SOLVER_RC" != "0" ] 2>/dev/null; then
        STATUS_DESC="Stopped (RC=$SOLVER_RC)"
    fi

    {
        echo "=============================================="
        echo "  Solver Resource Report: $JOBNAME"
        echo "=============================================="
        echo "Duration:      ~${DURATION}s  |  Samples: $N (2s interval)"
        echo "Status:        $STATUS_DESC"
        echo "System RAM:    $((TOTAL_MEM_KB / 1024 / 1024))GB"
        echo ""

        echo "--- Overall Resource Usage ---"
        printf "  %-16s %8s %8s %8s\n" "Metric" "Average" "Max" "Target"
        printf "  %-16s %8s %8s %8s\n" "--------" "--------" "--------" "--------"
        printf "  %-16s %7.1f%% %7.1f%% %8s\n" "CPU (all phases)" "$cpu_all_avg" "$cpu_all_max" ">=50%"
        printf "  %-16s %7.1f%% %7.1f%%\n" "MEM% (all)" "$mem_all_avg" "$mem_all_max"
        printf "  %-16s %7sMB %7sMB %8s\n" "RSS (peak)" "$rss_max_mb" "$rss_max_mb" "<=70%RAM"
        printf "  %-16s %7sMB\n" "VSZ (peak)" "$vsz_max_mb"
        printf "  %-16s %5.1f%% of system RAM\n" "RSS Peak%" "$rss_peak_pct"
        echo ""

        echo "--- Solving Phase (CPU >= 2%) ---"
        printf "  %-16s %8s %8s %8s\n" "Metric" "Average" "Max" "Target"
        printf "  %-16s %8s %8s %8s\n" "--------" "--------" "--------" "--------"
        printf "  %-16s %7.1f%% %7.1f%% %8s  [%s]\n" "CPU (active)" "$cpu_active_avg" "$cpu_active_max" ">=75%" "$cpu_pass"
        printf "  %-16s %6d samples (%d%% of total)\n" "Active" "$active_n" "$active_pct"
        echo ""

        echo "--- Idle / I/O Phase (CPU < 2%) ---"
        printf "  %-16s %6d samples (%d%% of total)\n" "Idle" "$idle_n" "$idle_pct"
        echo ""

        echo "--- Memory Summary ---"
        printf "  %-16s %8s %8s %8s\n" "Metric" "Value" "Target" "Status"
        printf "  %-16s %8s %8s %8s\n" "--------" "--------" "--------" "--------"
        printf "  %-16s %7.1fMB %8s  [%s]\n" "RSS Peak" "$rss_max_mb" "<=70% RAM" "$mem_pass"
        printf "  %-16s %7.1fMB\n" "VSZ Peak" "$vsz_max_mb"
        echo ""

        echo "--- Raw Samples (first 5 + last 5) ---"
        printf "  %-10s %7s %7s %8s %8s\n" "Time" "CPU%" "MEM%" "RSS(KB)" "VSZ(KB)"
        tail -n +2 "$SAMPLES_FILE" | head -5 | awk -F',' '{printf "  %-10s %7s %7s %8s %8s\n", $1, $2, $3, $4, $5}'
        echo "  ..."
        tail -5 "$SAMPLES_FILE" | awk -F',' '{printf "  %-10s %7s %7s %8s %8s\n", $1, $2, $3, $4, $5}'
    } > "$OUTFILE"

    echo ""
    echo "[monitor] Report saved: $OUTFILE"
    echo "  Overall CPU:  ${cpu_all_avg}% avg / ${cpu_all_max}% max"
    echo "  Active  CPU:  ${cpu_active_avg}% avg / ${cpu_active_max}% max  [${cpu_pass}]"
    echo "  Peak    RSS:  ${rss_max_mb}MB (${rss_peak_pct}% of RAM)  [${mem_pass}]"

    rm -f "$SOLVER_RC_FILE"
}
trap cleanup EXIT

# Init
echo "time,cpu,mem,rss,vsz" > "$SAMPLES_FILE"

echo "[monitor] Launching: $SOLVER $*"

# Background monitor
(
    while true; do
        PID=$(pgrep -x solver 2>/dev/null | head -1)
        if [ -z "$PID" ]; then
            break
        fi
        raw=$(ps -p "$PID" -o %cpu,%mem,rss,vsz --no-headers 2>/dev/null)
        if [ -n "$raw" ]; then
            read -r cpu mem rss vsz <<< "$raw"
            echo "$(date +%H:%M:%S),$cpu,$mem,$rss,$vsz" >> "$SAMPLES_FILE"
        fi
        sleep 2
    done
) &
MONITOR_PID=$!

# Run solver
set +e
OMP_NUM_THREADS="${OMP_NUM_THREADS:-4}" "$SOLVER" "$@"
echo "$?" > "$SOLVER_RC_FILE"
set -e
