/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: uln_url.cpp,v 1.17 2008/08/01 07:30:30 wangjg Exp $ 
 * 
 **************************************************************************/



/**
 * @file uln_url.cpp
 * @author wangjg(com@baidu.com)
 * @date 2008/01/17 13:40:38
 * @version $Revision: 1.17 $ 
 * @brief 
 *  
 **/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "uln_url.h"

#define FIRST_HEX_CHAR(x) ((u_char)(x) >> 4)
#define SECOND_HEX_CHAR(x) ((x) & ((1 << 4) - 1))
#define HEX_TO_CHAR(x1, x2) (((u_char)(x1) << 4) | (x2))

const char* const ULN_STD_SCHEME = "http";
const char* const ULN_STD_HOST = "www.baidu.com";
const char* const ULN_STD_PATH = "/";
const char* const ULN_STD_USERINFO = "user";

const int ULN_CHECK = 1;
const int ULN_COPY = 2;

static 
long long low_mask(u_char from_char, u_char to_char)
{
	long long res = 0;
	int f = from_char > 63 ? 63 : from_char;
	int l = to_char > 63 ? 63 : to_char;
	for (int i = f; i <= l; i++) {
		res |= 1LL << i;
	}
	return res;
}

static 
long long high_mask(u_char from_char, u_char to_char)
{
	long long res = 0;
	int f = from_char < 64 ? 64 : (from_char > 127 ? 127 : from_char);
	int l = to_char < 64 ? 64 : (to_char > 127 ? 127 : to_char);
	f -= 64;
	l -= 64;
	for (int i = f; i <= l; i++) {
		res |= 1LL << i;
	}
	return res;
}

static 
long long low_mask(char* chars)
{
	long long res = 0;
	u_char* p_char = (u_char *)chars;
	while(p_char != NULL && *p_char) {
		if (*p_char < 64) {
			res |= 1LL << (*p_char);
		}
		p_char++;
	}
	return res;
}

static 
long long high_mask(char* chars)
{
	long long res = 0;
	u_char* p_char = (u_char *)chars;
	while (p_char != NULL && *p_char) {
		if (*p_char >= 64 && *p_char < 128) {
			res |= 1LL << (*p_char - 64);
		}
		p_char++;
	}
	return res;
}


/* digit    = [0-9] */
static const long long L_DIGIT = low_mask('0', '9');
static const long long H_DIGIT = 0;

/* upalpha  = [A-Z] */
static const long long L_UPALPHA = 0;
static const long long H_UPALPHA = high_mask('A', 'Z');

/* lowalpha = [a-z] */
static const long long L_LOWALPHA = 0;
static const long long H_LOWALPHA = high_mask('a', 'z');

/* alpha    = lowalpha | upalpha */
static const long long L_ALPHA = L_LOWALPHA | L_UPALPHA;
static const long long H_ALPHA = H_LOWALPHA | H_UPALPHA;

/* alphanum = alpha | digit */
static const long long L_ALPHANUM = L_ALPHA | L_DIGIT;
static const long long H_ALPHANUM = H_ALPHA | H_DIGIT;

/* hex      = digit | [A-F] | [a-f] */
static const long long L_HEX = L_DIGIT;
static const long long H_HEX = high_mask('A', 'F') | high_mask('a', 'f');

/* mark     = [-_.!~*'()] */
static const long long L_MARK = low_mask("-_.!~*'()");
static const long long H_MARK = high_mask("-_.!~*'()");

/* unreserved = alphanum | mark */
static const long long L_UNRESERVED = L_ALPHANUM | L_MARK;
static const long long H_UNRESERVED = H_ALPHANUM | H_MARK;

/* reserved   = ";" | "/" | "?" | ":" | "@" | "&" | "=" | "+" | "$" | "," | "[" | "]" */
/* 根据 RFC 2732, 增加"[", "]"为保留字符*/
static const long long L_RESERVED = low_mask(";/?:@&=+$,[]");
static const long long H_RESERVED = high_mask(";/?:@&=+$,[]");


/* 最低位为1, 表示可以接受 非ASCII字符, 即ascii码 [128-255] */
static const long long L_ESCAPED = 1;
static const long long H_ESCAPED = 0;

static const long long L_DELETE = 0;
static const long long H_DELETE = (long long) (1ULL << 63);

static const long long L_SLASH = low_mask("\\");
static const long long H_SLASH = high_mask("\\");

/* 此类字符是RFC不允许的，但为了如旧库一样提高回召率 */
static const long long L_SPECIAL = low_mask("^{}[]`|!") | L_DELETE;
static const long long H_SPECIAL = high_mask("^{}[]`|!") | H_DELETE;

/* uric     = reserved | unreserved | escaped */
static const long long L_URIC = L_RESERVED | L_UNRESERVED | L_ESCAPED | L_SPECIAL | L_SLASH;
static const long long H_URIC = H_RESERVED | H_UNRESERVED | H_ESCAPED | H_SPECIAL | H_SLASH;

/* pchar    = unreserved | escaped | ":" | "@" | "&" | "=" | "+" | "$" | "," */
static const long long L_PCHAR = L_UNRESERVED | L_ESCAPED | low_mask(":@&=+$,") | L_SPECIAL;
static const long long H_PCHAR = H_UNRESERVED | H_ESCAPED | high_mask(":@&=+$,") | H_SPECIAL;

/* 所有path合法字符, pchar | ";" | "/" */
static const long long L_PATH = L_PCHAR | low_mask(";/?");
static const long long H_PATH = H_PCHAR | high_mask(";/?");


/* userinfo      = *( unreserved | escaped | ":" | "&" | "=" | "+" | "$" | "," )*/
static const long long L_USERINFO = L_UNRESERVED | L_ESCAPED | low_mask(":&=+$,");
static const long long H_USERINFO = H_UNRESERVED | H_ESCAPED | high_mask(":&=+$,");

static const long long L_HOST =  L_ALPHANUM | low_mask(".-_");
static const long long H_HOST =  H_ALPHANUM | high_mask(".-_");

/* scheme        = alpha *( alpha | digit | "+" | "-" | "." ) */
static const long long L_SCHEME = L_ALPHA | L_DIGIT | low_mask("+-.");
static const long long H_SCHEME = H_ALPHA | H_DIGIT | high_mask("+-.");


/* 本库允许的不安全字符，" " | "\"" | "<" | ">" */
static const long long L_UNSAFE = low_mask(" \"<>");
static const long long H_UNSAFE = high_mask(" \"<>");

static const long long L_SAFE = L_UNRESERVED | L_ESCAPED;
static const long long H_SAFE = H_UNRESERVED | H_ESCAPED;

/* 在url中出现以下字符将忽略并删除: "\r" | "\n" */
static const long long L_OMMIT = low_mask("\r\n");
static const long long H_OMMIT = high_mask("\r\n");

static const long long L_HOSTEND = low_mask(":/@");
static const long long H_HOSTEND = high_mask(":/@");

static const long long L_PATHEND = low_mask("?#");
static const long long H_PATHEND = high_mask("?#");

static char ULN_SCHEME_CHAR[256];
static char ULN_USERINFO_CHAR[256];
static char ULN_PATH_CHAR[256];
static char ULN_HEX_CHAR[256];
static char ULN_DIGIT_CHAR[256];
static char ULN_UNSAFE_CHAR[256];
static char ULN_SAFE_CHAR[256];
static char ULN_OMMIT_CHAR[256];
static char ULN_HOST_CHAR[256];
static char ULN_URIC_CHAR[256];
static char ULN_ESCAPED_CHAR[256];

static char ULN_DIG_TO_HEX[16];
static char ULN_TO_LOWER[256];
static char ULN_TO_UPPER[256];
static char ULN_HEX_TO_DIG[256];

static char ULN_HOSTEND_CHAR[256];
static char ULN_PATHEND_CHAR[256];

