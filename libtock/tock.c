#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "tock.h"

typedef struct {
  subscribe_cb *cb;
  int arg0;
  int arg1;
  int arg2;
  void* ud;
} tock_task_t;

#define TASK_QUEUE_SIZE  16
static tock_task_t task_queue[TASK_QUEUE_SIZE];
static int task_cur  = 0;
static int task_last = 0;

int tock_enqueue(subscribe_cb cb, int arg0, int arg1, int arg2, void* ud) {
  int next_task_last = (task_last + 1) % TASK_QUEUE_SIZE;
  if (next_task_last == task_cur) {
    return -1;
  }

  task_queue[task_last].cb   = cb;
  task_queue[task_last].arg0 = arg0;
  task_queue[task_last].arg1 = arg1;
  task_queue[task_last].arg2 = arg2;
  task_queue[task_last].ud   = ud;
  task_last = next_task_last;

  return task_last;
}

int tock_error_to_rcode(tock_error_t err) {
  switch (err) {
  case TOCK_ERROR_FAIL:        return TOCK_FAIL;
  case TOCK_ERROR_BUSY:        return TOCK_EBUSY;
  case TOCK_ERROR_ALREADY:     return TOCK_EALREADY;
  case TOCK_ERROR_OFF:         return TOCK_EOFF;
  case TOCK_ERROR_RESERVE:     return TOCK_ERESERVE;
  case TOCK_ERROR_INVAL:       return TOCK_EINVAL;
  case TOCK_ERROR_SIZE:        return TOCK_ESIZE;
  case TOCK_ERROR_CANCEL:      return TOCK_ECANCEL;
  case TOCK_ERROR_NOMEM:       return TOCK_ENOMEM;
  case TOCK_ERROR_NOSUPPORT:   return TOCK_ENOSUPPORT;
  case TOCK_ERROR_NODEVICE:    return TOCK_ENODEVICE;
  case TOCK_ERROR_UNINSTALLED: return TOCK_EUNINSTALLED;
  case TOCK_ERROR_NOACK:       return TOCK_ENOACK;
  default:                     return TOCK_FAIL;
  }
}

void yield_for(bool *cond) {
  while (!*cond) {
    yield();
  }
}

#if defined(__thumb__)

void yield(void) {
  if (task_cur != task_last) {
    tock_task_t task = task_queue[task_cur];
    task_cur = (task_cur + 1) % TASK_QUEUE_SIZE;
    task.cb(task.arg0, task.arg1, task.arg2, task.ud);
  } else {
    // Note: A process stops yielding when there is a callback ready to run,
    // which the kernel executes by modifying the stack frame pushed by the
    // hardware. The kernel copies the PC value from the stack frame to the LR
    // field, and sets the PC value to callback to run. When this frame is
    // unstacked during the interrupt return, the effectively clobbers the LR
    // register.
    //
    // At this point, the callback function is now executing, which may itself
    // clobber any of the other caller-saved registers. Thus we mark this
    // inline assembly as conservatively clobbering all caller-saved registers,
    // forcing yield to save any live registers.
    //
    // Upon direct observation of this function, the LR is the only register
    // that is live across the SVC invocation, however, if the yield call is
    // inlined, it is possible that the LR won't be live at all (commonly seen
    // for the `while (1) { yield(); }` idiom) or that other registers are
    // live, thus it is important to let the compiler do the work here.
    //
    // According to the AAPCS: A subroutine must preserve the contents of the
    // registers r4-r8, r10, r11 and SP (and r9 in PCS variants that designate
    // r9 as v6) As our compilation flags mark r9 as the PIC base register, it
    // does not need to be saved. Thus we must clobber r0-3, r12, and LR
    asm volatile (
      "svc 0       \n"
      :
      :
      : "memory", "r0", "r1", "r2", "r3", "r12", "lr"
      );
  }
}

