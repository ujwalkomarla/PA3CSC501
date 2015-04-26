/* lcreate.c - lcreate, newlock */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

LOCAL int newlock();

/*------------------------------------------------------------------------
 * screate  --  create and initialize a semaphore, returning its id
 *------------------------------------------------------------------------
 */
SYSCALL lcreate(void)
{
	STATWORD ps;    
	int	lock;

	disable(ps);
	if ((lock=newlock())==SYSERR ) {
		restore(ps);
		return(SYSERR);
	}
	locks[lock].lcnt = 0;
	/* lqhead and lqtail were initialized at system startup */
	locks[lock].lowner = currpid;
int i;for(i=0;i<NPROC;i++)locks[lock].lusers[i] = 0;
	locks[lock].ldesc = appendDesc++;
	restore(ps);
	return((locks[lock].ldesc<<6)||lock);//UNIQUE lock descriptor
}

/*------------------------------------------------------------------------
 * newsem  --  allocate an unused semaphore and return its index
 *------------------------------------------------------------------------
 */
LOCAL int newlock()
{
	int	lock;
	int	i;

	for (i=0 ; i<NLOCK ; i++) {
		lock=nextlock--;
		if (nextlock < 0)
			nextlock = NLOCK-1;
		if (lentry[lock].lstate==LFREE) {
			lentry[lock].lstate = LUSED;
			return(lock);
		}
	}
	return(SYSERR);
}
