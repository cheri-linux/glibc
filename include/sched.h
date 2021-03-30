#ifndef _SCHED_H
#include <posix/sched.h>

#ifndef _ISOMAC
/* Now define the internal interfaces.  */
extern int __sched_setparam (__pid_t __pid,
			     const struct sched_param *__param);
libc_hidden_proto (__sched_setparam)
extern int __sched_getparam (__pid_t __pid, struct sched_param *__param);
extern int __sched_setscheduler (__pid_t __pid, int __policy,
				 const struct sched_param *__param);
libc_hidden_proto (__sched_setscheduler)
extern int __sched_getscheduler (__pid_t __pid);
extern int __sched_yield (void);
libc_hidden_proto (__sched_yield)
extern int __sched_get_priority_max (int __algorithm);
libc_hidden_proto (__sched_get_priority_max)
extern int __sched_get_priority_min (int __algorithm);
libc_hidden_proto (__sched_get_priority_min)
extern int __sched_rr_get_interval (__pid_t __pid, struct timespec *__t);

/* These are Linux specific.  */
#ifndef __CHERI_PURE_CAPABILITY__
extern int __clone (int (*__fn) (void *__arg), void *__child_stack,
		    int __flags, void *__arg, ...);
#else
extern int __clone (int (*__fn) (void *__arg), void *__child_stack,
		    int __flags, void *__arg, void *__parent_tidptr,
		    void *__tls, void *__child_tidptr);

#define __CLONE_NARGS_X(a,b,c,d,e,f,g,h,...) h
#define __CLONE_NARGS(...) __CLONE_NARGS_X (__VA_ARGS__,3,2,1,0,)
#define __CLONE_CONCAT_X(a,b) a##b
#define __CLONE_CONCAT(a,b) __CLONE_CONCAT_X (a, b)
#define __CLONE_DISP(b,...) __CLONE_CONCAT (b,__CLONE_NARGS(__VA_ARGS__))(__VA_ARGS__)

#define __clone_cheri0(a,b,c,d) (__clone) (a, b, c, d, 0, 0, 0)
#define __clone_cheri1(a,b,c,d,e) (__clone) (a, b, c, d, e, 0, 0)
#define __clone_cheri2(a,b,c,d,e,f) (__clone) (a, b, c, d, e, f, 0)
#define __clone_cheri3(a,b,c,d,e,f,g) (__clone) (a, b, c, d, e, f, g)

#define __clone(...) __CLONE_DISP (__clone_cheri, __VA_ARGS__)
#endif
libc_hidden_proto (__clone)
extern int __clone2 (int (*__fn) (void *__arg), void *__child_stack_base,
		     size_t __child_stack_size, int __flags, void *__arg, ...);
libc_hidden_proto (__clone2)
#endif
#endif
