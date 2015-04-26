#include<lock.h>
#include<q.h>

int	nextlock;
int appendDesc;
struct	lentry	locks[NLOCKS];
void linit(void){
appendDesc = 0;
nextlock = NLOCKS-1;
	struct	lentry	*lptr;
	int i;
	for (i=0 ; i<NLOCKS ; i++) {	/* initialize semaphores */
		(lptr = &locks[i])->lstate = LFREE;
		lptr->lqtail = 1 + (lptr->lqhead = newqueue());
	}
}
