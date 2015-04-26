/* releaseall.c - release */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>



//#define	isempty(list)	(q[(list)].qnext >= NPROC)
//#define	nonempty(list)	(q[(list)].qnext < NPROC)
#define	getlkey(list)	(q[q[(list)].qnext].qkey)//key is nothing but the priority. similar to firstkey
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
					if(nonempty(lptr->lqhead)){
//without the one second condition
						tLtype = getltype(lptr->lqhead);
						tProc = getfirst(lptr->lqhead);
						ready(tProc,RESCHNO);
						while(tLtype != LWRITE && nonempty(lptr->lqhead)){
							tProc = getfirst(lptr->lqhead);
//SHOULD CHECK FOR SYSERR right? or do a nonempty
//SETTING lock entry
							ready(tProc,RESCHNO);
							tLtype = getltype(lptr->lqhead);
						}
						







						/*int tWaitPrio = getlkey(lptr->lqhead);
						int tWaitTime = getltime(lptr->lqhead);
						int tLtype = getltype(lptr->lqhead);
						int tProc = getfirst(lptr->lqhead);
						if(tLtype != LWRITE){
							while(firstwaitprio == getlkey(lptr->lqhead))
								if(firstwaittime < getltime(lptr->lqhead)+1000){
									if(getltype(lptr->lqhead)==LWRITE){
										insert2(tProc,lptr->lqtail,tWaittPrio,tLtype,tWaitTime);
										tProc = getfirst(lptr->lqhead);tLytpe=LWRITE;ready(tProc, RESCHNO);
										break;
									}
								}
							}
						}else{//WRITE
							ready(tProc, RESCHNO);
						}*/

					}				
				}
			}
		}
	}
	resched();
	restore(ps);
	return(retVal);
}





LOCAL int yourLuck(int lock){
	register struct	lentry	*tptr;
int tWaitPrio = getlkey(lptr->lqhead);
int tWaitTime = getltime(lptr->lqhead);
int tLtype = getltype(lptr->lqhead);
int tProc = getfirst(lptr->lqhead);


}
