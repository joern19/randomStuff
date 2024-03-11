#!/bin/sh

efibootmgr --create --disk /dev/nvme0n1 --part 1 --label "Arch Linux LTS" --loader /vmlinuz-linux-lts -u 'cryptdevice=UUID=2d0d9987-40c5-4194-93bf-6dcb04ba53cd:root root=/dev/mapper/root rw apparmor=1 security=apparmor initrd=\intel-ucode.img initrd=\initramfs-linux-lts.img'