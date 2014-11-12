/**
 * $Id: mmapfile.h,v 1.1 2009/03/16 01:00:07 pengdw Exp $
 */

#ifndef	_MMAPFILE_H__
#define	_MMAPFILE_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct {
    void *memory;
    struct stat st;

    int __mmapfile_pad0;
} mmapfile_t;

/**
 * 0, OPT_RDONLY = {PROT_READ, MAP_PRIVATE, MADV_SEQUENTIAL, ~ OPT_NPAD0}
 * OPT_RDONLY | OPT_NPAD0 = {PROT_READ, MAP_PRIVATE, MADV_SEQUENTIAL, OPT_NPAD0}
 *
 * OPT_RDWR = {PROT_READ | PROT_WRITE, MAP_SHARED}
 */

#define OPT_RDONLY	00
#define OPT_NPAD0	01
#define OPT_RDWR	02

int mmapfile(const char *filename, int flags, mmapfile_t * mfile);
int mmapfile2(const char *filename, int flags, mmapfile_t * mfile, size_t size, void **dst, size_t * nmemb);
int mmapfile_close(mmapfile_t * mfile);

#endif				/* ~ _MMAPFILE_H__ */
