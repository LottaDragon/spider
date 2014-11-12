/**
 * $Id: misc.cpp,v 1.21 2009/07/01 08:59:49 zhuhk Exp $
 */

#include <stdio.h>
#include <stdlib.h>

#include "misc.h"

void foo(void) {
}

// add more ...

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <openssl/md5.h>

#include "ul_net.h"
#include "misc.h"

// logfile

// misc.network

/**
 * return value : * socket: the connected socket with server
 */

int reconnet(char *hostname, int port) {
    int socket = -1;

    if (port > 0) {
        while (socket < 0) {
            socket = ul_tcpconnecto(hostname, port, 5);
            if (socket < 0) {
                WARNING("connect \"%s:%d\" failed, %m", hostname, port);

                struct timespec s_time;
                s_time.tv_sec = 5;
                s_time.tv_nsec = 0;

                nanosleep(&s_time, NULL);
            }
        }
    }

    return socket;
}

// misc.misc

unsigned char _sch_tolower[256] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
    64,

    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',

    91, 92, 93, 94, 95, 96,

    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',

    123, 124, 125, 126, 127,

    128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
    144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
    160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
    176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,

    192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207,
    208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
    224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255,
};

// # misc.string

/**
 * why not USE ul_dict.creat_sign_mds64 ().
 *
 * because this system use sign to store/retrieve infomation,
 * if creat_sign_mds64 is upgraded, this system will be UNUSABLE.
 *
 * so use our own implemented sign functions.
 */

static int create_sign2(const char *src, size_t len, uint32_t * sign1, uint32_t * sign2) {
    uint32_t md5sum[32];	// 16 is ok, but more space for assurance

    MD5((const unsigned char *) src, len, (unsigned char *) md5sum);

    *sign1 = md5sum[0] + md5sum[2];
    *sign2 = md5sum[1] + md5sum[3];	// diff from creat_sign_mds64.

    return 0;
}

/* NOTICE, GetLinkID != sign64 */
uint64_t sign64(const char *src, size_t len) {
    uint32_t sign1, sign2;

    // creat_sign_mds64 ((char *) src, len, &sign1, &sign2);
    create_sign2(src, len, &sign1, &sign2);

    return (((uint64_t) sign1 & 0xFFFFFFFF) | ((uint64_t) sign2 << 32));
}

uint32_t sign32(const char *src, size_t len) {
    uint32_t sign1, sign2;
    create_sign2(src, len, &sign1, &sign2);

    return sign1 + sign2;
}

const char *skip_protocol(const char *filename) {
    int i = 0;

    if (filename == NULL)
        return filename;

#define MAX_TESTED_CHARACTER 10

    while (i < MAX_TESTED_CHARACTER) {
        if (filename[i] == '\0')
            return filename;
        if (filename[i] == ':')
            break;
        else
            i++;
    }

    if (i < MAX_TESTED_CHARACTER) {
        if (filename[i + 1] != '/')
            return filename;
        if (filename[i + 2] != '/')
            return filename;
        else
            return filename + i + 3;
    }

    return filename;
}

const char *skip_domain(const char *filename) {
    const char *p = skip_protocol(filename);

    if (p && (p = strchr(p, '/')) != NULL)
        return p;
    else
        return p;
}

int domain_equal(const char *p1, const char *p2) {
    if (!p1 || !p2)
        return 0;
    p1 = skip_protocol(p1);
    p2 = skip_protocol(p2);

    int d1 = 0, d2 = 0;
    char *p0;
    if ((p0 = strchr(p1, '/')) != NULL)
        d1 = p0 - p1;
    else
        d1 = strlen(p1);

    if ((p0 = strchr(p2, '/')) != NULL)
        d2 = p0 - p2;
    else
        d2 = strlen(p2);

    if (d1 == d2 && strncasecmp(p1, p2, d1) == 0)
        return 1;
    else
        return 0;
}

char *strncasecpy(char *dest, const char *src, size_t n) {
    while (*src && n) {
        if (--n) {
            if (*src >= 'A' && *src <= 'Z')
                *dest = ((*src) | 0x20);
            else
                *dest = *src;

            dest++;
            src++;
        } else
            break;
    }

    *dest = '\0';
    return dest;
}

