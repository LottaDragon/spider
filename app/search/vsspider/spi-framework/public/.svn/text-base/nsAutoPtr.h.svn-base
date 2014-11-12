/**
 * $Id: nsAutoPtr.h,v 1.2 2009/03/18 13:11:51 pengdw Exp $
 */

#ifndef	_nsAutoPtr_h__
#define	_nsAutoPtr_h__

#include <vector>

using namespace std;

class nsAutoPtr {
public:
    nsAutoPtr();
    nsAutoPtr(void *ptr);
    nsAutoPtr(int sock);

    ~nsAutoPtr();

    void Addsock(int sock);
    void Addptr(void *ptr);

private:
    vector<int> socks;
    vector<void *> ptrs;
};

#endif	/* ~ _nsAutoPtr_h__ */