const int ULN_URL_MAX_NAME_LEN = 36;
typedef struct ul_ilist {
	char name[ULN_URL_MAX_NAME_LEN];
	u_int length;
} ul_ilist_t;

const ul_ilist_t ul_ignore_list[] = {
	{"ac.",3},
	{"ah.",3},
	{"bj.",3},
	{"co.",3},
	{"com.",4},
	{"cq.",3},
	{"ed.",3},
	{"edu.",4},
	{"fj.",3},
	{"gd.",3},
	{"go.",3},
	{"gov.",4},
	{"gs.",3},
	{"gx.",3},
	{"gz.",3},
	{"ha.",3},
	{"hb.",3},
	{"he.",3},
	{"hi.",3},
	{"hk.",3},
	{"hl.",3},
	{"hn.",3},
	{"jl.",3},
	{"js.",3},
	{"jx.",3},
	{"ln.",3},
	{"mo.",3},
	{"ne.",3},
	{"net.",4},
	{"nm.",3},
	{"nx.",3},
	{"or.",3},
	{"org.",4},
	{"pe.",3},
	{"qh.",3},
	{"sc.",3},
	{"sd.",3},
	{"sh.",3},
	{"sn.",3},
	{"sx.",3},
	{"tj.",3},
	{"tw.",3},
	{"www.",4},
	{"xj.",3},
	{"xz.",3},
	{"yn.",3},
	{"zj.",3}
};


const int ULN_URL_IGNORE_COUNT = sizeof(ul_ignore_list) / sizeof(ul_ilist_t);

static int g_uln_multi_schemes = 0;
static encode_mode_t g_uln_escaped_mode = ULN_TO_ESCAPED_MODE;

static 
inline bool match(long long L_SET, long long H_SET, u_int c)
{
	if (c < 64) {
		return (L_SET & (1LL << c)) ? 1 : 0;
	} else if (c < 128) {
		return (H_SET & (1LL << (c - 64))) ? 1 : 0;
	} else {
		return (L_SET & 1) ? 1 : 0;
	}
}

static 
int register_chars() {
	for (u_int i = 0; i < 256; i++) {
		ULN_SCHEME_CHAR[i] = match(L_SCHEME, H_SCHEME, i);
		ULN_USERINFO_CHAR[i] = match(L_USERINFO, H_USERINFO, i);
		ULN_DIGIT_CHAR[i] = match(L_DIGIT, H_DIGIT, i);
		ULN_PATH_CHAR[i] = match(L_PATH, H_PATH, i);
		ULN_HEX_CHAR[i] = match(L_HEX, H_HEX, i);
		ULN_UNSAFE_CHAR[i] = match(L_UNSAFE, H_UNSAFE, i);
		ULN_SAFE_CHAR[i] = match(L_SAFE, H_SAFE, i);
		ULN_OMMIT_CHAR[i] = match(L_OMMIT, H_OMMIT, i);
		ULN_HOST_CHAR[i] = match(L_HOST, H_HOST, i);
		ULN_URIC_CHAR[i] = match(L_URIC, H_URIC, i);
		ULN_ESCAPED_CHAR[i] = match(L_ESCAPED, H_ESCAPED, i);
		ULN_HOSTEND_CHAR[i] = match(L_HOSTEND, H_HOSTEND, i);
		ULN_PATHEND_CHAR[i] = match(L_PATHEND, H_PATHEND, i);

		ULN_TO_LOWER[i] = (char) i;
		ULN_TO_UPPER[i] = (char) i;
		if (i >= 'A' && i <= 'Z') {
			ULN_TO_LOWER[i] = (char) (i + 'a' - 'A');
		} else if (i >= 'a' && i <= 'z') {
			ULN_TO_UPPER[i] = (char) (i + 'A' - 'a');
		}

		ULN_HEX_TO_DIG[i] = 0;
	}

	for (u_int i = 0; i < 16; i++) {
		if (i < 10) {
			ULN_DIG_TO_HEX[i] = (char) (i + '0');
			ULN_HEX_TO_DIG[i + '0'] = (char) i;
		} else {
			ULN_DIG_TO_HEX[i] = (char) (i - 10 + 'A');
			ULN_HEX_TO_DIG[i - 10 + 'a'] = (char) i;
			ULN_HEX_TO_DIG[i - 10 + 'A'] = (char) i;
		}
	}
	return 0;
}

static int register_res = register_chars();


int uln_clear_url(uln_url_t* p_url)
{
	if (NULL == p_url) {
		return -1;
	}
	p_url->scheme[0] = 0;
	p_url->userinfo[0] = 0;
	p_url->host[0] = 0;
	p_url->port = -1;
	p_url->path[0] = 0;
	p_url->query[0] = 0;
	p_url->fragment[0] = 0;
	return 0;
}

static
int uln_check_scheme(const char* scheme)
{
	if (NULL == scheme) {
		return 0;
	}
	const u_char* p = (const u_char *)scheme;
	while (*p) {
		if (!ULN_SCHEME_CHAR[*p]) {
			return 0;
		}

		p++;
	}

	if (*scheme != 0 && 0 == g_uln_multi_schemes) {
		return 0 == strcasecmp(scheme, "http");
	}

	return 1;
}

static
int uln_check_userinfo(const char* userinfo)
{
	const u_char* p = (const u_char*) userinfo;
	if (NULL == p) {
		return 0;
	}

	while (*p) {
		if (!ULN_USERINFO_CHAR[*p]) {
			return 0;
		}
		p++;
	}

	return 1;
}

static
int uln_check_ipv6_address(const char* host, u_int host_len)
{
	if (NULL == host || host_len <= 2) {
		return 0;
	}

	if (*host != '[') {
		return 0;
	}
	
	const char* end = host + host_len;

	host++;
	
	// simple check, need to update, but currently, not neccessary.
	while (host != end) {
		if (0 == ULN_HEX_CHAR[(u_char) *host]
				&& *host != '.' && *host != ':' && !(']' == *host && (host + 1) == end)) {
			return 0;
		}
		host++;
	}

	if (*(host - 1) != ']') {
		return 0;
	}

	return 1;

}

static
int uln_check_host(const char* host, u_int host_len)
{
	if (NULL == host|| 0 == host_len) {
		return 0;
	}
	
	if ('[' == *host) {
		return uln_check_ipv6_address(host, host_len);
	}

	// must begin with ALPHANUM, or %xy char, for IPv4
	if ('.' == *host || '-' == *host) {
		return 0;
	}

	int doc_count = 0;
	const char* end = host + host_len;
	while (host != end) {
		if ('%' == *host) {
			// %xy is allowed
			if (!ULN_HEX_CHAR[(u_char) *(host + 1)] || !ULN_HEX_CHAR[(u_char) *(host + 2)]) {
				return 0;
			}
		} else if (!ULN_HOST_CHAR[(u_char) *host]) {
			return 0;
		}

		if ('.' == *host) {
			doc_count++;
		}
		host++;
	}

	if (0 == doc_count|| !match(L_ALPHANUM, H_ALPHANUM, (u_int) (int) *(host - 1))) {
		return 0;
	}

	return 1;
}

static
int uln_check_path(const char* path, u_int path_len)
{

	if (NULL == path|| 0 == path_len) {
		return 0;
	}

	if (*path != '/') {
		return 0;
	}
	
	const u_char* p = (const u_char*)path;
	const u_char* end = p + path_len;
	int before_colon = 1;
	while (p != end) {
		if (!ULN_PATH_CHAR[*p] && *p != '%') {
			if (*p != '\\' || before_colon) {
				return 0;
			}
		}
		if (';' == *p) {
			before_colon = 0;
		}
		p++;
	}

	return 1;
}

static
int uln_check_query(const char* query, u_int query_len)
{
	if (NULL == query) {
		return 0;
	}
	const u_char* p = (const u_char*) query;
	const u_char* end = (const u_char*) p + query_len;

	while (p != end) {
		if (!ULN_URIC_CHAR[*p] && *p != '%') {
			return 0;
		}
		p++;
	}

	return 1;
}

