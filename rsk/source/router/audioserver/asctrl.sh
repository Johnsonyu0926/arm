#!/bin/bash

AUDIO_SERVER_PID_FILE="/var/run/audioserver.pid"
AUDIO_SERVER_EXECUTABLE="/usr/bin/audioserver"

log() {
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $1"
}

check_root() {
    if [ "$(id -u)" != "0" ]; then
        log "This script must be run as root" 1>&2
        exit 1
    fi
}

is_running() {
    if [ -f "$AUDIO_SERVER_PID_FILE" ]; then
        PID=$(cat "$AUDIO_SERVER_PID_FILE")
        if [ -n "$PID" ] && kill -0 "$PID" 2>/dev/null; then
            return 0
        fi
    fi
    return 1
}

start_audioserver() {
    if is_running; then
        log "Audioserver is already running."
    else
        log "Starting audioserver..."
        if [ ! -x "$AUDIO_SERVER_EXECUTABLE" ]; then
            log "Error: $AUDIO_SERVER_EXECUTABLE not found or not executable."
            exit 1
        fi
        $AUDIO_SERVER_EXECUTABLE &
        echo $! > "$AUDIO_SERVER_PID_FILE"
        sleep 1
        if is_running; then
            log "Audioserver started successfully."
        else
            log "Error: Audioserver failed to start."
            exit 1
        fi
    fi
}

stop_audioserver() {
    if is_running; then
        PID=$(cat "$AUDIO_SERVER_PID_FILE")
        log "Stopping audioserver (PID: $PID)..."
        kill "$PID"
        for i in {1..10}; do
            if ! is_running; then
                break
            fi
            sleep 1
        done
        if is_running; then
            log "Error: Audioserver did not stop. Forcing termination."
            kill -9 "$PID"
            sleep 1
        fi
        rm -f "$AUDIO_SERVER_PID_FILE"
        log "Audioserver stopped."
    else
        log "Audioserver is not running."
    fi
}

restart_audioserver() {
    stop_audioserver
    start_audioserver
}

check_root

case "$1" in
    start)
        start_audioserver
        ;;
    stop)
        stop_audioserver
        ;;
    restart)
        restart_audioserver
        ;;
    status)
        if is_running; then
            log "Audioserver is running."
        else
            log "Audioserver is not running."
        fi
        ;;
    *)
        echo "Usage: $0 {start|stop|restart|status}"
        exit 1
        ;;
esac

exit 0

# By GST ARMV8 GCC13.2 asctri.sh