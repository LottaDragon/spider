#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "mcpack.h"
#include "misc.h"

#ifndef	WARNING
#define WARNING(fmt, args...) \
    do { \
	fprintf(stderr, "WARNING (%s:%d), " fmt "\n", __FILE__, __LINE__,  ##args); \
    } while (0)

#endif	/* ~ WARNING */

int load_mcpack(mcpack_description_t *md, void *mcpack)
{
    (void) md;
    (void) mcpack;

    while (md->addr) {
	int type = md->options & O_typeMask;

	switch (type) {
	    case O_int:
		if (md->addr)
		    *(int*)(md->addr) = 0;

		// (void)(mcpack, (int*)(md->addr));
		break;
	    case O_str:
		if (md->addr)
		    *(char **)(md->addr) = NULL;
		break;

	    default:
		WARNING("XXX, Unknown md->type %d, why?", type);
		break;
	}
	
	md ++;
    }

    return 0;
}

int pack_mcpack(mcpack_description_t *md, void *mcpack, int maxsize)
{
    (void) md;
    (void) mcpack;
    (void) maxsize;

    return 0;
}

void TRACE_mcpack(mc_pack_t *mcpack, char *descript)
{
    char tmpbuf[102400];

    *tmpbuf = '\0';
    if (mc_pack_pack2text(mcpack, tmpbuf, 102400, 0) == 0)
	TRACE("%s [%s]", descript?descript:"(mcpack)", tmpbuf);
    else WARNING("%s [mcpack-too-large]", descript?descript:"(mcpack)");
}

#ifdef	_MCPACK_CREATE_TESTSUITE__
#define	_MCPACK_CREATE_TESTSUITE__

int main(int argc, char **argv)
{
    (void) argc; (void) argv;

    struct {
	char *name;
	int type;
	int value;
    } e;

    struct mcpack_description_t
	e_description [] = {
	    {&e.name, "Name", O_str},
	    {&e.type, "PackType", O_int | O_optional},
	    {&e.value, "Value", O_int | O_optional},
	    {0, 0, 0}
	};

    void *mcpack;
    load_mcpack(e_description, mcpack);

    if (e.name && strcmp (e.name, "ec/BA") == 0) {
	return e.type;
    }

    return 0;
}

#endif	/* ~ _MCPACK_CREATE_TESTSUITE__ */
