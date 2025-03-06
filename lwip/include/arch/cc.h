#ifndef __ARCH_SYS_ARCH_H__
#define __ARCH_SYS_ARCH_H__

#define SYS_MBOX_NULL   NULL
#define SYS_SEM_NULL    NULL

// `newlib` also uses a macro named BYTE_ORDER, but very differently
// so we need to remove its definition first if it happened to be in use.
#ifdef BYTE_ORDER
#undef BYTE_ORDER
#endif
#define BYTE_ORDER LITTLE_ENDIAN

typedef void * sys_prot_t;

typedef void * sys_thread_t;

#endif /* __ARCH_SYS_ARCH_H__ */
