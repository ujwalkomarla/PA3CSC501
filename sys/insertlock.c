/* insertlock.c  -  insert lock*/

#include <conf.h>
#include <kernel.h>
#include <q.h>

/*------------------------------------------------------------------------
 * insertlock.c  --  insert an process into a q list in key order for locks
 *------------------------------------------------------------------------
 */
int insertlock(int proc, int head, int key, int type, int ltimed){

	int	next;			/* runs through list		*/
	int	prev;

	next = q[head].qnext;
	while (q[next].qkey < key)	/* tail has maxint as key	*/
		next = q[next].qnext;
	q[proc].qnext = next;
	q[proc].qprev = prev = q[next].qprev;
	q[proc].qkey  = key;

q[proc].ltype = type;
q[proc].ltime = ltimed;


	q[prev].qnext = proc;
	q[next].qprev = proc;
	return(OK);

}
