#!/bin/bash

echo "Dump the kernel zImage from Android device at first, eg..."
echo "dd if=/dev/block/platform/msm_sdcc.1/by-name/boot    of=/data/local/tmp/dump.img bs=4096"

pos=`grep -P -a -b -m 1 --only-matching '\x1F\x8B\x08' zImage | cut -f 1 -d :`
echo "Extracting gzip'd kernel image from file: zImage (start = $pos)"

if [ ! -z $pos ]; then
    echo "Dumping compressed image"
        dd if=zImage of=zImage_unpacked.gz bs=1 skip=$pos 2>/dev/null >/dev/null
    echo "Unzipping compressed image"
        gunzip -qf zImage_unpacked.gz
fi