static
int uln_check_fragment(const char* fragment)
{
	const u_char* p = (const u_char*)fragment;

	if (NULL == p) {
		return 0;
	}

	while (*p) {
		if (!ULN_URIC_CHAR[*p] && *p != '%') {
			return 0;
		}
		p++;
	}

	return 1;
}

static
inline const char* my_strchr(const char* s, const char* tokens)
{
	if (NULL == s|| NULL == tokens) {
		return NULL;
	}
	const char *p = NULL;
	while (*s) {
		p = tokens;
		while (*p) {
			if (*s == *p) {
				return s;
			}
			p++;
		}
		s++;
	}
	return NULL;
	
}

static
const char* find_char(const char*s, char* char_set, const char** e)
{
	if (NULL == s|| NULL == char_set || NULL == e) {
		return NULL;
	}
	*e = NULL;
	while (*s) {
		if (char_set[(u_char) *s]) {
			return s;
		}
		s++;
	}
	*e = s;
	return NULL;
}

static
const char* find_path_end(const char* s, const char** end)
{
    if (s == NULL) {
        return NULL;
    }

    while (*s) {
        if (ULN_PATHEND_CHAR[(u_char) *s]) {
            if (*s != '?' || *(s + 1) != 0) {
                return s;
            }
        } 
        s++;
    }
    *end = s;
    return NULL;
}

static
inline int uln_strlcpy(char* buf, const char* s, u_int buf_len)
{
	u_int s_len = strlen(s);
	if (s_len >= buf_len) {
		return -1;
	}
	memcpy(buf, s, s_len);
	buf[s_len] = 0;
	return (int) s_len;
}

static
inline int uln_strlcpy(char* buf, const char* s1, const char* s2, u_int buf_len)
{
	u_int s1_len = strlen(s1);
	u_int s2_len = strlen(s2);
	if (s1_len + s2_len >= buf_len) {
		return -1;
	}
	memcpy(buf, s1, s1_len);
	memcpy(buf + s1_len, s2, s2_len);
	buf[s1_len + s2_len] = 0;
	return (int) (s1_len + s2_len);
}

static
int uln_parse_url_strict(const char* url, uln_url_t* p_url, int check_flag)
{
	if (NULL == url|| NULL == p_url) {
		return -1;
	}
	
	if (uln_clear_url(p_url) < 0) {
		return -1;
	}

	const char* p = NULL;
	p = strchr(url, '/');
	if (NULL == p|| p == url || *(p - 1) != ':' || *(p + 1) != '/') {
		p = NULL;
	} else {
		// found "://"
		p--;
	}

	// parse scheme
	if (p != NULL) {
		// scheme found
		u_int scheme_len = (u_int)(p - url);
		if (scheme_len >= ULN_URL_MAX_SCHEME_LEN - 1) {
			// buffer overflow
			return -1;
		}
		
		memcpy(p_url->scheme, url, scheme_len);
		p_url->scheme[scheme_len] = 0;
		if ((check_flag & 1) && uln_check_scheme(p_url->scheme) == 0) {
			return -1;
		}

		p += 3;
	} else {
		// scheme not found
		p = url;
	}

	const char* end = NULL;
	const char* q = find_char(p, ULN_HOSTEND_CHAR, &end);
	
	if (q != NULL && '@' == *q) {
		// p .. q : userinfo@
		u_int userinfo_len = (u_int) (q - p);
		if (userinfo_len >= ULN_URL_MAX_USERINFO_LEN - 1) {
			// buffer overflow
			return -1;
		}

		memcpy(p_url->userinfo, p, userinfo_len);
		p_url->userinfo[userinfo_len] = 0;
	
		if ((check_flag & 1) && uln_check_userinfo(p_url->userinfo) == 0) {
			return -1;
		}

		// go on
		p += userinfo_len + 1;
		q = my_strchr(p, "/:");

	}

	u_int host_len = 0;
	
	if ('[' == *p) { // ipv6 host
		// "[xxxxx]" is host
		q = strchr(p, ']');
		if (NULL == q) {
			// "[xxx]" bracket not match
			return -1;
		}

		host_len = (u_int) (q + 1 - p);
		q++;
	
	} else if (NULL == q) {
		// p .. 串尾 host
		host_len = strlen(p);
	} else {
		// p .. q : host: | host/
		host_len = (u_int) (q - p);
	}

	if (host_len >= ULN_URL_MAX_HOST_LEN - 1) {
		return -1;
	}
	
	if (check_flag >= 2) {
		memcpy(p_url->host, p, host_len);
		p_url->host[host_len] = 0;
	}

	if ((check_flag & 1) && uln_check_host(p, host_len) == 0) {
		return -1;
	}

	if (NULL == q) {
		// host, done all
		p_url->path[0] = '/';
		p_url->path[1] = 0;
		return 0;
	} else if ('/' == *q) {
		// host/
		p = q;
	} else if (':' == *q) {
		// host:[port]
		q++;
		while (ULN_DIGIT_CHAR[(u_char) *q]) {
			if (p_url->port == -1) {
				p_url->port = *q - '0';
			} else {
				p_url->port = p_url->port * 10 + *q - '0';
				if (p_url->port > ULN_MAX_PORT_NUM) {
					// port out of range
					return -1;
				}
			}

			q++;
		}
		
		if (0 == *q) {
			// end of url, ie, host:port
			p_url->path[0] = '/';
			p_url->path[1] = 0;
			return 0;
		} else if ('/' == *q) {
			p = q;
		} else {
			// invalid
			return -1;
		}
	} else {
		// never happen
		return -1;
	}

	// path not empty
	// path?query#fragment
	q = find_path_end(p, &end);
	u_int path_len = 0;
	if (NULL == q) {
		// p .. 串尾: path
		path_len = (u_int) (end - p);
	} else {
		// path? | path#
		path_len = (u_int) (q - p);
	}

	if (path_len >= ULN_URL_MAX_PATH_LEN - 1) {
		return -1;
	}

	if (check_flag >= 2) {
		memcpy(p_url->path, p, path_len);
		p_url->path[path_len] = 0;
	}

	if ((check_flag & 1) && uln_check_path(p, path_len) == 0) {
		return -1;
	}

	if (q != NULL && '?' == *q) {
		// path?query[#fragment]
		const char* pf = strchr(q + 1, '#');
		u_int query_len = 0;
		if (NULL == pf) {
			// q + 1 .. 串尾 : query
			query_len = strlen(q + 1);
		} else {
			// q + 1 .. pf : query#
			query_len = (u_int) (pf - q - 1);
		}
		
		if (query_len >= ULN_URL_MAX_QUERY_LEN - 1) {
			// buffer overflow
			return -1;
		}
		
		if (check_flag >= 2) {
			memcpy(p_url->query, q + 1, query_len);
			p_url->query[query_len] = 0;
		}
		
		if ((check_flag & 1) && uln_check_query(q + 1, query_len) == 0) {
			return -1;
		}

		q = pf;
	}

	if (q != NULL && '#' == *q) {
		if (uln_strlcpy(p_url->fragment, q + 1, sizeof(p_url->fragment)) < 0) {
			return -1;
		}

		if ((check_flag & 1) && uln_check_fragment(p_url->fragment) == 0) {
			return -1;
		}
	}

	return 0;
}

static
int uln_is_ignore(const char* slip)
{
	if (NULL == slip) {
		return 0;
	}
	int head = 0;
	int tail = ULN_URL_IGNORE_COUNT - 1;
	int cur = 0;
	int ret = 0;
	while (head <= tail) {
		cur = (head + tail) / 2;
		ret = strncmp(slip, ul_ignore_list[cur].name,
				ul_ignore_list[cur].length);
		if (ret < 0) {   
			tail = cur - 1;
		} else if (ret > 0) {
			head = cur + 1;
		} else {
			return 1;
		}
	}
	return 0;
}


