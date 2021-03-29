#ifndef _SYS_PRCTL_H
#include_next <sys/prctl.h>

# ifndef _ISOMAC

#ifndef __CHERI_PURE_CAPABILITY__
extern int __prctl (int __option, ...);
#else
extern int __prctl (int __option, uintptr_t __arg2, uintptr_t __arg3,
		    uintptr_t __arg4, uintptr_t __arg5);

#define ___prctl_cheri0(option) (__prctl) (option, 0, 0, 0, 0)
#define ___prctl_cheri1(option,a) (__prctl) (option, __prc(a), 0, 0, 0)
#define ___prctl_cheri2(option,a,b) (__prctl) (option, __prc(a), __prc(b), 0, 0)
#define ___prctl_cheri3(option,a,b,c) (__prctl) (option, __prc(a), __prc(b), __prc(c), 0)
#define ___prctl_cheri4(option,a,b,c,d) (__prctl) (option, __prc(a), __prc(b), __prc(c), __prc(d))

#define __prctl(...) __PRCTL_DISP (___prctl_cheri, __VA_ARGS__)
#endif

# endif /* !_ISOMAC */
#endif
