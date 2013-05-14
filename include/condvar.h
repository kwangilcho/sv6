#pragma once

#ifdef __cplusplus
#include "cpputil.hh"           // For NEW_DELETE_OPS
#include "proc.hh"
#include "ilist.hh"
#endif

struct condvar {
  struct spinlock lock;
  ilist<proc,&proc::cv_waiters> waiters;

#ifdef __cplusplus
  // Construct an uninitialized condvar.  This should be move-assigned
  // from an initialized condvar before being used.  This is
  // constexpr, so it can be used for global condvars without
  // incurring a static constructor.
  condvar()
    : lock() { }

  condvar(const char *name)
    : lock(name, LOCKSTAT_CONDVAR) { }

  // Condvars cannot be copied.
  condvar(const condvar &o) = delete;
  condvar &operator=(const condvar &o) = delete;

  // Condvars can be moved.
  condvar(condvar &&o) = default;
  condvar &operator=(condvar &&o) = default;

  NEW_DELETE_OPS(condvar);

  void sleep(struct spinlock *);
  void sleep_to(struct spinlock*, u64);
  void wake_all(int yield=false, proc *callerproc=nullptr);
  void wake_one(proc *p);
#endif // __cplusplus
};

void            timerintr(void);
u64             nsectime(void);