static 
const char* get_first_path_char(const char* url)
{
	const char* p = my_strchr(url, "?/");
	if (p == NULL) {
		return NULL;
	}

	if (p != url && ':' == *(p - 1) && '/' == *p) {
		// "/" in "http:/" is not path
		p++;
		if ('/' == *p) {
			p++;
		}
		return my_strchr(p, "?/");
	}
	return p;
}

static
int uln_repair_url(const char* url, char* buf, int buf_len)
{
	char temp_url[ULN_URL_MAX_URL_LEN];
	
	while (' ' == *url) {
		// trim leading spaces
		url++;
	}

	u_int url_len = strlen(url);

	while (url_len >= 1 && ' ' == url[url_len - 1]) {
		// trim trailing spaces
		url_len--;
	}

	// 字符转换
	int before_colon = 1;
	int cur_pos = 0;
	for (u_int i = 0; i < url_len; i++) {
		if (ULN_UNSAFE_CHAR[(u_char) url[i]]) {
			if (cur_pos + 3 >= ULN_URL_MAX_URL_LEN - 1) {
				return -1;
			}
			temp_url[cur_pos++] = '%';
			temp_url[cur_pos++] = ULN_DIG_TO_HEX[FIRST_HEX_CHAR(url[i])];
			temp_url[cur_pos++] = ULN_DIG_TO_HEX[SECOND_HEX_CHAR(url[i])];
		} else if (ULN_OMMIT_CHAR[(u_char) url[i]]) {
			// 忽略
		} else {
			if (cur_pos + 1 >= ULN_URL_MAX_URL_LEN - 1) {
				return -1;
			}

			if ('\\' == url[i] && before_colon) {
				temp_url[cur_pos++] = '/';
			} else {
				temp_url[cur_pos++] = url[i];
			}
			if (';' == url[i]|| '?' == url[i]) {
				before_colon = 0;
			}
		}
	}
	temp_url[cur_pos] = 0;
	
	if (0 == cur_pos) {
		// empty string after trimed
		return -1;
	}

	const char* curp = temp_url;
	int len = 0;
	const char* first_path_char = get_first_path_char(curp);

	// scheme:/ -> scheme://
	char* p = strchr(curp, ':');
	if (p != NULL && '/' == *(p + 1) && *(p + 2) != '/'
			&& (NULL == first_path_char || p < first_path_char)) {
		// ie, ":/", and ":/" is not in the path part
		if (len + p - curp + 3 >= buf_len - 1) {
			return -1;
		}
		memcpy(buf + len, curp, (u_int) (p - curp));
		len += p - curp;
		memcpy(buf + len, "://", 3);
		len += 3;
		curp = p + 2;
	} 

	if (NULL == first_path_char) {
		first_path_char = curp + strlen(curp);
		// ie, *first_path_char == 0
	}
	

	// note, first_path_char - 1 is guaranteed not to exceed the buffer
	if ('.' == *(first_path_char - 1)
			|| ':' == *(first_path_char - 1)) {
		// "host./" or "host:/", "." or ":" should be eleminated.
		const char* q = first_path_char - 1;
		while (q >= curp && ('.' == *q || ':' == *q)) {
			q--;
		}
		q++;
		if (len + q - curp >= buf_len - 1) {
			return -1;
		}

		memcpy(buf + len, curp, (u_int) (q - curp));
		len += q - curp;
		curp = first_path_char;
	}

	if ('?' == *first_path_char || 0 == *first_path_char) {
		// the path "/" is missing, add it
		if (len + first_path_char - curp + 1 >= buf_len - 1) {
			return -1;
		}

		memcpy(buf + len, curp, (u_int) (first_path_char - curp));
		len += first_path_char - curp;
		curp = first_path_char;
		*(buf + len) = '/';
		len++;
	}

	// append the remaining chars
	const char* end = curp + strlen(curp);
	if (len + end - curp >= buf_len - 1) {
		return -1;
	}
	memcpy(buf + len, curp, (u_int) (end - curp));
	len += end - curp;
	*(buf + len) = 0;
	return 0;
}

static
int uln_parse_url_rough(const char* url, uln_url_t* p_url)
{
	if (NULL == url || NULL == p_url) {
		return -1;
	}
	
	char temp_url[ULN_URL_MAX_URL_LEN];
	int ret = uln_repair_url(url, temp_url, ULN_URL_MAX_URL_LEN);
	if (ret < 0) {
		return -1;
	}

	// parse again
	return uln_parse_url_strict(temp_url, p_url, ULN_CHECK | ULN_COPY);
}

int uln_parse_url(const char* url, uln_url_t* p_url) 
{
	int ret = uln_parse_url_strict(url, p_url, ULN_CHECK | ULN_COPY);
	if (ret < 0) {
		ret = uln_parse_url_rough(url, p_url);
	}
	return ret;
}

int uln_normalize_scheme(char* scheme)
{
	if (NULL == scheme) {
		return -1;
	}
	
	while (*scheme) {
		*scheme = ULN_TO_LOWER[(u_char) *scheme];
		scheme++;
	}

	// success
	return 0;
}

int uln_normalize_userinfo(char* userinfo)
{
	if (NULL == userinfo) {
		return -1;
	}

	// nothing change
	return 0;
}

int uln_normalize_host(char* host)
{

	if (NULL == host) {
		return -1;
	}
	
	u_char* p = (u_char *)host;


	while(*host) {

		*p = (u_char) *host;

		if ('%' == *host) {
			u_char c = (u_char) *p;
			char d1 = ULN_HEX_TO_DIG[(u_char) *(host + 1)];
			char d2 = ULN_HEX_TO_DIG[(u_char) *(host + 2)];

			// check
			if ((0 == d1 && *(host + 1) != '0') ||
					(0 == d2 && *(host + 2) != '0')) {
				c = 0;
			} else {
				c = (char) HEX_TO_CHAR(d1, d2);
			}

			if (0 == c|| !ULN_HOST_CHAR[c] 
					|| '.' == c || '[' == c || ']' == c) {
				// 转换失败或转换后不是host的合法字符, 保留原貌
				c = *p;
			} else {
				host += 2;
			}

			*p = c;
		}

		*p = (u_char) ULN_TO_LOWER[*p];

		p++;
		host++;
	}

	*p = 0;

	return 0;
}

int uln_absolutize_path(char* path)
{
	if (NULL == path) {
		return -1;
	}

	if (*path != '/') {
		return -1;
	}

	char* p = path;
	char* start = path;

	path++;

	while (*path && *path != ';') {
		if (*p != '/') {
			p++;
			*p = *path;
			path++;
			continue;
		}

		/* // */
		if (strncmp(path, "/", 1) == 0) {
			path++;
			continue;
		}

		/* /./ */
		if (strncmp(path, "./", 2) == 0) {
			path += 2;
			continue;
		}

		/* /../ */
		if (strncmp(path, "../", 3) == 0) {
			path += 3;

			if (p != start) {
				p--;
			}

			while (*p != '/') {
				p--;
			}
			continue;
		}
		
		p++;
		*p = *path;
		path++;
	}

	// attach the remaining string
	while(*path) {
		p++;
		*p = *path;
		path++;
	}

	p++;
	*p = 0;

	return p - start;
}

