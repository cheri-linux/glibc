#ifndef _SYS_IOCTL_H
#include <misc/sys/ioctl.h>

# ifndef _ISOMAC

/* Now define the internal interfaces.  */
#ifndef __CHERI_PURE_CAPABILITY__
extern int __ioctl (int __fd, unsigned long int __request, ...);
#else
extern int __ioctl (int __fd, unsigned long int __request,
		    void *__argp);

#define ___ioctl_cheri0(fd,request) (__ioctl) (fd, request, 0)
#define ___ioctl_cheri1(fd,request,argp) (__ioctl) (fd, request, argp)

#define __ioctl(...) __IOCTL_DISP (___ioctl_cheri, __VA_ARGS__)
#endif

libc_hidden_proto (__ioctl)

# endif /* !_ISOMAC */
#endif
