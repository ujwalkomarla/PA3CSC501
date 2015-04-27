/* lock.c - lock */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>
extern unsigned long	ctr1000;
/*------------------------------------------------------------------------
 * lock  --  make current process get a lock
 *------------------------------------------------------------------------
 */
SYSCALL	lock(int ldes1, int type, int priority)
{
	STATWORD ps;    
	struct	lentry	*lptr;
	struct	pentry	*pptr;
	int tlock = ldes1 & 63;
	int descript = ldes1 >>6;
	disable(ps);
	if(isbadlock(tlock) || (lptr= &locks[tlock])->lstate==LFREE || lptr->ldesc != descript){
/*#ifdef DEBUG
kprintf("%d lock descriptor %x ? %x\r\n",tlock,lptr->ldesc, descript );
#endif*/
		restore(ps);
		return(SYSERR);
	}
#ifdef DEBUG1
kprintf("acquire lock type = %c\r\n",type==READ?'R':'W');
#endif
	if(lptr->lcnt == 0){//If no one is holding the lock.
		lptr->lcnt = 1;
		lptr->ltype = type;
		lptr->lusers[currpid] = 1;
/*#ifdef DEBUG
kprintf("acquired lptr->lcnt = %d\r\n",lptr->lcnt);
#endif*/
		restore(ps);
		return(OK);
	}else{
		if(type == READ && lptr->ltype == READ){
			if(priority >= lastkey(lptr->lqtail)){//if priority >= waiting WRITE process priority
#ifdef DEBUG1
kprintf("firstkey is %d, asking is %d\r\n",lastkey(lptr->lqtail),priority);
#endif
				lptr->lcnt++;
				lptr->lusers[currpid]=1;
				restore(ps);
				return(OK);
			}
		}
	

	//Requested process wont't get the lock
	//Add to queue
		(pptr = &proctab[currpid])->pstate = PLWAIT;
		pptr->plock = tlock;
		//enqueue(currpid,lptr->lqtail);INSERT
//		insertlock(currpid,lptr->lqhead,priority,type,ctr1000);
insert(currpid,lptr->lqhead,priority);
q[currpid].ltype = type;
q[currpid].ltime = ctr1000;
//pptr->plwaittype = type;
//pptr->plwaittime = ctr1000;
		pptr->plockwaitret = OK;
		resched();
#ifdef DEBUG1
kprintf("acquired lptr->ltype = %c\r\n",(lptr->ltype==READ)?'R':'W');
#endif
		restore(ps);


	//After returning by gaining the lock
		if(pptr->plockwaitret != DELETED){		
				lptr->lcnt++;
				lptr->ltype = type;
				lptr->lusers[currpid]=1;
		}
kprintf("Lock Return Value %d",pptr->plockwaitret);
		return pptr->plockwaitret;
	}
}
