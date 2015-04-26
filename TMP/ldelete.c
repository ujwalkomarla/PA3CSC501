/* ldelete.c - ldelete */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * sdelete  --  delete a semaphore by releasing its table entry
 *------------------------------------------------------------------------
 */
SYSCALL ldelete(int lockdescriptor)
{
	STATWORD ps;    
	int	pid;
	struct	lentry	*lptr;
	int lock = lockdescriptor && 63;
	int descript = lockdescriptor>>6;
	disable(ps);
	//CHECK if it is an owner?
	if (isbadlock(lock) || locks[lock].lstate==LFREE || locks[lock].ldesc != descript ) {
		restore(ps);
		return(SYSERR);
	}
	lptr = &locks[lock];
	lptr->lstate = LFREE;
	if (nonempty(lptr->lqhead)) {
		while( (pid=getfirst(lptr->lqhead)) != EMPTY)
		  {
		    proctab[pid].plockwaitret = DELETED;
		    ready(pid,RESCHNO);
		  }
		resched();
	}
	restore(ps);
	return(OK);
}