/**
 * @brief %xy->char
 *
**/
static 
int uln_unescape_str(const char* str, char* buf, u_int buf_len)
{
	if (NULL == str || NULL == buf || 0 == buf_len) {
		return -1;
	}
	const u_char* q = (const u_char*) str;
	u_char* p = (u_char*) buf;
	u_int cur_len = 0;
	u_char c = 0;

	while (*q) {
		if (*q == '%' && *(q + 1) != '\0' && *(q + 2) != '\0') {
			// to char
			char d1 = ULN_HEX_TO_DIG[*(q + 1)];
			char d2 = ULN_HEX_TO_DIG[*(q + 2)];


			c = (char) HEX_TO_CHAR(d1, d2);

			if ((0 == d1 && *(q + 1) != '0') 
					|| (0 == d2 && *(q + 2) != '0')) {
				c = 0;
			} 


			if (c != 0 && ULN_SAFE_CHAR[c] != 0) {
				// must changed
				if (cur_len + 1 >= buf_len - 1) {
					return -1;
				}

				*p = c;
				q += 3;
				p++;
				cur_len++;
				continue;
			} else if (c != 0) {
				// 无需改变，但%xy应统一为大写
				if (cur_len + 3 >= buf_len - 1) {
					return -1;
				}

				*p = '%';
				*(p + 1) = (u_char) ULN_TO_UPPER[*(q + 1)];
				*(p + 2) = (u_char) ULN_TO_UPPER[*(q + 2)];

				q += 3;
				p += 3;
				cur_len += 3;
				continue;
			} else {
				// %xy中, x,y不合法，为提高回召率，保留
				if (cur_len +3 >= buf_len -1) {
					return -1;
				}
				*p = '%';
				*(p + 1) = *(q + 1);
				*(p + 2) = *(q + 2);
				q += 3;
				p += 3;
				cur_len += 3;
				continue;
			}
		}

		if (cur_len + 1 >= buf_len - 1) {
			return -1;
		}
		*p = *q;
		p++;
		q++;
		cur_len++;
	}

	if (cur_len >= buf_len) {
		return -1;
	}

	buf[cur_len] = 0;
	return (int) cur_len;
}


/**
 * @brief char->%xy.
 *
**/
static 
int uln_escape_str(const char* str, char* buf, u_int buf_len) 
{
	if (NULL == str || NULL == buf || 0 == buf_len) {
		return -1;
	}

	const u_char* q = (const u_char*) str;
	u_char* p = (u_char *) buf;
	u_int cur_len = 0;
	u_char c = 0;

	while (*q) {
		if ('%' == *q && *(q + 1) != '\0' && *(q + 2) != '\0') {
			// to char
			char d1 = ULN_HEX_TO_DIG[*(q + 1)];
			char d2 = ULN_HEX_TO_DIG[*(q + 2)];

			c = (char) HEX_TO_CHAR(d1, d2);

			if ((0 == d1 && *(q + 1) != '0') 
					|| (0 == d2 && *(q + 2) != '0')) {
				c = 0;
			} 
			
			if ((c != 0 && ULN_SAFE_CHAR[c] != 0 && 0 == ULN_ESCAPED_CHAR[c])) {
				// must changed
				if (cur_len + 1 >= buf_len - 1) {
					return -1;
				}

				*p = c;
				q += 3;
				p++;
				cur_len++;
				continue;
			} else if (c != 0) {
				// 无需改变，但%xy应统一为大写
				if (cur_len + 3 >= buf_len - 1) {
					return -1;
				}

				*p = '%';
				*(p + 1) = (u_char) ULN_TO_UPPER[*(q + 1)];
				*(p + 2) = (u_char) ULN_TO_UPPER[*(q + 2)];

				q += 3;
				p += 3;
				cur_len += 3;
				continue;
			} else {
				// %xy中, x,y不合法, 保留原状.
				if (cur_len + 3 >= buf_len - 1) {
					return 0;
				}

				*p = '%';
				*(p + 1) = *(q + 1);
				*(p + 2) = *(q + 2);
				q += 3;
				p += 3;
				cur_len += 3;
				continue;
			}
		} else if (ULN_ESCAPED_CHAR[*q]) {
			if (cur_len + 3 >= buf_len - 1) {
				return -1;
			}

			*p = '%';
			*(p + 1) = (u_char) ULN_DIG_TO_HEX[FIRST_HEX_CHAR(*q)];
			*(p + 2) = (u_char) ULN_DIG_TO_HEX[SECOND_HEX_CHAR(*q)];

			p += 3;
			q++;

			cur_len += 3;
			continue;
		} 

		if (cur_len + 1 >= buf_len - 1) {
			return -1;
		}
		*p = *q;
		p++;
		q++;
		cur_len++;
	}

	if (cur_len >= buf_len) {
		return -1;
	}

	buf[cur_len] = 0;
	return (int) cur_len;
}

int uln_normalize_path(char* path)
{
	char temp_path[ULN_URL_MAX_PATH_LEN];


	if (NULL == path) {
		return -1;
	}

	int ret = 0;
	if (g_uln_escaped_mode == ULN_TO_ESCAPED_MODE) {
		ret = uln_escape_str(path, temp_path, ULN_URL_MAX_PATH_LEN);
	} else if (g_uln_escaped_mode == ULN_UN_ESCAPED_MODE) {
		ret = uln_unescape_str(path, temp_path, ULN_URL_MAX_PATH_LEN);
	} else {
		// don't changed
		ret = uln_strlcpy(temp_path, path, ULN_URL_MAX_PATH_LEN);
	}

	if (ret < 0) {
		return -1;
	}

	ret = uln_absolutize_path(temp_path);
	if (ret < 0 || ret >= ULN_URL_MAX_PATH_LEN) {
		return -1;
	}

	memcpy(path, temp_path, (u_int) ret);
	path[ret] = 0;

	return 0;
}


int uln_normalize_query(char* query)
{
	if (g_uln_escaped_mode == ULN_UN_CHANGE_MODE) {
		return 0;
	}

	char temp_query[ULN_URL_MAX_QUERY_LEN];

	int ret = 0;
	if (g_uln_escaped_mode == ULN_TO_ESCAPED_MODE) {
		ret = uln_escape_str(query, temp_query, ULN_URL_MAX_QUERY_LEN);
	} else if (g_uln_escaped_mode == ULN_UN_ESCAPED_MODE) {
		ret = uln_unescape_str(query, temp_query, ULN_URL_MAX_QUERY_LEN);
	} 
	if (ret < 0 || ret >= ULN_URL_MAX_QUERY_LEN) {
		return -1;
	}

	memcpy(query, temp_query, (u_int) ret);
	query[ret] = 0;

	return 0;
}

int uln_normalize_fragment(char* fragment)
{	

	if (g_uln_escaped_mode == ULN_UN_CHANGE_MODE) {
		return 0;
	}

	char temp_fragment[ULN_URL_MAX_FRAGMENT_LEN];

	int ret = 0;
	if (g_uln_escaped_mode == ULN_TO_ESCAPED_MODE) {
		ret = uln_escape_str(fragment, temp_fragment, ULN_URL_MAX_FRAGMENT_LEN);
	} else if (g_uln_escaped_mode == ULN_UN_ESCAPED_MODE) {
		ret = uln_unescape_str(fragment, temp_fragment, ULN_URL_MAX_FRAGMENT_LEN);
	} 
	if (ret < 0 || ret >= ULN_URL_MAX_FRAGMENT_LEN) {
		return -1;
	}

	memcpy(fragment, temp_fragment, (u_int) ret);
	fragment[ret] = 0;

	return 0;
}

int uln_normalize_url(uln_url_t* p_url) 
{
	if (NULL == p_url) {
		return -1;
	}
	
	int ret = uln_normalize_scheme(p_url->scheme);
	if (ret < 0) {
		return -1;
	}

	ret = uln_normalize_userinfo(p_url->userinfo);
	if (ret < 0) {
		return -1;
	}

	ret = uln_normalize_host(p_url->host);
	if (ret < 0) {
		return -1;
	}


	ret = uln_normalize_path(p_url->path);
	if (ret < 0) {
		return -1;
	}
 
	ret = uln_normalize_query(p_url->query);
	if (ret < 0) {
		return -1;
	}

	ret = uln_normalize_fragment(p_url->fragment);
	if (ret < 0) {
		return -1;
	}

	// assembly changed
	if (strcasecmp(p_url->scheme, "http") == 0 || 0 == p_url->scheme[0]) {
		// HTTP scheme
		if (0 == g_uln_multi_schemes) {
			// http can be ommitted
			p_url->scheme[0] = 0;
		} else {

			memcpy(p_url->scheme, "http", 4);
			p_url->scheme[4] = 0;
		}

		if (ULN_HTTP_DEFAULT_PORT == p_url->port) {
			p_url->port = -1;
		}

		// fragment ommit
		p_url->fragment[0] = 0;
	}

	// success
	return 0;
}