char *strnCasecpy(char *dest, const char *src, size_t n) {
    while (*src && n) {
        if (--n) {
            if (*src >= 'a' && *src <= 'z')
                *dest = ((*src) & (~0x20));
            else
                *dest = *src;

            dest++;
            src++;
        } else
            break;
    }

    *dest = '\0';
    return dest;
}

int tokenlize(char *cmdline, char **tokens, int maxsize) {
    if (tokens == NULL || maxsize <= 0)
        return 0;
    if (cmdline == NULL) {
        *tokens = NULL;
        return 0;
    }

    int i = 0;
    while (i < maxsize - 1) {
        while (isspace(*cmdline))
            cmdline++;
        if (*cmdline == '\0')
            break;

        tokens[i++] = cmdline;
        while (!isspace(*cmdline) && *cmdline != '\0')
            cmdline++;

        if (*cmdline == '\0')
            break;
        else
            *cmdline++ = '\0';
    }

    tokens[i] = NULL;
    return i;
}

// misc.TIME

char *tmstamp(char *prefix, size_t maxlen, const char *fmt) {
    time_t now;
    struct tm tm;

    time(&now);
    localtime_r(&now, &tm);

    size_t plen = strlen(prefix);
    char *p0 = prefix + plen;
    plen = ((maxlen > plen) ? (maxlen - plen) : 0);

    strftime(p0, plen, (fmt ? fmt : ".%Y%m%d-%H%M%S"), &tm);
    return prefix;
}

char *tmdescribe(int sec, char *buf, size_t maxlen) {
    static char tmpbuf[LINE_MAX];

    if (!buf) {
        buf = tmpbuf;
        maxlen = sizeof(tmpbuf);
    }

    char *p0 = buf;
    size_t remain = maxlen;
    int r;

    if (sec == 0) {
        r = snprintf(p0, remain, "0s");
        return buf;
    } else if (sec < 0) {
        r = snprintf(p0, remain, "-");
        if (r == -1 || r >= (int) remain)
            return buf;
        else {
            p0 += r;
            remain -= r;
            sec *= -1;
        }
    }
#define	minute 60
#define	hour 60 * minute
#define	day 24 * hour
#define	week 7 * day
#define	year 52 * week

#define __tmdescribe_pad(fmt, duration) \
do { \
	if (sec >= duration) { \
		r = snprintf (p0, remain, fmt, sec / (duration)); \
		if (r == -1 || r >= (int) remain) return buf; \
		else { p0 += r; remain -= r; sec %= (duration); } \
	} \
} while (0)

    __tmdescribe_pad("%dy", year);
    __tmdescribe_pad("%dw", week);
    __tmdescribe_pad("%dd", day);
    __tmdescribe_pad("%dh", hour);
    __tmdescribe_pad("%dm", minute);
    __tmdescribe_pad("%ds", 1);

#undef	__tmdescribe_pad
#undef	year
#undef	week
#undef	day
#undef	hour
#undef	minute

    return buf;
}

int Nanosleep(int milidelay) {
    struct timespec s_time;

    if (milidelay <= 0) return 0;

    s_time.tv_sec = milidelay / 1000;
    s_time.tv_nsec = (milidelay % 1000) * 1000000;

    return nanosleep(&s_time, NULL);
}

int new_socket(char *hostname, int port, int timeout, int initdelay, int maxdelay) {
    int sock, retrycount = 0;

    while ((sock = ul_tcpconnecto_ms(hostname, port, timeout)) == -1) {
        WARNING("connect to {\"%s:%d\"} failed, %m", hostname, port);

        // retry-connect, wait {maxdelay, initdelay}
        int delay = (initdelay << retrycount);

        if (delay > maxdelay) {
            delay = maxdelay;
        } else {
            retrycount++;
        }

        // wait <delay> micro-seconds.
        Nanosleep(delay);
    }

    return sock;
}

time_t PR_Now () {
    struct timeval tv;
    gettimeofday (&tv, 0);

    return TIMEVAL2MS(tv);
}

// misc.HASH

