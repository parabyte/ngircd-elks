#!/usr/bin/env bash
set -e

SCRIPT_DIR=$(cd -- "$(dirname "${BASH_SOURCE[0]}")" && pwd)
cd "$SCRIPT_DIR"

if [[ "${1:-}" == "clean" ]]; then
	rm -rf build-elks
	rm -f src/config.h
	exit 0
fi

if [[ -z "${NGIRCD_ELKS_ENV:-}" ]]; then
	# shellcheck source=/dev/null
	source "$SCRIPT_DIR/env-elks-ngircd.sh"
fi

# shellcheck source=/dev/null
source "$TOPDIR/libc/watcom.model"
MODEL=${MODEL:-l}

LIBC_LIB="$TOPDIR/libc/libc${MODEL}.lib"
EWLINK="$TOPDIR/elks/tools/bin/ewlink"

if [[ ! -f "$LIBC_LIB" ]]; then
	cat <<EOF >&2
Missing ELKS Watcom libc archive: $LIBC_LIB
Please build it first, for example:
  make -C "$TOPDIR/libc" -f watcom.mk MODEL=${MODEL}
EOF
	exit 1
fi

if [[ ! -x "$EWLINK" ]]; then
	echo "ELKS ewlink helper not found or not executable: $EWLINK" >&2
	exit 1
fi

BUILD_ROOT="$SCRIPT_DIR/build-elks"
OBJ_DIR="$BUILD_ROOT/obj"
BIN_DIR="$BUILD_ROOT/bin"

mkdir -p "$OBJ_DIR/ngircd" "$OBJ_DIR/portab" "$OBJ_DIR/tool" "$OBJ_DIR/ipaddr" "$BIN_DIR"

cp src/config-elks.h src/config.h

if command -v wcc >/dev/null 2>&1; then
    WCC=${WCC:-$(command -v wcc)}
elif [[ -x $WATCOM/binl64/wcc ]]; then
    WCC=${WCC:-$WATCOM/binl64/wcc}
elif [[ -x $WATCOM/binl/wcc ]]; then
    WCC=${WCC:-$WATCOM/binl/wcc}
else
    WCC=${WCC:-wcc}
fi

CCOPTSS="-os -bt=none -0 -zq -s -ml -wx -zastd=c99 -zls"
CCOPTST="-oaxet -oh -ol+ -bt=none -0 -zq -s -ml -wx -zastd=c99 -zls"

INCLUDES=(
	-i="$SCRIPT_DIR/src"
	-i="$SCRIPT_DIR/src/ngircd"
	-i="$SCRIPT_DIR/src/portab"
	-i="$SCRIPT_DIR/src/tool"
	-i="$SCRIPT_DIR/src/ipaddr"
	-i="$SCRIPT_DIR/include"
	-i="$TOPDIR/include"
	-i="$TOPDIR/libc/include"
	-i="$TOPDIR/elks/include"
	-i="$TOPDIR/libc/include/watcom"
	-i="$TOPDIR/libc/include/c86"
)

DEFINES=(-dNGIRCD_ELKS -dNGIRCD_DISABLE_SSL -dNGIRCD_DISABLE_RESOLVER)

OBJ_FILES=()

build_sources() {
	local opts=$1
	local module=$2
	shift 2
	for src in "$@"; do
		local base=${src%.c}
		local obj="$OBJ_DIR/$module/${base}.obj"
		echo "$WCC $opts ${DEFINES[*]} ${INCLUDES[*]} src/$module/$src -fo=$obj"
		$WCC $opts "${DEFINES[@]}" "${INCLUDES[@]}" "src/$module/$src" -fo="$obj"
		OBJ_FILES+=("$obj")
	done
}

build_sources "$CCOPTSS" portab \
	elks-stubs.c \
	strdup.c \
	strlcpy.c \
	strndup.c \
	strtok_r.c \
	vsnprintf.c \
	waitpid.c

build_sources "$CCOPTSS" tool \
	tool.c

build_sources "$CCOPTSS" ipaddr \
	ng_ipaddr.c

build_sources "$CCOPTST" ngircd \
	array.c \
	channel.c \
	class.c \
	client.c \
	client-cap.c \
	conf.c \
	conn.c \
	conn-encoding.c \
	conn-func.c \
	conn-ssl.c \
	conn-zip.c \
	hash.c \
	io.c \
	irc.c \
	irc-cap.c \
	irc-channel.c \
	irc-encoding.c \
	irc-info.c \
	irc-login.c \
	irc-metadata.c \
	irc-mode.c \
	irc-op.c \
	irc-oper.c \
	irc-server.c \
	irc-write.c \
	lists.c \
	log.c \
	login.c \
	match.c \
	ngircd.c \
	numeric.c \
	op.c \
	pam.c \
	parse.c \
	proc.c \
	resolve.c \
	sighandlers.c

OUTPUT_OS2="$BIN_DIR/ngircd.os2"

echo "$EWLINK --stack 0x1000 --heap 0x2000 -o $OUTPUT_OS2 ${OBJ_FILES[*]}"
"$EWLINK" --stack 0x1000 --heap 0x2000 -o "$OUTPUT_OS2" "${OBJ_FILES[@]}"

find "$OBJ_DIR" -name '*.obj' -delete
find "$OBJ_DIR" -name '*.err' -delete