int uln_check_url(const char* url)
{
	uln_url_t url_t;
	if (NULL == url) {
		return -1;
	}
	
	if (uln_parse_url_strict(url, &url_t, 1) == 0) {
		return 0;
	}
	
	char temp_url[ULN_URL_MAX_URL_LEN];
	if (uln_repair_url(url, temp_url, ULN_URL_MAX_URL_LEN) < 0) {
		return -1;
	}

	return uln_parse_url_strict(temp_url, &url_t, ULN_CHECK);
}

int uln_normalize_url(const char* url, char* buf, u_int buf_len)
{
	uln_url_t url_t;

	if (NULL == url || NULL == buf || 0 == buf_len) {
		return -1;
	}
	
	int ret = uln_parse_url(url, &url_t);
	if (ret < 0) {
		return -1;
	}

	ret = uln_normalize_url(&url_t);
	if (ret < 0) {
		return -1;
	}

	
	ret = uln_to_urlstr(&url_t, buf, buf_len);
	if (ret < 0) {
		return -1;
	}

	return 0;
}

int uln_to_urlstr(const uln_url_t* p_url, char* buf, u_int buf_len)
{
	if (NULL == p_url || NULL == buf || 0 == buf_len) {
		return -1;
	}

	u_int cur_len = 0;
	int ret = 0;

	// add scheme if exists
	if (p_url->scheme[0] != 0) {
		ret = uln_strlcpy(buf + cur_len, p_url->scheme, "://", buf_len - cur_len);
		if (ret < 0) {
			return -1;
		}
		cur_len += ret;
	}

	// add userinfo if exists
	if (p_url->userinfo[0] != 0) {
		ret = uln_strlcpy(buf + cur_len, p_url->userinfo, "@", buf_len - cur_len);
		if (ret < 0) {
			return -1;
		}
		cur_len += ret;
	}

	// add host
	ret = uln_strlcpy(buf + cur_len, p_url->host, buf_len - cur_len);
	if (ret < 0) {
		return -1;
	}
	cur_len += ret;

	// add port if exists
	if (p_url->port >= 0) {
		ret = snprintf(buf + cur_len, buf_len - 1 - cur_len, ":%d", p_url->port);
		if (ret < 0 || (u_int) ret >= buf_len - 1 - cur_len) {
			return -1;
		}
		cur_len += ret;
	}

	// add path
	if (p_url->path[0] != '/') {
		ret = uln_strlcpy(buf + cur_len, "/", p_url->path, buf_len - cur_len);
	} else {
		ret = uln_strlcpy(buf + cur_len, p_url->path, buf_len - cur_len);
	}
	if (ret < 0) {
		return -1;
	}
	cur_len += ret;


	// add query if exists
	if (p_url->query[0] != 0) {
		ret = uln_strlcpy(buf + cur_len, "?", p_url->query, buf_len - cur_len);
		if (ret < 0) {
			return -1;
		}
		cur_len += ret;
	}

	// add fragment if exists
	if (p_url->fragment[0] != 0) {
		ret = uln_strlcpy(buf + cur_len, "#", p_url->fragment, buf_len - cur_len);
		if (ret < 0) {
			return -1;
		}
		cur_len += ret;
	}

	buf[cur_len] = 0;

	return 0;
}

int uln_set_encode_mode(encode_mode_t encode_mode)
{
	if (encode_mode != 	ULN_TO_ESCAPED_MODE && 
			encode_mode != ULN_UN_ESCAPED_MODE &&
				encode_mode != ULN_UN_CHANGE_MODE) {
		return -1;
	}
	g_uln_escaped_mode = encode_mode;
	return 0;
}

int uln_set_multi_schemes(int is_multi_schemes) 
{
	if (is_multi_schemes != 0 && is_multi_schemes != 1) {
		return -1;
	}

	g_uln_multi_schemes = is_multi_schemes;
	return 0;
}

static 
int uln_get_static_path(const char* path, char* buf, u_int buf_len)
{
	if (NULL == path || NULL == buf || 0 == buf_len) {
		return -1;
	}

	u_int cur_len = 0;
	while (*path && *path != ';' && cur_len < buf_len - 1) {
		*buf = *path;
		buf++;
		path++;
		cur_len++;
	}

	if (cur_len >= buf_len - 1) {
		return -1;
	}
	*buf = 0;
	return 0;
}

static 
int uln_is_dotip(const char* host)
{
	if (NULL == host) {
		return 0;
	}

	in_addr l_inp;
	u_int ret = 0;

	ret = inet_aton(host, &l_inp);
	return ret != 0;
}

static 
int uln_is_ipv6(const char* host)
{
	if (NULL == host) {
		return 0;
	}

	return uln_check_ipv6_address(host, strlen(host));
}

static 
int uln_get_host_trunk(const char* host, char *buf, u_int buf_len)
{
	char* pfirst = NULL;
	char* ptail = NULL;
	u_int len = 0;
	
	if (NULL == host || NULL == buf || 0 == buf_len) {
		return -1;
	}

	if (0 == *host) {
		buf[0] = 0;
		return 0;
	}

	if (uln_is_dotip(host) || uln_is_ipv6(host)) {
		if (strlen(host) >= buf_len) {
			// buffer over flow
			return -1;
		}
		memcpy(buf, host, buf_len - 1);
		buf[buf_len - 1] = 0;
		return 0;
	}

	ptail = strrchr(host, '.');
	if (NULL == ptail) {
		// no dot
		return -1;
	}
	pfirst = ptail - 1;
	while (pfirst >= host - 1) {
		if ((pfirst < host)||('.' == *pfirst)) {
			if (uln_is_ignore(pfirst + 1) == 0) {
				len = (u_int) (ptail - pfirst - 1);

				if (len >= buf_len) {
					// buffer overflow
					return -1;
				}

				memcpy(buf, pfirst + 1, len);
				buf[len]=0;
				return 0; 
			} else {
				ptail = pfirst;
			}
		}
		pfirst--;
	}
	return -1;
}


static 
int uln_get_main_domain(const char* host, char* buf, u_int buf_len)
{
	if (NULL == host || NULL == buf || 0 == buf_len) {
		return -1;
	}
	
	char* pfirst = NULL;
    char* ptail  = NULL;
    u_int len = 0; 

    if (uln_is_dotip(host) == 1 || uln_is_ipv6(host) == 1) {
		if (strlen(host) >= buf_len) {
			// buffer overflow
			return -1;
		}
        memcpy(buf, host, buf_len - 1);
        buf[buf_len - 1] = 0;
        return 0;
    }    


    ptail = strrchr(host, '.');
    if (NULL == ptail) {    
        goto end; 
    }

    pfirst = ptail - 1;

    while (pfirst >= host - 1) {    
        if (pfirst == host - 1 || '.' == *pfirst) {
            if (uln_is_ignore(pfirst + 1) == 0) {

                len = (u_int) (ptail - pfirst - 1);

				if (len >= buf_len) {
					// buffer overflow
					return -1;
				}
                memcpy(buf, pfirst + 1, len);
                buf[len]=0;
                return 0;
            }
            else {
                ptail = pfirst;
            }
        }
        pfirst--;
    }

end:
	if (strlen(host) >= buf_len) {
		return -1;
	}
    memcpy(buf, host, buf_len - 1);
    buf[buf_len - 1]=0;
	return 0;
}



