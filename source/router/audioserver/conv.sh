#!/bin/sh
echo "start conv..."
result=$(ffprobe "$1" 2>&1 | grep "32 kb")
if [ "$result" != "" ]
then
    echo "is 32 kb, should conv."
    ffmpeg -y -i "$1" -acodec pcm_s16le -f s16le -ac 2 -ar 16000 /tmp/x.pcm
    ffmpeg -y -f s16be -ac 2 -ar 16000 -acodec pcm_s16le -i /tmp/x.pcm /tmp/new_mp3.mp3
    cp /tmp/new_mp3.mp3 "$1"
    echo "conv success!"
else
  echo "is not 32kb, skip."
  exit
fi