/*
 * ELKS compatibility stubs.
 */

#include "compat.h"

#if defined(__ELKS__)

#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#if defined(__WATCOMC__)
#include <syselks.h>
#endif

int ngircd_h_errno;

static struct hostent elks_hostent;
static struct in_addr elks_in_addr;
static char *elks_aliases[] = { NULL };
static char *elks_addr_list[] = { (char *)&elks_in_addr, NULL };

static int
inet_aton_parse(const char *cp, struct in_addr *inp)
{
	unsigned long parts[4];
	unsigned long val;
	int i;

	if (!cp || !inp)
		return 0;

	for (i = 0; i < 4; i++) {
		if (*cp < '0' || *cp > '9')
			return 0;
		val = 0;
		while (*cp >= '0' && *cp <= '9') {
			val = (val * 10) + (unsigned long)(*cp - '0');
			if (val > 255)
				return 0;
			cp++;
		}
		parts[i] = val;
		if (i < 3) {
			if (*cp != '.')
				return 0;
			cp++;
		}
	}
	if (*cp != '\0')
		return 0;

	inp->s_addr = htonl((parts[0] << 24) | (parts[1] << 16) |
			    (parts[2] << 8) | parts[3]);
	return 1;
}

struct hostent *
gethostbyname(const char *name)
{
	ipaddr_t addr;

	if (!name) {
		ngircd_h_errno = HOST_NOT_FOUND;
		return NULL;
	}

	addr = in_gethostbyname(name);
	if (!addr) {
		ngircd_h_errno = HOST_NOT_FOUND;
		return NULL;
	}

	elks_in_addr.s_addr = addr;
	elks_hostent.h_name = (char *)name;
	elks_hostent.h_aliases = elks_aliases;
	elks_hostent.h_addrtype = AF_INET;
	elks_hostent.h_length = sizeof(elks_in_addr);
	elks_hostent.h_addr_list = elks_addr_list;
	elks_addr_list[0] = (char *)&elks_in_addr;

	return &elks_hostent;
}

struct hostent *
gethostbyaddr(const char *addr, int len, int type)
{
	(void)addr;
	(void)len;
	(void)type;

	ngircd_h_errno = HOST_NOT_FOUND;
	return NULL;
}

int
inet_aton(const char *cp, struct in_addr *inp)
{
	return inet_aton_parse(cp, inp);
}

char *
inet_ntoa(struct in_addr in)
{
	return in_ntoa(in.s_addr);
}

int
gethostname(char *name, size_t len)
{
	const char *fallback = "localhost";
	size_t n;

	if (!name || len == 0)
		return -1;
	n = strlen(fallback);
	if (n >= len)
		n = len - 1;
	memcpy(name, fallback, n);
	name[n] = '\0';
	return 0;
}

int
getsockopt(int sock, int level, int optname, void *optval, socklen_t *optlen)
{
	int *ival;

	(void)sock;
	(void)level;

	if (!optval || !optlen || *optlen < (socklen_t)sizeof(int)) {
		errno = EINVAL;
		return -1;
	}

	ival = (int *)optval;
	if (optname == SO_ERROR) {
		*ival = 0;
		*optlen = (socklen_t)sizeof(int);
		return 0;
	}

	errno = ENOSYS;
	return -1;
}

int
getsocknam(int sock, struct sockaddr *address, socklen_t *address_len, int peer)
{
#if defined(__WATCOMC__)
	syscall_res res;

	res = sys_call4(SYS_getsocknam, (unsigned)sock, (unsigned)address,
		(unsigned)address_len, (unsigned)peer);
	__syscall_return(int, res);
#else
	(void)sock;
	(void)address;
	(void)address_len;
	(void)peer;
	errno = ENOSYS;
	return -1;
#endif
}

int
chroot(const char *path)
{
#if defined(__WATCOMC__)
	syscall_res res;

	res = sys_call1(SYS_chroot, (unsigned)path);
	__syscall_return(int, res);
#else
	(void)path;
	errno = ENOSYS;
	return -1;
#endif
}

int
raise(int sig)
{
	return kill(getpid(), sig);
}

int
pipe(int pipedes[2])
{
#if defined(__WATCOMC__)
	syscall_res res;

	res = sys_call1(SYS_pipe, (unsigned)pipedes);
	__syscall_return(int, res);
#else
	(void)pipedes;
	errno = ENOSYS;
	return -1;
#endif
}

int
sigblock(int mask)
{
	(void)mask;
	return 0;
}

int
sigsetmask(int mask)
{
	(void)mask;
	return 0;
}

#endif /* __ELKS__ */