int uln_set_scheme(uln_url_t* p_url, const char* buf)
{
	if (NULL == p_url || NULL == buf) {
		return -1;
	}

	int ret = uln_check_scheme(buf);
	if (0 == ret) {
		return -1;
	}

	ret = uln_strlcpy(p_url->scheme, buf, sizeof(p_url->scheme));
	if (ret < 0) {
		return -1;
	}
	return 0;
}

int uln_set_userinfo(uln_url_t* p_url, const char* buf)
{
	if (NULL == p_url || NULL == buf) {
		return -1;
	}
	int ret = uln_check_userinfo(buf);
	if (0 == ret) {
		return -1;
	}

	if (strlen(buf) >= ULN_URL_MAX_USERINFO_LEN) {
		return -1;
	}

	ret = uln_strlcpy(p_url->userinfo, buf, sizeof(p_url->userinfo));
	if (ret < 0) {
		return -1;
	}
	return 0;
}

int uln_set_host(uln_url_t* p_url, const char* buf)
{
	if (NULL == p_url || NULL == buf) {
		return -1;
	}

	int ret = uln_check_host(buf, strlen(buf));
	if (1 == ret) {
		ret = uln_strlcpy(p_url->host, buf, sizeof(p_url->host));
		if (ret < 0) {
			return -1;
		}
		return 0;
	} else {
		// strict check failed, try looser constraint
		char temp_url[ULN_URL_MAX_URL_LEN];
		uln_url_t url;
		ret = snprintf(temp_url, ULN_URL_MAX_URL_LEN, "%s://%s@%s%s", ULN_STD_SCHEME, 
				ULN_STD_USERINFO, buf, ULN_STD_PATH);
		if (ret < 0 || ret >= ULN_URL_MAX_URL_LEN) {
			return -1;
		}
		ret = uln_parse_url_rough(temp_url, &url);
		if (ret < 0) {
			return -1;
		}

		ret = uln_strlcpy(p_url->host, url.host, sizeof(p_url->host));
		if (ret < 0) {
			return -1;
		}
		return 0;
	}
}

int uln_set_port(uln_url_t* p_url, const char* buf)
{
	if (NULL == p_url || NULL == buf) {
		return -1;
	}
	
	if (0 == buf[0]) {// empty, send default
		p_url->port = -1;
		return 0;
	}

	char* end = NULL;
	int port = strtol(buf, &end, 10);
	if (end != NULL && 0 == *end) {
		if (port <= 0 || port > ULN_MAX_PORT_NUM) {
			// out of range
			return -1;
		}
		// success
		p_url->port = port;
		return 0;
	} else {
		return -1;
	}
}

int uln_set_path(uln_url_t* p_url, const char* buf)
{
	if (NULL == p_url || NULL == buf) {
		return -1;
	}

	int ret = uln_check_path(buf, strlen(buf));
	if (ret != 0) {
		ret = uln_strlcpy(p_url->path, buf, sizeof(p_url->path));
		if (ret < 0) {
			return -1;
		}
		return 0;
	} else {
		// strict check failed, loose the constrait
		char temp_url[ULN_URL_MAX_URL_LEN];
		ret = snprintf(temp_url, ULN_URL_MAX_URL_LEN, "%s://%s%s%s", 
				ULN_STD_SCHEME, ULN_STD_HOST, '/' == *buf ? "" : "/", buf);
		if (ret < 0 || ret >= ULN_URL_MAX_URL_LEN) {
			return -1;
		}
	
		uln_url_t url;
		ret = uln_parse_url_rough(temp_url, &url);
		if (ret < 0) {
			return -1;
		}

		ret = uln_strlcpy(p_url->path, url.path, sizeof(p_url->path));
		if (ret < 0) {
			return -1;
		}
		return 0;
	}
}

int uln_set_file(uln_url_t* p_url, const char* buf)
{
	if (NULL == p_url || NULL == buf) {
		return -1;
	}

	char temp_url[ULN_URL_MAX_URL_LEN];
	uln_url_t url;

	int ret = snprintf(temp_url, ULN_URL_MAX_URL_LEN, "%s://%s%s%s", 
				ULN_STD_SCHEME, ULN_STD_HOST, '/' == *buf ? "" : "/", buf);
	if (ret < 0 || ret >= ULN_URL_MAX_URL_LEN) {
		return -1;
	}
	
	ret = uln_parse_url(temp_url, &url);
	if (ret < 0) {
		return -1;
	}

	ret = uln_strlcpy(p_url->path, url.path, sizeof(p_url->path));
	if (ret < 0) {
		return -1;
	}

	ret = uln_strlcpy(p_url->query, url.query, sizeof(p_url->query));
	if (ret < 0) {
		return -1;
	}
	return 0;
}


int uln_set_query(uln_url_t* p_url, const char* buf)
{
	if (NULL == p_url || NULL == buf) {
		return -1;
	}

	int ret = uln_check_query(buf, strlen(buf));
	if (ret != 0) {
		// strict check success
		ret = uln_strlcpy(p_url->query, buf, sizeof(p_url->query));
		if (ret < 0) {
			return -1;
		}
		return 0;
	} else {
		// strict check failed, loose the constrait
		char temp_url[ULN_URL_MAX_URL_LEN];
		ret = snprintf(temp_url, ULN_URL_MAX_URL_LEN, "%s/%s?%s", ULN_STD_HOST, ULN_STD_PATH, buf);
		if (ret < 0 || ret >= ULN_URL_MAX_URL_LEN) {
			return -1;
		}

		uln_url_t url;
		ret = uln_parse_url_rough(temp_url, &url);
		if (ret < 0) {
			return -1;
		}

		ret = uln_strlcpy(p_url->query, url.query, sizeof(p_url->query));
		if (ret < 0) {
			return -1;
		}
		return 0;
	}
}

int uln_set_fragment(uln_url_t* p_url, const char* buf)
{
	if (NULL == p_url || NULL == buf) {
		return -1;
	}

	int ret = uln_check_fragment(buf);
	if (ret != 0) {
		ret = uln_strlcpy(p_url->fragment, buf, sizeof(p_url->fragment));
		if (ret < 0) {
			return -1;
		}
		return 0;
	} else {
		// strict check failed, loose the constrait
		char temp_url[ULN_URL_MAX_URL_LEN];
		ret = snprintf(temp_url, ULN_URL_MAX_URL_LEN, "%s/%s#%s", ULN_STD_HOST, ULN_STD_PATH, buf);
		if (ret < 0 || ret >= ULN_URL_MAX_URL_LEN) {
			return -1;
		}

		uln_url_t url;
		ret = uln_parse_url_rough(temp_url, &url);
		if (ret < 0) {
			return -1;
		}

		ret = uln_strlcpy(p_url->fragment, url.fragment, sizeof(p_url->fragment));
		if (ret < 0) {
			return -1;
		}
		return 0;
	}
}


int uln_set_url_segment(uln_url_t* p_url, segment_type_t segment_type, const char* buf)
{
	if (NULL == p_url || NULL == buf) {
		return -1;
	}
	int ret = 0;
	switch (segment_type) {
		case SEG_URL_SCHEME:
			ret = uln_set_scheme(p_url, buf);
			break;
		case SEG_URL_USERINFO:
			ret = uln_set_userinfo(p_url, buf);
			break;
		case SEG_URL_HOST:
			ret = uln_set_host(p_url, buf);
			break;
		case SEG_URL_PORT:
			ret = uln_set_port(p_url, buf);
			break;
		case SEG_URL_PATH:
			ret = uln_set_path(p_url, buf);
			break;
		case SEG_URL_FILE:
			ret = uln_set_file(p_url, buf);
			break;
		case SEG_URL_QUERY:
			ret = uln_set_query(p_url, buf);
			break;
		case SEG_URL_FRAGMENT:
			ret = uln_set_fragment(p_url, buf);
			break;
		default:
			// unreconized
			return -1;
	}

	return ret;
}

