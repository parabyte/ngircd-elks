/*
 * ngIRCd -- The Next Generation IRC Daemon
 *
 * Statically defined configuration for the ELKS 16-bit build.
 */

#ifndef NGIRCD_CONFIG_ELKS_H
#define NGIRCD_CONFIG_ELKS_H

#define NGIRCD_ELKS 1
#define NGIRCD_DISABLE_SSL 1
#define NGIRCD_DISABLE_RESOLVER 1

#define PACKAGE "ngircd"
#define PACKAGE_NAME "ngIRCd"
#define PACKAGE_TARNAME "ngircd"
#define PACKAGE_VERSION "ELKS"
#define PACKAGE_STRING "ngIRCd ELKS"
#define PACKAGE_BUGREPORT "ngircd@lists.barton.de"
#define PACKAGE_URL "https://ngircd.barton.de/"

#define VERSION "ELKS"

#define SYSCONFDIR "/etc"
#define DOCDIR "/usr/share/doc/ngircd"

#define HOST_CPU "i8086"
#define HOST_VENDOR "elks"
#define HOST_OS "elks"

#define STDC_HEADERS 1
#define PROTOTYPES 1

#define HAVE_STDINT_H 1
#define HAVE_STDBOOL_H 1
#define HAVE_STDDEF_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_SYS_SOCKET_H 1
#define HAVE_SYS_TIME_H 1
#define HAVE_SYS_SELECT_H 1
#define HAVE_UNISTD_H 1
#define HAVE_STRING_H 1
#define HAVE_STDLIB_H 1
#define HAVE_FCNTL_H 1
#define HAVE_NETDB_H 1
#define HAVE_NETINET_IN_H 1
#define HAVE_ARPA_INET_H 1
#define HAVE_ERRNO_H 1
#define HAVE_INTTYPES_H 1
#define HAVE_MALLOC_H 1

#define HAVE_SELECT 1

#define HAVE_WAITPID 1

#ifndef AF_UNSPEC
#define AF_UNSPEC (-1)
#endif
#ifndef SO_ERROR
#define SO_ERROR 4
#endif

#ifdef __WATCOMC__
extern int chroot(const char *);
extern int setpgrp(int, int);
#endif

#define HAVE_socklen_t 1

#endif /* NGIRCD_CONFIG_ELKS_H */

/* -eof- */