int find_buckets_size(int scale) {
    if (scale < 13)
        return 13;

    // we may initial a static prime number array, and lookup.

    // dummy loopup ...
    while (scale % 2 == 0 || scale % 3 == 0 || scale % 5 == 0 || scale % 7 == 0 || scale % 11 == 0)
        scale++;

    return scale;
}

int isValidID(int id)
{
    if (id == 0) return 0;

    int Low3 = ((id >> 24) ^ (id >> 16));
    int Low4 = ((id >> 24) + (id >> 16));

    if (((id >> 8) & 0xFF) == (Low3 & 0xFF)
	    && (id & 0xFF) == (Low4 & 0xFF))
	return 1;
    else return 0;
}

/* NOTICE, GetLinkID != sign64 */
int64_t GetLinkID(const char *pLink)
{
    MD5_CTX md5ctx;
    int64_t md5sum[3];

    MD5_Init(&md5ctx);
    if (pLink)
	MD5_Update(&md5ctx, pLink, strlen(pLink));
    MD5_Final((unsigned char *) md5sum, &md5ctx);

    return md5sum[0] + md5sum[1];
}

extern int Sleep(int seconds, int *stat, int wait_value){
	int cnt;
	
	if(seconds<=0){
		return 0;
	}else if(seconds<=5){
		sleep(5);
	}else{
		cnt = 0;
		while(*stat==wait_value && cnt<seconds){
			sleep(5);
			cnt += 5;
		}
	}
	return 0;
}

//added by zhuhk@2009-05-07
extern int str_split(char *str, char *spliter, char **array, int max){
	int i;
	char *pch;
	char s[50];
	
	if(str==NULL){
		return 0;	
	}
	
	//将\r\n\t等转义字符 转换为 原始字符
	snprintf(s,sizeof(s),"%s",spliter?spliter:"");
		
	i=0;
	array[i++] = str;
	while(str&&*str&&i<max){
		for(pch=s;pch&&*pch;pch++){
			if(*str==*pch){
				*str = 0;
				array[i++] = str+1;
				break;
			}
		}
		str++;
	}
	//删除行尾的\r\n
	if(i>0){
		pch = array[i-1]+strlen(array[i-1]);
		pch--;
		while(pch>=array[i-1]){
			if(*pch=='\r'||*pch=='\n'){
				*(pch--) = 0;
			}else{
				break;
			}
		}
	}
	return i;
}
//删除一个字符串开头及结尾的空白符,str内容将被修改;删除空白符后的字符串长度为0或str==NULL,返回NULL
//其它情况返回不含空白符的字符串
extern char *str_trim(char *str)
{
	int len;
	
	if(str == NULL){
		return NULL;
	}	
	len = strlen(str);
	//ignore the leading space
	while((len>=0)&&(isspace(*str))){
		str++;
		len--;
	}
	//ignore the tailing space
	while(len > 0){
		if(isspace(str[len-1])){
			str[len-1] = '\0';
			len--;
		}else{
			break;
		}
	}
	if(len == 0)
		return NULL;
	
	return str;	
}

//return:copy的byte数;
static int strsubcpy(char *dst,char *start,char *end,int max){
	int i;
	
	if(dst==NULL||start==NULL||max<1){
		return 0;
	}
	for(i=0;start<=end&&i<max-1;i++){
		*(dst++) = *(start++);
	}
	*dst = 0;
	return i;
}


//从文件中读取一行,删除行首、行尾的空白符,跳过空白行,返回字符串
extern char * read_line(char *line_buffer,int buf_len, FILE *fp)
{
	char *pch;
	
	if(line_buffer==NULL || fp==NULL)
		return NULL;

	while(fgets(line_buffer,buf_len,fp) != NULL){
		pch = str_trim(line_buffer);
		if( pch != NULL ){
			return pch;
		}
	}
	return NULL;
}

