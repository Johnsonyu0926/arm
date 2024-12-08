#!/bin/sh
tts -t "$1" -p $2 -l $3 -o2 2>&1 >/dev/null
ffmpeg -y -f s16le -ar 16000 -ac 1 -i /tmp/tts.wav -filter:a "volume=15dB" /tmp/output.wav 2>&1 >/dev/null
ffplay -autoexit -nodisp /tmp/output.wav 2>&1 >/dev/null