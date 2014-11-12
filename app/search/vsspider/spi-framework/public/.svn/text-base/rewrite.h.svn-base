/**
 * $Id: rewrite.h,v 1.2 2009/06/24 14:21:42 pengdw Exp $
 *
 *  rewrite-once = {
 *    "*" => { "^http://en.wikipedia.org/(.*)" => "http://127.0.0.1:1025/proxyme/$1" },
 *  }
 *
 *  rewrite-final = {
 *    "1,2,3,4-12" => { "^/proxyme/?(.*)" => "/$1" },
 *    "13" => { ".*" => "en.wikipedia.org" },
 *  }
 */

#ifndef	_rewrite_h__
#define	_rewrite_h__

#include <pcre.h>
#include <string>

using namespace std;

class rewrite {
public:
    rewrite();
    ~rewrite();

    int setregex(string regstr);
    int setreplace(string replacestr);

    int pcre_replace(const string &match_buf, string &result);

    int pcre_match(const string &match_buf);
    int pcre_match(const char *match_buf);
    int pcre_match(const char *match_buf, size_t len);

    /** copy-ctor */
    rewrite(const rewrite &re);

private:
    pcre *regex; /* regex compiled from the <match> */

    string match;
    string replace;
};

#endif	/* ~ _rewrite_h__ */
