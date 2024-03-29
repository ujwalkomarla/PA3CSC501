//#define DEBUG
/* lock.h - isbadlock */
//#include <proc.h>
#ifndef _LOCK_H_
#define _LOCK_H_

#ifndef	NLOCKS
#define	NLOCKS		50	/* number of locks, if not defined	*/
#endif

#define	LFREE	'\01'		/* this lock is free		*/
#define	LUSED	'\02'		/* this lock is used		*/

#define READ '\01' 			/*this lock is held for read*/
#define WRITE '\02' 		/*this lock is held for write*/
//DELETED Defined in kernel.h
// #define DELETED '\03'		/*this lock is deleted*/

struct	lentry	{		/* lock table entry		*/
	char lstate;		/* the state LFREE or LUSED		*/
	char ltype;		/* held for READ or WRITE */


	int	lcnt;		/* count for this lock		*/



//Which all process hold the lock, as in created or acquired the lock.
	int lowner;//REQUIRED?
	int lusers[NLOCKS];//for READ locks?



	int ldesc;		/* lock descriptor	*/

	int	lqhead;		/* q index of head of list		*/
	int	lqtail;		/* q index of tail of list		*/
};
extern	struct	lentry	locks[];
extern	int	nextlock;
extern int appendDesc;

void linit(void);
#define	isbadlock(s)	(s<0 || s>=NLOCKS)

#endif

