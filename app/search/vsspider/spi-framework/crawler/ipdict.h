#ifndef __IP_DICT_H__
#define __IP_DICT_H__

#include <time.h>
#include "bsl/containers/hash/bsl_hashmap.h"
#include "bsl/containers/string/bsl_string.h"

#include "spidns.h"

typedef bsl::string bstring;

//最久不更新次数阈值
static const int IP_UPDATE_THRES = 20;

// ip结点
struct ip_node{
	time_t	last_update_time;		//上次dns更新时间
	int		update_cnt;				//未更新次数计数
	char	ip[32];					//ip信息
	siteinfo_t siteinfo;			//用来存储robots信息
};

// bstring的hash仿函数
struct bstring_hash{
	inline size_t operator () (const bstring& str) const{
		size_t ret = 0;
		const char* p = str.c_str();
		for ( ; *p != '\0'; p++){
			ret *= 16777619;
			ret ^= (size_t) *(const unsigned char*) p;
		}
		return ret;
	}
};


typedef bsl::hashmap<bstring,ip_node,bstring_hash> ip_dict_t;

/**
 * @brief url转化给ip
 * 
 * @param [in] url
 * @param [out] ip
 * @param [in] ip_len
 * @return int 是否获取ip信息
 * @retval  -1 失败；0 成功
 **/
int url2ip(const char *url, char *ip, int ip_len);

/**
 * @brief site转化给ip
 * 
 * @param [in] site
 * @param [out] ip
 * @param [in] ip_len
 * @return int 是否获取ip信息
 * @retval  -1 失败；0 成功
 **/
int site2ip(const char *site, char *ip, int ip_len);


/**
 * @brief url 是否被robots屏蔽
 * @return int 
 * @retval 1 是 ; 0否
 */
int is_robots_banned(const char *url);


/**
 * @brief url 获取ip信息和robots信息
 * @param [in] url
 * @param [in/out] ip 
 * @param [in] ip_len
 * @param [out] robots  1 forbiden; 0 ok
 * @return int 
 * @retval -1 失败 ; 0成功
 */
int get_ip_and_robots(const char *url, char *ip, int ip_len, int *robots);

/**
 * @brief 词典初始化
 * @return int 初始化结果
 * @retval -1失败；0成功
 **/
int init_ipdict();

/**
 * @brief for debug
 */
void info_ipdict();

#endif