int subscribe(uint32_t driver, uint32_t subscribe,
              subscribe_cb cb, void* userdata) {
  register uint32_t r0 asm ("r0") = driver;
  register uint32_t r1 asm ("r1") = subscribe;
  register void*    r2 asm ("r2") = cb;
  register void*    r3 asm ("r3") = userdata;
  register int ret asm ("r0");
  asm volatile (
    "svc 1"
    : "=r" (ret)
    : "r" (r0), "r" (r1), "r" (r2), "r" (r3)
    : "memory");
  return ret;
}

subscribe_return_t subscribe2(uint32_t driver, uint32_t subscribe,
              subscribe_cb cb, void* userdata) {
  register uint32_t r0 asm ("r0") = driver;
  register uint32_t r1 asm ("r1") = subscribe;
  register void*    r2 asm ("r2") = cb;
  register void*    r3 asm ("r3") = userdata;
  register int rtype asm ("r0");
  register int rv1 asm ("r1");
  register int rv2 asm ("r2");
  register int rv3 asm ("r3");
  asm volatile (
    "svc 1"
    : "=r" (rtype), "=r" (rv1), "=r" (rv2), "=r" (rv3)
    : "r" (r0), "r" (r1), "r" (r2), "r" (r3)
    : "memory");

  if (rtype == TOCK_SYSCALL_SUCCESS_U32_U32) {
    subscribe_return_t rval = {true, (subscribe_cb*)rv1, (void*)rv2, 0};
    return rval;
  } else if (rtype == TOCK_SYSCALL_FAILURE_U32_U32) {
    subscribe_return_t rval = {false, (subscribe_cb*)rv2, (void*)rv3, (tock_error_t)rv1};
    return rval;
  } else {
    exit(-1);
  }
}



int command(uint32_t driver, uint32_t command, int data, int arg2) {
  register uint32_t r0 asm ("r0") = driver;
  register uint32_t r1 asm ("r1") = command;
  register uint32_t r2 asm ("r2") = data;
  register uint32_t r3 asm ("r3") = arg2;
  register int ret asm ("r0");
  asm volatile (
    "svc 2"
    : "=r" (ret)
    : "r" (r0), "r" (r1), "r" (r2), "r" (r3)
    : "memory"
    );
  return ret;
}

syscall_return_t command2(uint32_t driver, uint32_t command, int data, int arg2) {
  register uint32_t r0 asm ("r0") = driver;
  register uint32_t r1 asm ("r1") = command;
  register uint32_t r2 asm ("r2") = data;
  register uint32_t r3 asm ("r3") = arg2;
  register uint32_t rcode asm ("r0");
  register uint32_t rv1 asm ("r1");
  register uint32_t rv2 asm ("r2");
  register uint32_t rv3 asm ("r3");
  asm volatile (
    "svc 2"
    : "=r" (rcode), "=r" (rv1), "=r" (rv2), "=r" (rv3)
    : "r" (r0), "r" (r1), "r" (r2), "r" (r3)
    : "memory"
    );
  syscall_return_t rval = {rcode, {rv1, rv2, rv3}};
  return rval;
}


int allow(uint32_t driver, uint32_t allow, void* ptr, size_t size) {
  register uint32_t r0 asm ("r0") = driver;
  register uint32_t r1 asm ("r1") = allow;
  register void*    r2 asm ("r2") = ptr;
  register size_t r3 asm ("r3")   = size;
  register int ret asm ("r0");
  asm volatile (
    "svc 3"
    : "=r" (ret)
    : "r" (r0), "r" (r1), "r" (r2), "r" (r3)
    : "memory"
    );
  return ret;
}

