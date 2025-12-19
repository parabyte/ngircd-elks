#ifndef NGIRCD_CONFIG_H
#define NGIRCD_CONFIG_H
/*
 * Minimal config.h for ELKS/OpenWatcom builds.
 * Autoconf isn't used for the ELKS port.
 */

/*
 * Workaround for ELKS headers which use #if UNUSED.
 * Include them here before portab.h defines UNUSED as an attribute.
 */
#define UNUSED 0
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#undef UNUSED

#define HAVE_CONFIG_H 1
#define PROTOTYPES 1
#define STDC_HEADERS 1

#define HAVE_ARPA_INET_H 1
#define HAVE_CTYPE_H 1
#define HAVE_FCNTL_H 1
#define HAVE_INTTYPES_H 1
#define HAVE_NETDB_H 1
#define HAVE_NETINET_IN_H 1
#define HAVE_STDDEF_H 1
#define HAVE_STDINT_H 1
#define HAVE_STDIO_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRING_H 1
#define HAVE_SYS_SOCKET_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TIME_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_SYS_WAIT_H 1
#define HAVE_TIME_H 1
#define HAVE_UNISTD_H 1

#define HAVE_INET_ATON 1
#define HAVE_SELECT 1
#define HAVE_SETSID 1
#define HAVE_SIGACTION 1
#define HAVE_socklen_t 1

#define HOST_OS "elks"
#define HOST_CPU "i86"
#define HOST_VENDOR "unknown"

#define PACKAGE "ngircd"
#define PACKAGE_NAME "ngIRCd"
#define PACKAGE_VERSION "elks"
#define VERSION "elks"

#define DOCDIR "/doc"

#endif
