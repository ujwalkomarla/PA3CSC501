/* releaseall.c - release */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>



//#define	isempty(list)	(q[(list)].qnext >= NPROC)
//#define	nonempty(list)	(q[(list)].qnext < NPROC)
//#define	getlkey(list)	(q[q[(list)].qnext].qkey)//key is nothing but the priority. similar to firstkey
//#define getltype(list)	(q[q[(list)].qnext].ltype)
//#define getltime(list)	(q[q[(list)].qnext].ltime)
//#define lastkey(tail)	(q[q[(tail)].qprev].qkey)
//#define firstid(list)	(q[(list)].qnext)

/*------------------------------------------------------------------------
 * releaseall  --  release 'numlocks' number of locks, and corresponding lock's one waiting process
 *------------------------------------------------------------------------
 */
SYSCALL releaseall(int numlocks,int args, ...)
{
	STATWORD ps;    
	register struct	lentry	*lptr;

	disable(ps);
	int passedPar,tlock,descript;


	int tprev,ttime,tprio,ttype,toRun;
	int retVal = OK;
	unsigned long	*a;		/* points to list of args	*/
	a = (unsigned long *)(&args) + (numlocks-1);
	for(;numlocks>0;numlocks--){
		passedPar = *a--;
		descript = passedPar >> 6;
		tlock = passedPar & 63;
		if (isbadlock(tlock) || (lptr= &locks[tlock])->lstate==LFREE || lptr->ldesc != descript) {
			retVal = SYSERR;
		}else{
//kprintf("1");
			if(lptr->lusers[currpid]!=1){
		 		retVal = SYSERR;
//kprintf(" 2");
			}else{			
				lptr->lusers[currpid]=0;
				lptr->lcnt--;
//kprintf(" 3");
				if(lptr->lcnt==0){ 
//kprintf(" 4");
				// schedule next on queue
				// if type=read, release all other process waiting with type = read and priority greater than any other waiting process with type = write.
					if(nonempty(lptr->lqhead)){
//kprintf(" 5");
						if(lptr->ltype == READ){
							//If the current 'type' was read lock, then the queue would have 'tail' pointed to 'write'; followed, if at all by read process.
							//even if the read process has same priority, as per project specifications, it goes to write process(oldest).
							//to WRITE process
//kprintf(" 6");
							ready(getlast(lptr->lqtail),RESCHNO);
						}else{
							//If the current 'type' is write lock, and the 'tail' of queue has 'write' process. Same as first READ case.
//kprintf(" 7");
							if(q[q[lptr->lqtail].qprev].ltype == WRITE){
//kprintf(" 8");
								ready(getlast(lptr->lqtail),RESCHNO);
							}else{//Since, 'read' first in queue, we may have waiting 'write' process in queue with same priority. Which may also be in the 1sec limit, in which case, they get to run.
//kprintf(" 9");
								tprev = q[lptr->lqtail].qprev;
								ttype = q[tprev].ltype;
								ttime = q[tprev].ltime;
								while(q[tprev].qkey == q[q[tprev].qprev].qkey){
//kprintf(" 10");
									if(q[q[tprev].qprev].ltype == WRITE && (q[q[tprev].qprev].ltime - ttime < 1000)){
//kprintf(" 11");
										ttype = WRITE;
										break;
									}
									tprev = q[tprev].qprev;
								}


		 						if(ttype==READ){
//kprintf(" 12");
									tprev = q[lptr->lqtail].qprev;
									int breakPrio = 0;
									while( tprev != lptr->lqhead && (!breakPrio || tprio == q[tprev].qkey) ){

//kprintf(" 13");
										if(q[tprev].ltype != WRITE){
//kprintf(" 14");
											if (tprev < NPROC){
//kprintf("PROC:%d?",tprev);
												toRun = tprev;
												tprev = q[tprev].qprev;
												ready(dequeue(toRun),RESCHNO);
												//continue;
											}
											else//shouldn't go into this condition
												break;
										}else{ 
//kprintf(" 15");
											breakPrio = 1;
											tprio = q[tprev].qkey;
											tprev = q[tprev].qprev;
										}
										
									}
								}else{//WRITE
//kprintf(" 16");
									tprev = q[lptr->lqtail].qprev;
									while(q[tprev].ltype != WRITE && tprev != lptr->lqhead)
										tprev = q[tprev].qprev;
									if (tprev < NPROC)
										ready(dequeue(tprev),RESCHNO);
									else//shouldn't go into this condition
										break;
								}
							}
						}						
					}				
				}
			}
		}
	}
//kprintf("\r\n");
	resched();
	restore(ps);
	return(retVal);
}
