
#ifndef __UL_ICONV_H__
#define __UL_ICONV_H__

#include "ul_dict.h"
#include <iconv.h>

#ifndef u_char
#define u_char		unsigned char
#endif

// This init function should be called in main thread.
// Return : 0, success; -1, failed
int ul_jcode_transfer_init();

 unsigned short get_sjis_fa2gb(unsigned short index) ;
// in this module , gb means GB18030
typedef struct _jp2gb_t
{
	iconv_t unicode2gb;
	iconv_t utf82gb;
	iconv_t jis2gb;
	iconv_t sjis2gb;
	iconv_t eucjp2gb;
} jp2gb_t;

// These function should be called in every sub thread.
// Return : 0, success; -1, failed
int ul_jp2gb_open(jp2gb_t *pConv);
int ul_jp2gb_close(jp2gb_t *pConv);

typedef struct _gb2jp_t
{
	iconv_t gb2unicode;
	iconv_t gb2utf8;
	iconv_t gb2jis;
	iconv_t gb2sjis;
	iconv_t gb2eucjp;
} gb2jp_t;

// These function should be called in every sub thread.
// Return : 0, success; -1, failed
int ul_gb2jp_open(gb2jp_t *pConv);
int ul_gb2jp_close(gb2jp_t *pConv);


// transfer functions
// Input : psrc, end with '\0'
// 	 : pdes, dest buffer
// 	 : destlen, length of dest buffer, at least >= strlen(psrc)+1, 
// 	 	    2*strlen(psrc) may be safe enough as GB18030 code
//
// Output: pdes, end with '\0'
// 	 	 
// Return : -1, failed; >=0, success, the size of un-converted bytes

int ul_jis_gb_str(char *psrc, char *pdes, int destlen, jp2gb_t *pConv);
int ul_jis_gb_str(char *psrc, char *pdes, int destlen);

int ul_sjis_gb_str(char *psrc, char *pdes, int destlen, jp2gb_t *pConv);
int ul_sjis_gb_str(char *psrc, char *pdes, int destlen);

int ul_eucjp_gb_str(char *psrc, char *pdes, int destlen, jp2gb_t *pConv);
int ul_eucjp_gb_str(char *psrc, char *pdes, int destlen);

int ul_utf8_gb_str(char *psrc, char *pdes, int destlen, jp2gb_t *pConv);
int ul_utf8_gb_str(char *psrc, char *pdes, int destlen);


// the following are functions of gb str transfer to jp encoded str
// Input : destlen , length of dest buffer, at least >= 1.5 * strlen(psrc) + 1

int ul_gb_jis_str(char *psrc, char *pdes, int destlen, gb2jp_t *pConv);
int ul_gb_jis_str(char *psrc, char *pdes, int destlen);

int ul_gb_sjis_str(char *psrc, char *pdes, int destlen, gb2jp_t *pConv);
int ul_gb_sjis_str(char *psrc, char *pdes, int destlen);

int ul_gb_eucjp_str(char *psrc, char *pdes, int destlen, gb2jp_t *pConv);
int ul_gb_eucjp_str(char *psrc, char *pdes, int destlen);

int ul_gb_utf8_str(char *psrc, char *pdes, int destlen, gb2jp_t *pConv);
int ul_gb_utf8_str(char *psrc, char *pdes, int destlen);

/* 将 gb18030 编码 中的 半角假名 转换到全角假名 
 * pdes and  psrc can same memory 
 * destlen >= strlen(psrc) + 1 ;
 *
 * return : 转换后pdes 里面的字符个数 , not include '\0'
 * 	    failed return -1  if parameter error
 **/
int ul_gb_jm_bj2qj(char * psrc , char *pdes , unsigned int destlen) ;

/*是否是gb18030 1字节*/
inline int is_gb_byte1( u_char *pstr )
{
	return ( pstr[0]<=0x7F ) ;
}

/*是否是gb18030 2字节*/
inline int is_gb_byte2( u_char *pstr )
{
	return ( pstr[0] >= 0x81 && pstr[0] <= 0xFE  && pstr[1] >= 0x40 && pstr[1] <= 0xFE && pstr[1] != 0x7F ) ;
}

/*是否是gb18030 4字节*/
inline int is_gb_byte4( u_char *pstr )
{
	return (   pstr[0] >= 0x81 && pstr[0] <= 0xFE
	       	&& pstr[1] >= 0x30 && pstr[1] <= 0x39
	        && pstr[2] >= 0x81 && pstr[2] <= 0xFE
	        && pstr[3] >= 0x30 && pstr[3] <= 0x39 ) ;
}
#endif


