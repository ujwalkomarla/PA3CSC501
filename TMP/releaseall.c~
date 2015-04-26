/* releaseall.c - release */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>



//#define	isempty(list)	(q[(list)].qnext >= NPROC)
//#define	nonempty(list)	(q[(list)].qnext < NPROC)
#define	getlkey(list)	(q[q[(list)].qnext].qkey)//key is nothing but the priority
#define getltype(list)	(q[q[(list)].qnext].ltype)
#define getltime(list)	(q[q[(list)].qnext].ltime)
//#define lastkey(tail)	(q[q[(tail)].qprev].qkey)
//#define firstid(list)	(q[(list)].qnext)

/*------------------------------------------------------------------------
 * releaseall  --  release 'numlocks' number of locks, and corresponding lock's one waiting process
 *------------------------------------------------------------------------
 */
SYSCALL releaseall(int numlocks,int args);

{
	STATWORD ps;    
	register struct	lentry	*lptr;

	disable(ps);
	int passedPar,lock,descript;
	int retVal = OK;
	unsigned long	*a;		/* points to list of args	*/
	a = (unsigned long *)(&args) + (numlocks-1);
	for(;numlocks>0;numlocks--){
		passedPar = *a--;
		descript = passedPar >> 6;
		lock = passedPar & 63;
		if (isbadlock(lock) || (lptr= &locks[lock])->lstate==LFREE || lptr->ldesc != descrpit) {
			retVal = SYSERR;
		}else{
			if(lptr->lusers[currpid]!=1){
				retVal = SYSERR;
			}else{			
				lptr->lusers[currpid]=0;
				lptr->lcnt--;
if(lptr->lcnt==0){ 
// schedule next on queue
// if type=read, release all other process waiting with type = read and priority greater than any other waiting process with type = write.


					ready(getfirst(lptr->lqhead), RESCHNO);
}

			}
		}
	}
	resched();
	restore(ps);
	return(retVal);
}

