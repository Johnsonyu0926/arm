#!/bin/bash

FIRMWARE_PATH="$1" # 从命令行参数获取固件文件路径
DEVICE_PATH="/dev/sda" # 固定设备路径

# 校验固件文件（假设使用 MD5 校验）
EXPECTED_MD5="your_expected_md5_checksum"
ACTUAL_MD5=$(md5sum "$FIRMWARE_PATH" | awk '{ print $1 }')

if [ "$ACTUAL_MD5" != "$EXPECTED_MD5" ]; then
    echo "MD5 checksum mismatch. Aborting upgrade."
    rm "$FIRMWARE_PATH"
    exit 1
fi

# 执行固件升级
fwup -a -i "$FIRMWARE_PATH" -d "$DEVICE_PATH"

if [ $? -eq 0 ]; then
    echo "Firmware upgrade successful. Rebooting..."
    reboot
else
    echo "Firmware upgrade failed."
    rm "$FIRMWARE_PATH"
    exit 1
fi