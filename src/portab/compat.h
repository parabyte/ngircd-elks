#ifndef NGIRCD_COMPAT_H
#define NGIRCD_COMPAT_H

#if defined(__ELKS__)

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>

#ifndef AF_UNSPEC
#define AF_UNSPEC 0xFFFF
#endif
#ifndef SO_ERROR
#define SO_ERROR 4
#endif

#ifndef HOST_NOT_FOUND
#define HOST_NOT_FOUND 1
#endif
#ifndef NO_DATA
#define NO_DATA 2
#endif
#ifndef NO_RECOVERY
#define NO_RECOVERY 3
#endif
#ifndef TRY_AGAIN
#define TRY_AGAIN 4
#endif

struct hostent {
	char *h_name;
	char **h_aliases;
	int h_addrtype;
	int h_length;
	char **h_addr_list;
};

#define h_addr h_addr_list[0]

#define h_errno ngircd_h_errno
extern int ngircd_h_errno;

struct hostent *gethostbyname(const char *name);
struct hostent *gethostbyaddr(const char *addr, int len, int type);

int inet_aton(const char *cp, struct in_addr *inp);
char *inet_ntoa(struct in_addr in);

int gethostname(char *name, size_t len);

int getsockopt(int sock, int level, int optname, void *optval, socklen_t *optlen);
int getsocknam(int sock, struct sockaddr *address, socklen_t *address_len, int peer);

int chroot(const char *path);
int raise(int sig);

int sigblock(int mask);
int sigsetmask(int mask);

#endif /* __ELKS__ */

#endif /* NGIRCD_COMPAT_H */
