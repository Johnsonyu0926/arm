#!/bin/sh
vol=$(ffmpeg -i "$1" -af "volumedetect" -f null /dev/null 2>&1| grep mean_volume| cut -f 2 -d ':' | cut -f 2 -d ' ' | cut -f 2 -d '-' | cut -f 1 -d '.')
echo "get val: $vol"
if [ "$vol" -gt 14 ]
then
  x=$((vol-14))
  ffmpeg -y -i "$1" -filter:a "volume=${x}dB" /tmp/vol."$2"
  cp /tmp/vol."$2" "$1"
fi