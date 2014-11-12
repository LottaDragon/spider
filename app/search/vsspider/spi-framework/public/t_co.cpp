#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <assert.h>

#include "cooperate.h"
#include "misc.h"

int n1, n2;

int p0 (int idx, void *val)
{
	(void)(intptr_t)val;
	static int count = 0;
	if (count ++ == 30) return 1;

	// printf ("%d empty now\n", idx);
	// sleep (rand () % 3);
	printf ("%d ok now, send out\n", idx);
	n1 ++;
	return 0;
}

int p1 (int idx, void *val)
{
	(void)(intptr_t)val;
	// printf ("%d full now, work ...\n", idx);
	sleep (rand () % 2);
	printf ("%d finished now, send back\n", idx);
	n2 ++;
	return 0;
}

int main ()
{
    cooperate_t cop;

    if (cop.Init(10, 10) == -1) {
	WARNING("Init failed, %m");
	return -1;
    }

    if (cop.Run(p0, 0, p1, 0, M_STABLE | M_STEPOVER) == -1) {
	WARNING("Run failed, %m");
	return -1;
    }

    printf ("n1 %d, n2 %d\n", n1, n2);
    TestOK ("cooperate", n1 == n2);

    return 0;
}
