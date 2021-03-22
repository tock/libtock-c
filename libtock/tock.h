#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (subscribe_upcall)(int, int, int, void*);

typedef enum {
  TOCK_SYSCALL_FAILURE             =   0,
  TOCK_SYSCALL_FAILURE_U32         =   1,
  TOCK_SYSCALL_FAILURE_U32_U32     =   2,
  TOCK_SYSCALL_FAILURE_U64         =   3,
  TOCK_SYSCALL_SUCCESS             = 128,
  TOCK_SYSCALL_SUCCESS_U32         = 129,
  TOCK_SYSCALL_SUCCESS_U32_U32     = 130,
  TOCK_SYSCALL_SUCCESS_U64         = 131,
  TOCK_SYSCALL_SUCCESS_U32_U32_U32 = 132,
  TOCK_SYSCALL_SUCCESS_U64_U32     = 133
} syscall_rtype_t;

// System call return values, used by C userspace: 0 is success and
// negative values are failures.
#define TOCK_SUCCESS       0
#define TOCK_FAIL         -1
#define TOCK_EBUSY        -2
#define TOCK_EALREADY     -3
#define TOCK_EOFF         -4
#define TOCK_ERESERVE     -5
#define TOCK_EINVAL       -6
#define TOCK_ESIZE        -7
#define TOCK_ECANCEL      -8
#define TOCK_ENOMEM       -9
#define TOCK_ENOSUPPORT   -10
#define TOCK_ENODEVICE    -11
#define TOCK_EUNINSTALLED -12
#define TOCK_ENOACK       -13
#define TOCK_EBADRVAL     -1024

// Error codes from the kernel (kernel::ErrorCode). libtock-c transforms
// these into system call return values. Userspace C code should not see
// these values.
typedef enum {
  TOCK_ERROR_FAIL        = 1,
  TOCK_ERROR_BUSY        = 2,
  TOCK_ERROR_ALREADY     = 3,
  TOCK_ERROR_OFF         = 4,
  TOCK_ERROR_RESERVE     = 5,
  TOCK_ERROR_INVAL       = 6,
  TOCK_ERROR_SIZE        = 7,
  TOCK_ERROR_CANCEL      = 8,
  TOCK_ERROR_NOMEM       = 9,
  TOCK_ERROR_NOSUPPORT   = 10,
  TOCK_ERROR_NODEVICE    = 11,
  TOCK_ERROR_UNINSTALLED = 12,
  TOCK_ERROR_NOACK       = 13,
} tock_error_t;
  
typedef struct {
  syscall_rtype_t type;
  uint32_t data[3];
} syscall_return_t;

typedef struct {
  bool success;
  subscribe_upcall* callback;
  void* userdata;
  tock_error_t error;
} subscribe_return_t;

typedef struct {
  bool success;
  void* ptr;
  size_t size;
  tock_error_t error;
} allow_rw_return_t;

typedef struct {
  bool success;
  const void* ptr;
  size_t size;
  tock_error_t error;
} allow_ro_return_t;

int tock_error_to_rcode(tock_error_t);

int tock_enqueue(subscribe_upcall cb, int arg0, int arg1, int arg2, void* ud);

void yield(void);
void yield_for(bool*);
int yield_no_wait(void);
 
void tock_exit(uint32_t completion_code) __attribute__ ((noreturn));
void tock_restart(uint32_t completion_code) __attribute__ ((noreturn));

__attribute__ ((warn_unused_result))
syscall_return_t command(uint32_t driver, uint32_t command,
			  int arg1, int arg2);
  
// Pass this to the subscribe syscall as a function pointer to
// be the Null Upcall.
#define TOCK_NULL_UPCALL 0
  
__attribute__ ((warn_unused_result))
subscribe_return_t subscribe(uint32_t driver, uint32_t subscribe,
			      subscribe_upcall uc, void* userdata);

__attribute__ ((warn_unused_result))
allow_rw_return_t allow_readwrite(uint32_t driver, uint32_t allow,
				  void* ptr, size_t size);

__attribute__ ((warn_unused_result))
allow_ro_return_t allow_readonly(uint32_t driver, uint32_t allow,
				 const void* ptr, size_t size);

// op_type can be:
// 0: brk, arg1 is pointer to new memory break
// 1: sbrk, arg1 is increment to increase/decrease memory break
void* memop(uint32_t op_type, int arg1);

// Wrappers around memop to support app introspection
void* tock_app_memory_begins_at(void);
void* tock_app_memory_ends_at(void);
void* tock_app_flash_begins_at(void);
void* tock_app_flash_ends_at(void);
void* tock_app_grant_begins_at(void);
int tock_app_number_writeable_flash_regions(void);
void* tock_app_writeable_flash_region_begins_at(int region_index);
void* tock_app_writeable_flash_region_ends_at(int region_index);


// Checks to see if the given driver number exists on this platform.
bool driver_exists(uint32_t driver);


const char* tock_strerr(tock_error_t tock_errno);
const char* tock_strrcode(int tock_rcode);
 
void tock_expect(int expected, int actual, const char* file, unsigned line);
#define TOCK_EXPECT(_e, _a) tock_expect((_e), (_a), __FILE__, __LINE__)

#ifdef __cplusplus
}
#endif