allow_ro_return_t allow_readonly(uint32_t driver, uint32_t allow, const void* ptr, size_t size) {
  register uint32_t r0 asm ("r0") = driver;
  register uint32_t r1 asm ("r1") = allow;
  register const void*    r2 asm ("r2") = ptr;
  register size_t r3 asm ("r3")   = size;
  register int rtype asm ("r0");
  register int rv1 asm ("r1");
  register int rv2 asm ("r2");
  register int rv3 asm ("r3");
  asm volatile (
    "svc 4"
    : "=r" (rtype), "=r" (rv1), "=r" (rv2), "=r" (rv3)
    : "r" (r0), "r" (r1), "r" (r2), "r" (r3)
    : "memory"
    );
  if (rtype == TOCK_SYSCALL_SUCCESS_U32_U32) {
    allow_ro_return_t rv = {true, (const void*)rv1, (size_t)rv2, 0};
    return rv;
  } else if (rtype == TOCK_SYSCALL_FAILURE_U32_U32) {
    allow_ro_return_t rv = {false, (const void*)rv2, (size_t)rv3, (tock_error_t)rv1};
    return rv;
  } else {
    // Invalid return type
    exit(-1);
  }
}
allow_rw_return_t allow_readwrite(uint32_t driver, uint32_t allow, void* ptr, size_t size) {
  register uint32_t r0 asm ("r0") = driver;
  register uint32_t r1 asm ("r1") = allow;
  register const void*    r2 asm ("r2") = ptr;
  register size_t r3 asm ("r3")   = size;
  register int rtype asm ("r0");
  register int rv1 asm ("r1");
  register int rv2 asm ("r2");
  register int rv3 asm ("r3");
  asm volatile (
    "svc 3"
    : "=r" (rtype), "=r" (rv1), "=r" (rv2), "=r" (rv3)
    : "r" (r0), "r" (r1), "r" (r2), "r" (r3)
    : "memory"
    );
  if (rtype == TOCK_SYSCALL_SUCCESS_U32_U32) {
    allow_rw_return_t rv = {true, (void*)rv1, (size_t)rv2, 0};
    return rv;
  } else if (rtype == TOCK_SYSCALL_FAILURE_U32_U32) {
    allow_rw_return_t rv = {false, (void*)rv2, (size_t)rv3, (tock_error_t)rv1};
    return rv;
  } else {
    // Invalid return type
    exit(1);
  }
}

void* memop(uint32_t op_type, int arg1) {
  register uint32_t r0 asm ("r0") = op_type;
  register int r1 asm ("r1")      = arg1;
  register void*   ret asm ("r0");
  asm volatile (
    "svc 5"
    : "=r" (ret)
    : "r" (r0), "r" (r1)
    : "memory"
    );
  return ret;
}

#elif defined(__riscv)

// Implementation of the syscalls for generic RISC-V platforms.
//
// For RISC-V, the arguments are passed through registers a0-a4. Generally,
// the syscall number is put in a0, and the required arguments are specified in
// a1-a4. Nothing specifically syscall related is pushed to the process stack.

void yield(void) {
  if (task_cur != task_last) {
    tock_task_t task = task_queue[task_cur];
    task_cur = (task_cur + 1) % TASK_QUEUE_SIZE;
    task.cb(task.arg0, task.arg1, task.arg2, task.ud);
  } else {
    asm volatile (
      "li    a0, 0\n"
      "ecall\n"
      :
      :
      : "memory", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
      "t0", "t1", "t2", "t3", "t4", "t5", "t6", "ra"
      );

  }
}

int subscribe(uint32_t driver, uint32_t subscribe,
              subscribe_cb cb, void* userdata) {
  register uint32_t a1  asm ("a1") = driver;
  register uint32_t a2  asm ("a2") = subscribe;
  register void*    a3  asm ("a3") = cb;
  register void*    a4  asm ("a4") = userdata;
  register int ret asm ("a0");
  asm volatile (
    "li    a0, 1\n"
    "ecall\n"
    : "=r" (ret)
    : "r" (a1), "r" (a2), "r" (a3), "r" (a4)
    : "memory");
  return ret;
}


