#ifndef _SYS_MMAN_H
#include <misc/sys/mman.h>

#ifndef _ISOMAC
/* Now define the internal interfaces.  */
extern void *__mmap (void *__addr, size_t __len, int __prot,
		     int __flags, int __fd, __off_t __offset);
libc_hidden_proto (__mmap)
extern void *__mmap64 (void *__addr, size_t __len, int __prot,
		       int __flags, int __fd, __off64_t __offset);
libc_hidden_proto (__mmap64)
extern int __munmap (void *__addr, size_t __len);
libc_hidden_proto (__munmap)
extern int __mprotect (void *__addr, size_t __len, int __prot);
libc_hidden_proto (__mprotect)

extern int __madvise (void *__addr, size_t __len, int __advice);
libc_hidden_proto (__madvise)

/* This one is Linux specific.  */
#ifndef __CHERI_PURE_CAPABILITY__
extern void *__mremap (void *__addr, size_t __old_len,
		       size_t __new_len, int __flags, ...);
#else
extern void *__mremap (void *__addr, size_t __old_len, size_t __new_len,
		       int __flags, void *__new_address);

#define ___mremap_cheri0(a,b,c,d) __cheri_long((__mremap) (a, b, c, d, 0), c)
#define ___mremap_cheri1(a,b,c,d,e) __cheri_long((__mremap) (a, b, c, d, e), c)

#define __mremap(...) __MREMAP_DISP (___mremap_cheri, __VA_ARGS__)
#endif
libc_hidden_proto (__mremap)

# if IS_IN (rtld)
#  include <dl-mman.h>
# endif
#endif

#endif
