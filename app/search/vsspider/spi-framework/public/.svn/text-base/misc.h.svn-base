/**
 * $Id: misc.h,v 1.24 2009/06/29 11:05:59 zhuhk Exp $
 */

#ifndef	_MISC_H__
#define	_MISC_H__

#include <stdint.h>
#include <ctype.h>

// logfile

#if defined (WITHOUT_ULLIB)
// ...

#else
#include "ul_log.h"

#define WARNING(fmt, msg ...) \
	do { \
		ul_writelog (UL_LOG_WARNING, "(%s:%d,%s) " fmt, __FILE__, __LINE__,__FUNCTION__, ##msg); \
	} while (0)

#define FATAL(fmt, msg ...) \
	do { \
		ul_writelog (UL_LOG_FATAL, "(%s:%d,%s) " fmt, __FILE__, __LINE__,__FUNCTION__, ##msg); \
	} while (0)

#define NOTICE(fmt, msg ...) \
	do { \
		ul_writelog (UL_LOG_NOTICE, "(%s:%d,%s) " fmt,__FILE__, __LINE__,__FUNCTION__, ##msg); \
	} while (0)

#define TRACE(fmt, msg ...) \
	do { \
		ul_writelog (UL_LOG_TRACE, "(%s:%d,%s) " fmt,__FILE__, __LINE__,__FUNCTION__, ##msg); \
	} while (0)

#endif				/* ~ WITHOUT_ULLIB */

#ifdef __cplusplus
	extern "C"{
#endif

#define CONST_STR_LEN(x) (x), ((x) ? sizeof(x)-1 : 0)

// misc.misc
#ifndef _GLIBCXX_ALGORITHM
#ifndef min
#define min(a,b) ((a) > (b)?(b):(a))
#endif	/* ~ min */

#ifndef max
#define max(a,b) ((a) < (b)?(b):(a))
#endif	/* ~ max */
#endif

#ifndef	xfree
#define xfree(ptr) do { if ((ptr)) { free(ptr); (ptr) = NULL; }} while (0)
#endif	/* ~ xfree */

#ifdef __cplusplus
#define BEGIN_EXTERN_C       extern "C" {
#define END_EXTERN_C         }
#else
#define BEGIN_EXTERN_C
#define END_EXTERN_C
#endif

void foo(void);

// misc.string

uint64_t sign64(const char *src, size_t len);
uint32_t sign32(const char *src, size_t len);

#define strlcpy(dst, src, size) (snprintf ((dst), (size), "%s", (src)))

#define trim(start) \
do { \
    while (isspace(*start)) \
	start++; \
    if (*start == '\0') \
	break; \
 \
    char *stop = start + strlen(start) - 1; \
    while (stop >= start && isspace(*stop)) \
	*stop-- = '\0'; \
} while (0)

#define linkbreak(x, ey, stream) \
	do { if ((x) % (ey) == (ey) -1) fputc ('\n', (stream)); } while (0)

/**
 * cp src to dest (atmost nbytes, include tail NUL-byte)
 * and all 'A' - 'Z' will be tolower (case).
 *     all 'a' - 'z' will be toupper (Case).
 *
 * return last-byte ('\0') in dest;
 */

char *strncasecpy(char *dest, const char *src, size_t n);
char *strnCasecpy(char *dest, const char *src, size_t n);

const char *skip_protocol(const char *filename);
const char *skip_domain(const char *filename);
int domain_equal(const char *p1, const char *p2);
int tokenlize(char *cmdline, char **tokens, int maxsize);

// misc.TIME

#ifndef TIMEVAL2MS
#define TIMEVAL2MS(tv) \
    ((tv).tv_sec * 1000 + (tv).tv_usec / 1000)
#endif  /* ~ TIMEVAL2MS */

time_t PR_Now ();

char *tmstamp(char *prefix, size_t maxlen, const char *fmt = NULL);
char *tmdescribe(int sec, char *buf = NULL, size_t maxlen = 0);

int Nanosleep(int milidelay);

// wrap for sleep. 
// if(*stat!=wait_value),return in less than 5 seconds; or return in 'seconds' 
extern int Sleep(int seconds, int *stat, int wait_value);

// misc.HASH
int find_buckets_size(int scale);
int new_socket(char *hostname, int port, int timeout, int initdelay, int maxdelay);
int reconnet(char *hostname, int port);

int isValidID(int id);

int64_t GetLinkID(const char *pLink);

typedef void *
(pThread_t)(void *);

// misc.test

#define TestOK(section, ok) \
	do { \
		fprintf (stdout, "Test \"%s\", %s\n", (section), (ok)?"ok":"failed"); \
	} while (0)


//added by zhuhk@2009-05-07
//将一个字符串使用spliter中的字符切分为最多max块,每块的的索引在array中保存，返回值是切出的快数
extern int str_split(char *str, char *spliter, char **array, int max);
//去掉开头和结尾的空白符
extern char *str_trim(char *str);
//从文件中读取一行,删除行首、行尾的空白符,跳过空白行,返回字符串
extern char * read_line(char *line_buffer,int buf_len, FILE *fp);

//added by zhuhk@2009-06-24
#define SPI_MAX_URL_LEN 2048
//eg: schema://userinfo@host:port(/?#)path
typedef struct{
	int port;
	char *schema;
	char *usrpwd;
	char *site;
	char *path;
	char buf[SPI_MAX_URL_LEN+10];
}spi_url_t;
#define SPI_URL_INITIALIZER {"","",""}
#define SPI_URL_INIT(spi_url) \
	do{\
		(spi_url)->schema = "http";\
		(spi_url)->usrpwd = NULL;\
		(spi_url)->site = "";\
		(spi_url)->port = 80;\
		(spi_url)->path = "/";\
		(spi_url)->buf[0] = 0;\
	}while(0)
//Return 0 on succes, -1 on any error
extern int spi_url_parse(const char *url,spi_url_t *spi_url);
extern int spi_url_sprint(spi_url_t *url,char *buf,int buf_size);
extern int spi_url_print(spi_url_t *url);

#ifdef __cplusplus
}
#endif

#endif				/* ~ _MISC_H__ */