int uln_get_url_segment(const uln_url_t* p_url, segment_type_t segment_type, char* buf, u_int buf_len)
{
	if (NULL == p_url || NULL == buf || 0 == buf_len) {
		return -1;
	}

	int ret = 0;

	switch (segment_type) {
		case SEG_URL_SCHEME:
			ret = uln_strlcpy(buf, p_url->scheme, buf_len);
			break;
		case SEG_URL_USERINFO:
			ret = uln_strlcpy(buf, p_url->userinfo, buf_len);
			break;
		case SEG_URL_HOST:
			ret = uln_strlcpy(buf, p_url->host, buf_len);
			break;
		case SEG_URL_PORT:
			if (p_url->port != -1) {
				ret = snprintf(buf, buf_len, "%d", p_url->port);
			} else {
				*buf = 0;
				ret = 0;
			}
			break;
		case SEG_URL_PATH:
			ret = uln_strlcpy(buf, p_url->path, buf_len);
			break;
		case SEG_URL_QUERY:
			ret = uln_strlcpy(buf, p_url->query, buf_len);
			break;
		case SEG_URL_FRAGMENT:
			ret = uln_strlcpy(buf, p_url->fragment, buf_len);
			break;
		case SEG_URL_FILE: // path?query
			if (p_url->query[0] != 0) {
				ret = snprintf(buf, buf_len, "%s?%s", p_url->path, p_url->query);
			} else {
				ret = uln_strlcpy(buf, p_url->path, buf_len);
			}
			break;
		case SEG_URL_STATIC_PATH:// path without params
			ret = uln_get_static_path(p_url->path, buf, buf_len);
			break;
		case SEG_URL_MAIN_DOMAIN:
			ret = uln_get_main_domain(p_url->host, buf, buf_len);
			break;
		case SEG_URL_HOST_TRUNK:
			ret = uln_get_host_trunk(p_url->host, buf, buf_len);
			break;
		default:
			// unreconized
			ret = -1;
	}

	if (ret < 0 || (u_int) ret >= buf_len) {
		ret = -1;
	} else {
		ret = 0;
	}
	return ret;
}


int uln_is_dyn_url(const uln_url_t* p_url)
{
	if (NULL == p_url) {
		return -1;
	}
	return p_url->query[0] != 0 
			|| strchr(p_url->path, ';') != NULL 
				|| strchr(p_url->path, '=') != NULL;
}

int uln_is_abs_path(const char* url)
{
	if (NULL == url) {
		return 0;
	}
	return '/' == *url || '\\' == *url;
}

int uln_is_url(const char* url)
{
	if (NULL == url || 0 == *url) {
		return 0;
	}

	if (strstr(url, ":/") != NULL) {
		return 1;
	}

	return 0;
}


int uln_is_rel_path(const char* url)
{

	const char* ignore_prefix[20] = {
		"javascript:", "mailto:", "about:", NULL
	};

	if (NULL == url || 0 == *url) {
		return 0;
	}

	if (uln_is_url(url) == 1) {
		return 0;
	}

	if (uln_is_abs_path(url) == 1) {
		return 0;
	}

	for (int i = 0; ignore_prefix[i] != NULL; i++) {
		if (strncasecmp(url, ignore_prefix[i], 
					strlen(ignore_prefix[i])) == 0) {
			return 0;
		}
	}
	return 1;
}



int uln_url_clone(const uln_url_t* src, uln_url_t* des)
{
	if (NULL == src || NULL == des) {
		return -1;
	}
	
	if (uln_strlcpy(des->scheme, src->scheme, sizeof(des->scheme)) < 0) {
		return -1;
	}
	if (uln_strlcpy(des->userinfo, src->userinfo, sizeof(des->userinfo)) < 0) {
		return -1;
	}
	if (uln_strlcpy(des->host, src->host, sizeof(des->host)) < 0) {
		return -1;
	}
	des->port = src->port;
	if (uln_strlcpy(des->path, src->path, sizeof(des->path)) < 0) {
		return -1;
	}
	if (uln_strlcpy(des->query, src->query, sizeof(des->query)) < 0) {
		return -1;
	}
	if (uln_strlcpy(des->fragment, src->fragment, sizeof(des->fragment)) < 0) {
		return -1;
	}
	return 0;
}

int uln_resolve_url(const uln_url_t* p_base_url, const char* child_url, uln_url_t* p_resolved_url)
{
	if (NULL == p_base_url || NULL == child_url || NULL == p_resolved_url) {
		return -1;
	}
	
	int ret = 0;

	if (uln_is_url(child_url) == 1) {
		ret = uln_parse_url(child_url, p_resolved_url);
		if (ret < 0) {
			return -1;
		}
		ret = uln_normalize_url(p_resolved_url);
		return ret;
	} else if (uln_is_rel_path(child_url)) {
		char temp_path[ULN_URL_MAX_PATH_LEN];
		ret = uln_url_clone(p_base_url, p_resolved_url);
		if (ret < 0) {
			return -1;
		}

		char* p = strrchr(p_base_url->path, '/');
		u_int len = 0;
		if (NULL == p) {
			// impossible happen, because path in the structure always begin with '/'
			// in case that happen, add '/'
			temp_path[0] = '/';
			temp_path[1] = 0;
			len = 1;
		} else {
			len = (u_int) (p - p_base_url->path + 1);
			if (len >= ULN_URL_MAX_PATH_LEN) {
				return -1;
			}

			memcpy(temp_path, p_base_url->path, len);
			temp_path[len] = 0;
		}
		// strcat the child to the path
		ret = uln_strlcpy(temp_path + len, child_url, ULN_URL_MAX_PATH_LEN - len);
		if (ret < 0) {
			return -1;
		}

		// add the path
		ret = uln_set_url_segment(p_resolved_url, SEG_URL_FILE, temp_path);
		if (ret < 0) {
			return -1;
		}

		ret = uln_set_url_segment(p_resolved_url, SEG_URL_FRAGMENT, "");
		if (ret < 0) {
			return -1;
		}

		ret = uln_normalize_url(p_resolved_url);
		return ret;


	} else if (uln_is_abs_path(child_url)) {
		ret = uln_url_clone(p_base_url, p_resolved_url);
		if (ret < 0) {
			return -1;
		}
		
		ret = uln_set_url_segment(p_resolved_url, SEG_URL_FILE, child_url);
		if (ret < 0) {
			return -1;
		}

		ret = uln_set_url_segment(p_resolved_url, SEG_URL_FRAGMENT, "");
		if (ret < 0) {
			return -1;
		}

		ret = uln_normalize_url(p_resolved_url);
		if (ret < 0) {
			return -1;
		}
	} else {
		// invalid child_url
		return -1;
	}

	return 0;
}

int uln_resolve_url(const uln_url_t* p_base_url, const char* child_url, char* buf, u_int buf_len)
{
	if (NULL == p_base_url || NULL == child_url || NULL == buf || 0 == buf_len) {
		return -1;
	}

	uln_url_t resolved_url;
	int ret = uln_resolve_url(p_base_url, child_url, &resolved_url);
	if (ret < 0) {
		return -1;
	}

	return uln_to_urlstr(&resolved_url, buf, buf_len);
}

int uln_is_dyn_url(const char* url) {
	if (NULL == url) {
		return -1;
	}

	uln_url_t url_t;
	int ret = uln_parse_url(url, &url_t);
	if (ret < 0) {
		return -1;
	}

	return uln_is_dyn_url(&url_t);
}

int uln_get_url_segment(const char* url, segment_type_t segment_type, char* buf, u_int buf_len)
{
	if (NULL == url || NULL == buf || 0 == buf_len) {
		return -1;
	}

	uln_url_t url_t;
	int ret = uln_parse_url(url, &url_t);
	if (ret < 0) {
		return -1;
	}

	return uln_get_url_segment(&url_t, segment_type, buf, buf_len);
}

int uln_parse_url_without_check(const char* url, uln_url_t* p_url) 
{
	return uln_parse_url_strict(url, p_url, ULN_COPY);
}

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
