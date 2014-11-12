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
 * @brief parse a url, ul_parse_url��֧�ִ���256��url
 */
int parse_a_url(const char *url, spi_url_t *out);


/** 
 * @brief ��ȡһ��
 *      
 * @param [in] fp : FILE* �ļ����
 * @param [out] line : ���ļ��ж���һ�е�buffer
 * @param [in] len : buffer����
 * @return  int 
 *          ����    -1��ʾ�ļ��Ѷ���
 *          ����    -2��ʾ��������
 *          ����    0 ��ʶ�ɹ���ȡһ��
 **/
int get_line(FILE* fp, char* line, int len);

// print mcpack content
void p_mcpack(mc_pack_t *mcpack, char *descript);

int is_same_site(char *lhs, char *rhs);

#endif
