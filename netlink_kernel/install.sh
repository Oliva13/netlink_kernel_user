#!/bin/bash

echo "Start moduls send_m_kern.ko"

sudo cp ./send_m_kern.ko   /lib/modules/$(uname -r)/kernel/crypto/

sudo insmod send_m_kern.ko
