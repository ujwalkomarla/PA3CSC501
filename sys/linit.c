#include<lock.h>
#include<q.h>
struct	lentry	locks[NLOCKS];
int	nextlock;
int appendDesc;
void linit(void){
appendDesc = 0;
nexLock = NLOCKS-1;
	struct	lentry	*lptr;
	for (i=0 ; i<NLOCK ; i++) {	/* initialize semaphores */
		(lptr = &locks[i])->lstate = LFREE;
		lptr->lqtail = 1 + (lptr->lqhead = newqueue());
	}
}
