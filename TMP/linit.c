#include<lock.h>
void linit(void){
	struct	lentry	*lptr;
	for (i=0 ; i<NLOCK ; i++) {	/* initialize semaphores */
		(lptr = &locks[i])->lstate = LFREE;
		lptr->lqtail = 1 + (lptr->lqhead = newqueue());
	}
}
