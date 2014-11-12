#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <assert.h>

#include "ul_url.h"
#include "ul_log.h"

/*
#include "ul_func.h"
#include "ul_dict.h"
#include "ul_sign.h"
#include "ul_conf.h"
#include "ul_exlink.h"
#include "ul_pack.h"

#include <htmlparser2/uni_gb.h>
#include <htmlparser2/html_reference_char.h>
#include <htmlparser2/html_text_utils.h>

#include <htmlparser2/html_dtd.h>
#include <htmlparser2/html_tree_internal.h>
#include <htmlparser2/html_tree.h>

#include <htmlparser2/html_parser.h>

#include "ul_ccode.h"
#include "ul_char.h"
// */

#ifndef _COMMON_FUNC_H_
#define _COMMON_FUNC_H_

#define	CATCH_JS_CMD 	0x01
#define ACK_CATCH_CMD	0x02
#define GET_JS_CMD		0x03
#define RET_JS_CMD		0x04
#define RUN_JSLINK_CMD	0x05
#define ACK_RUN_CMD		0x06
#define RET_LINK_CMD	0x07
#define RET_WRITED_PAGE_CMD    0x08
#define RUN_WRITED_PAGE_JSLINK_CMD 0x09

#define	TYPE_BYTE			0
#define VER_BYTE			1
#define ACK_BYTE			2
#define	LINK_NUM_BYTE		2
#define	DYNLINK_NUM_BYTE	2
#define	PACK_LEN_BYTE		8
#define	PACK_BODY_BYTE		12
#define	SCRP_LEN_BYTE		12
#define LINK_LEN_BYTE       12
#define	DYNLINK_LEN_BYTE	16
#define TITLE_LEN_BYTE      16
#define	SCRP_BYTE			20	
#define	URLS_BYTE			20	


#define	PACK_LEN_BYTE_NUM		4	
#define	LINK_NUM_BYTE_NUM		2	
#define	DYNLINK_NUM_BYTE_NUM	2
#define	SCRP_LEN_BYTE_NUM		4
#define	DYNLINK_LEN_BYTE_NUM	4
#define LINK_LEN_BYTE_NUM       4
#define TITLE_LEN_BYTE_NUM      4

#define VER				0x01

#define	MAX_URL_NUM	1024
#define	MAX_DYNLINK_NUM	1024

#define TOO_MANY_URL 	-2
#define	OK				1
#define	BUSY			-2
#define FAIL 			-1

#define CATCHINTG_NOW				0x01	//改url正在被抓取
#define HTTP_2						0x02	//HTTP返回2XX
#define HTTP_3						0x03	//HTTP返回3XX
#define HTTP_4						0x04	//HTTP返回4XX
#define HTTP_OTHER					0x05	//HTTP返回非2XX,3XX,4XX
#define TIME_OUT					0x06	//改url正在被抓取
#define	CATCH_FAIL					0x07	//该url抓取失败


typedef struct _js_url_t{
	char url[UL_MAX_URL_LEN];
}js_url_t;

typedef struct _dyn_link_t{
	char js_link[UL_MAX_URL_LEN];
	int  offset;
}dyn_link_t;

#define is_space(c)	( c == ' ' || c == '\t' || c == '\n' || c == '\r')

#define is_alpha(c)	( (c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A'))

#define is_digit(c)	( (c <= '9' && c >= '0'))

#define is_chinese(c1,c2) (((unsigned char)c1 >= 0x81 && (unsigned char)c1 <= 0xfe) && ( (unsigned char)c2 >= 0x40 && (unsigned char)c2 <= 0xff))

#define is_chn_space(ch0,ch1)   ((unsigned char)(ch0) == 0xA1 && (unsigned char)(ch1) == 0xA1)

#define trim1(_p,_pend)\
	do{\
		while(is_space(*(_p)))\
			_p++;\
		while(is_space(*(_pend)))\
		{\
			*(_pend) = 0;\
			_pend--;\
		}\
	}while(0)
	
extern int make_catch_js_pack(char *pack,int pack_len,js_url_t *jsurl,int *url_num);

extern int extract_cmd_head(char *head,int *url_num,int *packet_length,int *ack);

extern int extract_js_catch_body(char *body,js_url_t *js_url,int url_num,int pack_length);

extern int make_ack_catch_pack(char *pack,int pack_len,int ack);

extern int make_ret_js_pack(char *pack,int pack_len,int http_status,char *page,int page_len);

extern int _copy_text(char *dest,int avail,int size,char *src);

extern int make_get_js_pack(char *pack,int pack_len,char *url);

extern int extract_run_jslink_body(char *body,char (*js_link)[UL_MAX_URL_LEN],int *offset,char *url,char *script,
int packet_length,int js_link_num);

extern int make_ret_link_pack(char *pack,int pack_len,int ack,char *link,int offset);

extern int extract_ret_jslink_body(char *body,int packet_length,char (*js_link)[UL_MAX_URL_LEN],
int *offset,char *title,int max_title_len,int max_link_num);

extern int extract_ret_writed_page_body(char *body,char *html, int packet_length);

extern int make_ack_run_pack(char *pack,int pack_len,int ack);

extern int make_run_jslink_pack(char *pack,int pack_len,char *url,char *script,int script_len,dyn_link_t *dyn_link,int *dyn_link_num);

extern int make_run_writed_jslink_pack(char *pack,int pack_len,char *url,char *script,int script_len,dyn_link_t *dyn_link,int *dyn_link_num);

extern int net4_ntop(unsigned int ip, char *ipstr);
#endif
