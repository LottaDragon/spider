/**
 * mcpack.o, try make mcpack easy to handle.
 *
 * XXX, this is a wrapper try to make mcpack easy, but NOT FINISHED.
 *
 * but, i want to say "mcpack sucks!!!".
 *
 * $Id: mcpack.h,v 1.9 2009/04/21 13:46:07 pengdw Exp $
 */

#ifndef	_MCPACK_H__
#define	_MCPACK_H__

// true mcpack.
#include "mc_pack.h"
#include "nshead.h"

enum {
    O_str = 1,
    O_int,
    O_uint,
    O_int64,
    O_uint64,

    O_typeMask = 0xFF,
};

enum {
    O_optional = 0x100,
};

typedef struct mcpack_description_t {
    void *addr;
    char *name;
    int options;
} mcpack_description_t;

int load_mcpack(mcpack_description_t * md, void *mcpack);
int pack_mcpack(mcpack_description_t * md, void *mcpack, int maxsize);

void TRACE_mcpack(mc_pack_t *mcpack, char *description = NULL);

#define PREPARE_MCPACK(tmpbuf_, max_tmpbuf_size_) \
    nshead_t *nshead; \
    mc_pack_t *mcpack; \
 \
    int max_mpkbuf_size, \
        max_idxbuf_size; \
 \
    max_mpkbuf_size = (max_tmpbuf_size_) / 2; \
    max_idxbuf_size = (max_tmpbuf_size_) - (int)sizeof(nshead_t) - max_mpkbuf_size; \
 \
    assert(max_mpkbuf_size > 0 && max_idxbuf_size > 0); \
 \
    char *mpkbuf = (tmpbuf_) + sizeof(nshead_t), \
         *idxbuf = (tmpbuf_) + sizeof(nshead_t) + max_mpkbuf_size; \
 \
    nshead = (nshead_t*) (tmpbuf_); \
    mcpack = mc_pack_open_w(2, mpkbuf, max_mpkbuf_size, idxbuf, max_idxbuf_size); \
 do {} while(0)

#define PREPARE_MCPACK_isvalid(max_tmpbuf_size_, size_) \
    ((size_) > 0 && (size_) < (max_tmpbuf_size_) / 2)

#define PREPARE_MCPACK_readonly(tmpbuf_, max_tmpbuf_size_, pad0) \
    nshead_t *nshead##pad0; \
    mc_pack_t *mcpack##pad0; \
 \
    int max_mpkbuf_size##pad0, \
        max_idxbuf_size##pad0; \
 \
    max_mpkbuf_size##pad0 = (max_tmpbuf_size_) / 2; \
    max_idxbuf_size##pad0 = (max_tmpbuf_size_) - (int)sizeof(nshead_t) - max_mpkbuf_size##pad0; \
 \
    assert(max_mpkbuf_size##pad0 > 0 && max_idxbuf_size##pad0 > 0); \
 \
    char *mpkbuf##pad0 = (tmpbuf_) + sizeof(nshead_t), \
         *idxbuf##pad0 = (tmpbuf_) + sizeof(nshead_t) + max_mpkbuf_size##pad0; \
 \
    nshead##pad0 = (nshead_t*) (tmpbuf_); \
    mcpack##pad0 = mc_pack_open_r(mpkbuf##pad0, max_mpkbuf_size##pad0, \
	    idxbuf##pad0, max_idxbuf_size##pad0); \
 do {} while(0)

#define PREPARE_MCPACK_readonly0(tmpbuf_, max_tmpbuf_size_, pad0) \
    nshead##pad0 = (nshead_t*) (tmpbuf_); \
    mcpack##pad0 = mc_pack_open_r(mpkbuf##pad0, max_mpkbuf_size##pad0, \
	    idxbuf##pad0, max_idxbuf_size##pad0); \
 do {} while(0)

#define PREPARE_MCPACK_append(tmpbuf_, max_tmpbuf_size_, pad0) \
    nshead_t *nshead##pad0; \
    mc_pack_t *mcpack##pad0; \
 \
    int max_mpkbuf_size##pad0, \
        max_idxbuf_size##pad0; \
 \
    max_mpkbuf_size##pad0 = (max_tmpbuf_size_) / 2; \
    max_idxbuf_size##pad0 = (max_tmpbuf_size_) - (int)sizeof(nshead_t) - max_mpkbuf_size##pad0; \
 \
    assert(max_mpkbuf_size##pad0 > 0 && max_idxbuf_size##pad0 > 0); \
 \
    char *mpkbuf##pad0 = (tmpbuf_) + sizeof(nshead_t), \
         *idxbuf##pad0 = (tmpbuf_) + sizeof(nshead_t) + max_mpkbuf_size##pad0; \
 \
    nshead##pad0 = (nshead_t*) (tmpbuf_); \
    mcpack##pad0 = mc_pack_open_rw(mpkbuf##pad0, max_mpkbuf_size##pad0, \
	    idxbuf##pad0, max_idxbuf_size##pad0); \
 do {} while(0)

#endif				/* ~ _MCPACK_H__ */