int command(uint32_t driver, uint32_t command, int data, int arg2) {
  register uint32_t a1  asm ("a1") = driver;
  register uint32_t a2  asm ("a2") = command;
  register uint32_t a3  asm ("a3") = data;
  register uint32_t a4  asm ("a4") = arg2;
  register int ret asm ("a0");
  asm volatile (
    "li    a0, 2\n"
    "ecall\n"
    : "=r" (ret)
    : "r" (a1), "r" (a2), "r" (a3), "r" (a4)
    : "memory");
  return ret;
}

int allow(uint32_t driver, uint32_t allow, void* ptr, size_t size) {
  register uint32_t a1  asm ("a1") = driver;
  register uint32_t a2  asm ("a2") = allow;
  register void*    a3  asm ("a3") = ptr;
  register size_t a4  asm ("a4")   = size;
  register int ret asm ("a0");
  asm volatile (
    "li    a0, 3\n"
    "ecall\n"
    : "=r" (ret)
    : "r" (a1), "r" (a2), "r" (a3), "r" (a4)
    : "memory");
  return ret;
}

void* memop(uint32_t op_type, int arg1) {
  register uint32_t a1  asm ("a1") = op_type;
  register uint32_t a2  asm ("a2") = arg1;
  register void*    ret asm ("a0");
  asm volatile (
    "li    a0, 4\n"
    "ecall\n"
    : "=r" (ret)
    : "r" (a1), "r" (a2)
    : "memory");
  return ret;
}

#endif

void* tock_app_memory_begins_at(void) {
  return memop(2, 0);
}

void* tock_app_memory_ends_at(void) {
  return memop(3, 0);
}

void* tock_app_flash_begins_at(void) {
  return memop(4, 0);
}

void* tock_app_flash_ends_at(void) {
  return memop(5, 0);
}

void* tock_app_grant_begins_at(void) {
  return memop(6, 0);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbad-function-cast"
int tock_app_number_writeable_flash_regions(void) {
  return (int) memop(7, 0);
}
#pragma GCC diagnostic pop

void* tock_app_writeable_flash_region_begins_at(int region_index) {
  return memop(8, region_index);
}

void* tock_app_writeable_flash_region_ends_at(int region_index) {
  return memop(9, region_index);
}

bool driver_exists(uint32_t driver) {
  int ret = command(driver, 0, 0, 0);
  return ret >= 0;
}

const char* tock_strerr(tock_error_t err) {
  return tock_strrcode(tock_error_to_rcode(err));
}

const char* tock_strrcode(int tock_rcode) {
  switch (tock_rcode) {
    case TOCK_SUCCESS:
      return "Success";
    case TOCK_FAIL:
      return "Unknown Error";
    case TOCK_EBUSY:
      return "Underlying system is busy; retry";
    case TOCK_EALREADY:
      return "The state requested is already set";
    case TOCK_EOFF:
      return "The component is powered down";
    case TOCK_ERESERVE:
      return "Reservation required before use";
    case TOCK_EINVAL:
      return "An invalid parameter was passed";
    case TOCK_ESIZE:
      return "Parameter passed was too large";
    case TOCK_ECANCEL:
      return "Operation cancelled by a call";
    case TOCK_ENOMEM:
      return "Memory required not available";
    case TOCK_ENOSUPPORT:
      return "Operation or command is unsupported";
    case TOCK_ENODEVICE:
      return "Device does not exist";
    case TOCK_EUNINSTALLED:
      return "Device is not physically installed";
    case TOCK_ENOACK:
      return "Packet transmission not acknowledged";
  }
  return "Invalid error number";
}

void tock_expect(int expected, int actual, const char* file, unsigned line) {
  if (expected != actual) {
    printf("Expectation failure in \"%s\" at line %u\n", file, line);
    printf("Expected value: %d\n", expected);
    printf(" But got value: %d (possible error: %s)\n", actual, tock_strrcode(actual));
    exit(-1);
  }
}
