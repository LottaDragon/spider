#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <limits.h>

#include "mmapfile.h"

#define DEFAULT_MMAPFILE_PAGESIZE	4096

static int __mmapfile_pagesize = 0;

int mmapfile(const char *filename, int flags, mmapfile_t * mfile)
{
    int readonly = 1, pad0 = 1;
    int file;

    // __mmapfile_pagesize

    if (__mmapfile_pagesize == 0) {
	int pagesize = getpagesize();
	if (pagesize <= 0)
	    pagesize = DEFAULT_MMAPFILE_PAGESIZE;

	__mmapfile_pagesize = pagesize;
    }

    assert(filename);
    assert(mfile);

    if ((flags & OPT_RDWR))
	readonly = 0;
    else if ((flags & OPT_NPAD0))
	pad0 = 0;

    file = open(filename, (readonly ? O_RDONLY : O_RDWR), 0);
    if (file == -1)
	return -1;

    if (fstat(file, &(mfile->st)) == -1) {
	close(file);
	return -1;
    }

    mfile->memory = mmap(0, mfile->st.st_size,
			 (readonly ? (PROT_READ | PROT_WRITE) : (PROT_READ | PROT_WRITE)),
			 (readonly ? MAP_PRIVATE : MAP_SHARED), file, 0);

    if (mfile->memory == MAP_FAILED) {
	close(file);
	return -1;
    }

    close(file);

    mfile->__mmapfile_pad0 = 0;

    if (readonly) {
	void *mymemory = 0;

	if (pad0 && mfile->st.st_size % __mmapfile_pagesize == 0) {
	    // filesize is pagesize aligned, so we use our memory
	    // to skip "off-by-one bugs" caused "SIGBUG".
	    // so, mfile->memory is assured to terminated with '\0'.

	    mymemory = malloc(mfile->st.st_size + __mmapfile_pagesize);

	    if (mymemory) {
		memcpy(mymemory, mfile->memory, mfile->st.st_size);
		((char *) mymemory)[mfile->st.st_size] = 0;
	    }

	    munmap(mfile->memory, mfile->st.st_size);
	    mfile->memory = mymemory;
	    mfile->__mmapfile_pad0 = 1;

	    return ((mymemory) ? 0 : -1);
	} else
	    madvise(mfile->memory, mfile->st.st_size, MADV_SEQUENTIAL);
    }

    return 0;
}

int mmapfile2(const char *filename, int flags, mmapfile_t * mfile, size_t size, void **dst, size_t * nmemb)
{
    if (mmapfile(filename, flags, mfile) == -1)
	return -1;

    if (mfile->st.st_size % size) {
	mmapfile_close(mfile);
	errno = EBADMSG;
	return -1;
    }

    if (dst)
	*dst = mfile->memory;
    if (nmemb)
	*nmemb = mfile->st.st_size / size;

    return 0;
}

int mmapfile_close(mmapfile_t * mfile)
{
    // __mmapfile_pagesize

    if (__mmapfile_pagesize == 0) {
	int pagesize = getpagesize();
	if (pagesize <= 0)
	    pagesize = DEFAULT_MMAPFILE_PAGESIZE;

	__mmapfile_pagesize = pagesize;
    }

    if (mfile) {
	if (mfile->__mmapfile_pad0 && mfile->st.st_size % __mmapfile_pagesize == 0) {
	    if (mfile->memory) {
		free(mfile->memory);
		mfile->memory = 0;
	    }
	} else if (mfile->memory)
	    munmap(mfile->memory, mfile->st.st_size);
    }

    return 0;
}
