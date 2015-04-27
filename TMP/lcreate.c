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
	int	tlock;

	disable(ps);
	if ((tlock=newlock())==SYSERR ) {
		restore(ps);
		return(SYSERR);
	}
	locks[tlock].lcnt = 0;
	/* lqhead and lqtail were initialized at system startup */
	locks[tlock].lowner = currpid;
int i;for(i=0;i<NPROC;i++)locks[tlock].lusers[i] = 0;
	locks[tlock].ldesc = appendDesc;
appendDesc++;
	restore(ps);
#ifdef DEBUG1
kprintf("lock descriptor %x\r\n",(locks[tlock].ldesc<<6)|tlock);
#endif
	return((locks[tlock].ldesc<<6)|tlock);//UNIQUE lock descriptor
}

/*------------------------------------------------------------------------
 * newsem  --  allocate an unused semaphore and return its index
 *------------------------------------------------------------------------
 */
LOCAL int newlock()
{
	int	tlock;
	int	i;

	for (i=0 ; i<NLOCKS ; i++) {
		tlock=nextlock--;
		if (nextlock < 0)
			nextlock = NLOCKS-1;
		if (locks[tlock].lstate==LFREE) {
			locks[tlock].lstate = LUSED;
			return(tlock);
		}
	}
	return(SYSERR);
}

