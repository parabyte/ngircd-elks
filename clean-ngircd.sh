#!/bin/sh
set -e

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname "$0")" && pwd)
TOPDIR=${TOPDIR:-$(cd "$SCRIPT_DIR/.." && pwd)}

make -C "$TOPDIR" -f "$TOPDIR/ngircd/Makefile.owc" TOPDIR="$TOPDIR" clean

rm -f \
	"$TOPDIR/ngircd/ngircd.os2" \
	"$TOPDIR/elkscmd/rootfs_template/sbin/ngircd"
