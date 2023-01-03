#!/bin/sh

mkdir -p /root/.config/gdb
echo add-auto-load-safe-path /workspaces/ArmOS/src/kernel/.gdbinit >> /root/.config/gdb/gdbinit
