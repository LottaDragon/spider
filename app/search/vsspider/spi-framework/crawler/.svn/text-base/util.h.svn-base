#ifndef __CRW_UTIL_H__
#define __CRW_UTIL_H__ 

#include <stdint.h>
#include "mc_pack.h"
#include "uln_url.h"
#include "misc.h"

/**
 * remove beginning and ending space char
 * if exist beginning space char, move the string bytes forward
 * if exist ending space char, mark the terminate
 */
//char *str_trim(char *str);

/**
 * remove beginning and ending space char
 * if exist beginning space char, just mark first byte, DO NOT MOVE
 * if exist ending space char, mark the terminate
 */
char *str_trim_ex(char *str);

// sleep in mili-secs
//int Nanosleep(int milidelay);

// get current mili-secs
int64_t get_ms();

/**
 * get the meta refresh url
 * if redir success, return 0 and fill redir
 * else redir fail, return -1
 */
int is_meta_refresh(char *content, char *redir, int redir_len);


/**
 * get absolute url
 * if success, fill urlbuf and return it
 * else return NULL
 */
char* get_abs_url(const char* baseurl, const char* childurl, char *urlbuf, size_t buflen);


/**
 * @brief parse a url, ul_parse_url不支持大于256的url
 */
int parse_a_url(const char *url, spi_url_t *out);


/** 
 * @brief 获取一行
 *      
 * @param [in] fp : FILE* 文件句柄
 * @param [out] line : 从文件中读出一行的buffer
 * @param [in] len : buffer长度
 * @return  int 
 *          返回    -1表示文件已读完
 *          返回    -2表示读到空行
 *          返回    0 标识成功读取一行
 **/
int get_line(FILE* fp, char* line, int len);

// print mcpack content
void p_mcpack(mc_pack_t *mcpack, char *descript);

int is_same_site(char *lhs, char *rhs);

#endif
