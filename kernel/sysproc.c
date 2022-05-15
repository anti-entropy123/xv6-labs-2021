#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

pte_t *walk(pagetable_t pagetable, uint64 va, int alloc);

uint64
sys_exit(void)
{
  int n;
  if (argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0; // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if (argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;

  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (myproc()->killed)
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// #ifdef LAB_PGTBL
int sys_pgaccess(void)
{
  struct proc *p = myproc();
  // vmprint(p->pagetable);
  uint64 va;
  int page_num;
  argaddr(0, &va);
  argint(1, &page_num);
  // printf("va=%p\n", va);
  int result = 0;
  for (int i = 0; i < page_num; i++)
  {
    pte_t *pte = walk(p->pagetable, va, 0);
    va += PGSIZE;
    if (pte == 0 || (*pte & PTE_A) == 0)
    {
      continue;
    }
    // printf("access %d: %p\n", i, (*pte));
    result += 1 << i;
    *pte &= (~PTE_A);
  }
  // printf("result=%p\n", result);
  uint64 uva;
  argaddr(2, &uva);
  copyout(p->pagetable, uva, (char *)(&result), sizeof(result));
  return 0;
}
// #endif

uint64
sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
