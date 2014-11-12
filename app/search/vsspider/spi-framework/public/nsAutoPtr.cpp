#include <stdio.h>
#include <stdlib.h>

#include "nsAutoPtr.h"

nsAutoPtr::nsAutoPtr()
{
}

nsAutoPtr::nsAutoPtr(void *ptr)
{
    Addptr(ptr);
}

nsAutoPtr::nsAutoPtr(int sock)
{
    Addsock(sock);
}

void nsAutoPtr::Addsock(int sock)
{
    if (sock >= 0)
	socks.push_back(sock);
}

void nsAutoPtr::Addptr(void *ptr)
{
    if (ptr)
	ptrs.push_back(ptr);
}

nsAutoPtr::~nsAutoPtr()
{
    if (socks.size()) {
	vector<int>::iterator iter;
	for (iter = socks.begin(); iter != socks.end(); iter ++)
	    close (*iter);
    }

    if (ptrs.size()) {
	vector<void *>::iterator vter;
	for (vter = ptrs.begin(); vter != ptrs.end(); vter ++)
	    free (*vter);
    }
}
