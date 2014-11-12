/**
 * $Id: rewrite.cpp,v 1.1 2009/06/24 13:15:04 pengdw Exp $
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "rewrite.h"
#include "misc.h"

rewrite::rewrite()
{
    regex = NULL;
}

rewrite::~rewrite() {
    if (regex) pcre_free(regex);
}

rewrite::rewrite(const rewrite &re)
{
    if (re.replace.size()) replace = re.replace;
    
    /* re-compile a regex */
    if (setregex(re.match) == -1)
	assert(0);
}

int rewrite::setregex(string regstr) {
    const char *errptr;
    int erroff;

    if (NULL == (regex = pcre_compile(regstr.c_str(),
                                      0, &errptr, &erroff, NULL))) {
        WARNING("regex compilation for %s failed at %s", regstr.c_str(), errptr);
        return -1;
    }

    match = regstr;
    return 0;
}

int rewrite::setreplace(string replacestr) {
    replace = replacestr;
    return 0;
}

int rewrite::pcre_match(const string &match_buf)
{
    return pcre_match(match_buf.c_str(), match_buf.length());
}

int rewrite::pcre_match(const char *match_buf)
{
    return pcre_match(match_buf, strlen(match_buf));
}

int rewrite::pcre_match(const char *match_buf, size_t len)
{
    int ovec [10 * 3];
    int n;

    if ((n = pcre_exec(regex, NULL, match_buf, len, 0, 0, ovec, 30)) < 0) {
	if (n != PCRE_ERROR_NOMATCH) {
	    return n;
	}

	return 0;
    }

    return 1;
}

int rewrite::pcre_replace(const string &match_buf, string &result) {
    const char *pattern = replace.c_str();
    size_t pattern_len = replace.length();

# define N 10
    int ovec[N * 3];
    int n;

    if ((n = pcre_exec(regex, NULL, match_buf.c_str(), match_buf.length(), 0, 0, ovec, 3 * N)) < 0) {
        if (n != PCRE_ERROR_NOMATCH) {
            return n;
        }
    } else {
        const char **list;
        size_t start, end;
        size_t k;

        /* it matched */
        pcre_get_substring_list(match_buf.c_str(), ovec, n, &list);

        /* search for $[0-9] */
        result.clear();

        start = 0;
        end = pattern_len;
        for (k = 0; k < pattern_len; k++) {
            if ((pattern[k] == '$') &&
                    isdigit((unsigned char)pattern[k + 1])) {
                /* got one */

                size_t num = pattern[k + 1] - '0';

                end = k;

                result.append(pattern + start, end - start);

                /* n is always > 0 */
                if (num < (size_t)n) {
                    result.append(list[num]);
                }

                k++;
                start = k + 1;
            }
        }

        result.append(pattern + start, pattern_len - start);
        pcre_free(list);
    }

    return n;
}