extern int spi_url_parse(const char *url,spi_url_t *spi_url){
	char *pch,*poff;
	int off=0;
	
	if(spi_url){
		SPI_URL_INIT(spi_url);	
	}
	
	if(spi_url==NULL||url==NULL){
		WARNING("spi_url=%p,url=%p",spi_url,url);
		return -1;	
	}

	if(strlen(url) >= SPI_MAX_URL_LEN){
		WARNING("UrlTooLong. strlen(url)=%lu,SPI_MAX_URL_LEN=%d.url=%s",strlen(url),SPI_MAX_URL_LEN,url);
	}
	
	poff = (char *)url;
	
	//get schema, poff will be behind "://"
	spi_url->schema = spi_url->buf+off;
	pch = strstr(poff,"://");
	if(pch){
		off += strsubcpy(spi_url->buf+off,poff,pch-1,sizeof(spi_url->buf)-off);
		poff = pch+3;
	}else{
		off += strlcpy(spi_url->buf+off,"http",sizeof(spi_url->buf)-off);
	}
	off++;
	
	//get the char that split path and site
	pch = poff;
	while(*pch && *pch!='/' && *pch!='#' && *pch!='?'){
		pch++;	
	}
	
	spi_url->path = spi_url->buf+off;
	spi_url->buf[off++] = '/';
	if(*pch){
		off += strlcpy(spi_url->buf+off,(*pch=='/')?(pch+1):pch,sizeof(spi_url->buf)-off);
	}
	spi_url->buf[off] = 0;
	off++;
	

	spi_url->site = spi_url->buf+off;
	off += strsubcpy(spi_url->buf+off,poff,pch-1,sizeof(spi_url->buf)-off);
	poff = spi_url->site;
	
	//get usrpwd, poff behind '@'
	pch = index(poff,'@');
	if(pch){
		spi_url->usrpwd = poff;
		*pch = 0;
		poff = pch+1;
	}
	
	//get site and port
	pch = index(poff,':');
	if(pch){
		spi_url->port = atoi(pch+1);
		*pch = 0;	
	}
	spi_url->site = poff;
	
	if(spi_url->site[0]==0){
		WARNING("NoSite.url=%s",url);
		return -1;
	}

	if(spi_url->port<=0||spi_url->port>65535){
		spi_url->port = 80;	
	}

	return 0;
}

int spi_url_sprint(spi_url_t *url,char *buf,int buf_size){
	int off = 0;
	
	if(url==NULL||url->site==NULL||url->site[0]==0){
		WARNING("spi_url=%p,url->site=%p,url->site[0]=%d",
			url, url?url->site:NULL, (url&&url->site)?url->site[0]:0);
		return -1;		
	}
	
	off += snprintf(buf+off,buf_size-off,"%s://",url->schema);
	
	if(url->usrpwd && off<buf_size){
		off += snprintf(buf+off,buf_size-off,"%s@",url->usrpwd);
	}
	
	if(url->site && off<buf_size){
		off += snprintf(buf+off,buf_size-off,"%s",url->site);
	}
	if(url->port!=80&& off<buf_size){
		off += snprintf(buf+off,buf_size-off,":%d",url->port);
	}
	if(off<buf_size){
		off += snprintf(buf+off,buf_size-off,"%s",url->path?url->path:"");
	}
	if(off<buf_size){
		buf[off] = 0;
	}else{
		buf[buf_size-1] = 0;	
	}
	return 0;
}

int spi_url_print(spi_url_t *url){
	if(url==NULL||url->site==NULL||url->site[0]==0){
		WARNING("spi_url=%p,url->site=%p,url->site[0]=%d",
			url, url?url->site:NULL, (url&&url->site)?url->site[0]:0);
		return -1;	
	}
	#define print_item(var) printf("  %s=%s\n",(#var),(var)?(var):"(nil)")
	#define print_port(var) printf("  %s=%d\n",(#var),(var))
	print_item(url->schema);
	print_item(url->usrpwd);
	print_item(url->site);
	print_port(url->port);
	print_item(url->path);
	#undef print_item
	#undef print_port
	
	return 0;
}

#ifdef	_CREATE_TEST_SUITE__

int main() {
    WARNING("why ?");
    WARNING("are you %s ?", "Jason");
    WARNING("Hello, this is %s, %s is %d years old.", "David", (1 ? "he" : "she"), 20);

    return 0;
}

#endif				/* _CREATE_TEST_SUITE__ */
