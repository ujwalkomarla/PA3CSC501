/* lock.c - lock */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * lock  --  make current process get a lock
 *------------------------------------------------------------------------
 */
SYSCALL	lock(int ldes1, int type, int priority)
{
	STATWORD ps;    
	struct	lentry	*sptr;
	struct	pentry	*pptr;
	int lock = ldes1 & 63;
	int descript = ldes1 >>6;
	disable(ps);
	if (isbadlock(lock) || (lptr= &locks[lock])->lstate==LFREE) || lptr->ldesc != descript){
		restore(ps);
		return(SYSERR);
	}

	if(lptr->lcnt == 0){//If no one is holding the lock.
		lptr->lcnt = 1;
		lptr->ltype = type;
		lptr->lusers[currpid] = 1;
		restore(ps);
		return(OK);
	}else{
		if(type == LREAD && lptr->ltype == LREAD){
			if(priority >= firstkey(lptr->lqhead)){//if priority >= waiting WRITE process priority
				lptr->lcnt++;
				lptr->lusers[currpid]=1;
				restore(ps);
				return(OK);
			}
		}
	

	//Requested process wont't get the lock
	//Add to queue
		(pptr = &proctab[currpid])->pstate = PRWAIT;
		pptr->plock = lock;
		//enqueue(currpid,lptr->lqtail);INSERT
		insertlock(currpid,lptr->lqtail,priority,type,ctr1000);
		pptr->plockwaitret = OK;
		resched();
		restore(ps);


	//After returning by gaining the lock
		lptr->lcnt++;
		lptr->ltype = type;
		lptr->lusers[currpid]=1;
		return pptr->plockwaitret;
	}
}
