/** * $Id: vs-spider.h,v 1.34 2009/06/10 07:34:58 zhuhk Exp $
 */

#ifndef	_VSSPIDER_H__
#define	_VSSPIDER_H__

// ...

#define mkCMD(cmdNo) \
	(((GroupName) << 16) + (cmdNo))

enum {
	// selector
#define GroupName 0
	eSelectorCMDXXX = mkCMD(0),
	eSelectorCMDArbitrateResp = mkCMD(1),
	eSelectorCMDResultResp = mkCMD(2),
	eSelectorCMDRegister = mkCMD(3),

	e_selector_dnsfailed = mkCMD(32),
	e_selector_robotsforbidden = mkCMD(33),

	// BA
#undef	GroupName
#define GroupName 1	// 65536
	eBACMDXXX = mkCMD(0),
	eBACMDArbitrate = mkCMD(1),
	eBACMDResult = mkCMD(2),
	eBACMDRegisterResp = mkCMD(3),

	eBACMDNewArbitrate = mkCMD(4),
	eBACMDNewArbitrateResp = mkCMD(5),

	// cc
#undef	GroupName
#define GroupName 2	// 131072
	PACK_BA2CC			= mkCMD(0),  	//BA仲裁结果给CC
	PACK_CC_PULL_ST		= mkCMD(1),		//CC向Selector拉数据
	PACK_ST_PUSH_CC		= mkCMD(2),		//Selector向CC发数据
	PACK_CC2CR			= mkCMD(3),		//CC转发数据给Crawler
	PACK_ST_LOGIN_CC	= mkCMD(4),		//Selector登陆CC模块

	PACK_CR2PP 			= mkCMD(5),		//Crawler发送给PP的包
	
	PACK_QUERY_AB_REQ	= mkCMD(6),		//请求ab查找的包
	PACK_QUERY_AB_RES	= mkCMD(7),		//回复ab查找的包
	PACK_UPDATE_AB_REQ	= mkCMD(8),		//请求ab修改的包
	PACK_UPDATE_AB_RES	= mkCMD(9),		//回复ab修改的包

#undef  GroupName

#define GroupName 3	// 196608
//linkdb请求包类型
	PACK_LINKDB_SELECT			= mkCMD(0),	// 196608
	PACK_LINKDB_SEEK			= mkCMD(1),
	PACK_LINKDB_UPDATE			= mkCMD(2),
	PACK_LINKDB_INSERT			= mkCMD(3),
	PACK_LINKDB_DELETE			= mkCMD(4),
	PACK_LINKDB_SESSION_LIST	= mkCMD(5),
	PACK_LINKDB_SESSION_STAT	= mkCMD(6),
//linkdb响应包类型
	PACK_LINKDB_SUCCESS			= mkCMD(7),	// 196615
	PACK_LINKDB_FAILED 			= mkCMD(8),
	PACK_LINKDB_END				= mkCMD(9),
	PACK_LINKDB_NOT_FOUND		= mkCMD(10),
	PACK_LINKDB_EXIST			= mkCMD(11),
	PACK_LINKDB_REQ_TYPE		= mkCMD(12),

//请求包类型
	PACK_LINKDB_LOAD_META       = mkCMD(13),
	PACK_LINKDB_DUMP_STAT		= mkCMD(14),
#undef  GroupName

//saver
#define GroupName 4	// 262144

	PACK_CRAWL_RESULT			= mkCMD(0),
	PACK_FOLLOW_LINK			= mkCMD(1),
	PACK_DUMP_FIFO_STAT			= mkCMD(2),
	PACK_LOAD_FIFO_STAT			= mkCMD(3),
	PACK_STOP_SAVER				= mkCMD(4),

	PACK_INVALID_PACK_TYPE		= mkCMD(5),
	PACK_SAVER_SUCCESS			= mkCMD(6),
	PACK_SAVER_FAILED			= mkCMD(7)

#undef  GroupName

};


/**
 * Max mcpack size.
 */
 
#define	MAX_PACK_SIZE			1024*1024


/**
 * Default Ports.
 */

#define DEFAULT_BA_SERVPORT			9000
#define DEFAULT_LINKDB_SERVPORT			18000
#define DEFAULT_CC_SERVPORT			6776
#define DEFAULT_CRAWLER_SERVPORT		9300
#define DEFAULT_ABILITYMONTIOR_SERVPORT		12321
#define DEFAULT_SAVER_SERVPORT			17000
#define DEFAULT_AB_SERVPORT			12321
#define DEFAULT_EC_SERVPORT			2160
#define DEFAULT_SFAGENT_SERVPORT		9100
#define DEFAULT_SF_SERVPORT			2324

//module sender name
#define		BA		"ba"
#define		CC		"crawler-control"
#define		ST		"selector"
#define		CWR		"crawler"
#define		AB		"ability-montior"
#define		DB		"linkdb"
#define		SV		"saver"

//pack field string macro
#define SENDER				"Sender"
#define PACK_TYPE			"Pack_type"
#define LOGID				"LogID"

#define GROUPID				"GroupID"
#define SELECTID			"SelectID"
#define ALLOCATED			"Allocated"
#define IP					"IP"
#define CRAWLNUM			"Crawlnum"  
#define PORT				"Port"


#define	SUBPACKNUM			"Subpacknum"


//linkdb相关字段宏定义，mppack包key和数据库字段保持一致
#define	LINK_ID				"link_id"
#define	P_LINK_ID			"p_link_id"
#define	SESSION_ID			"session_id"
#define	SITE_ID				"site_id"

#define	RANK_IN				"rank_in"
#define	RANK_OUT			"rank_out"
#define	LINK_TYPE			"link_type"
#define	LINK_LEVEL			"link_level"
#define	LINK_DEPTH			"link_depth"
#define	URLNEW				"urlnew"
#define	LINK_CHARACTER		"link_character"
#define	ROBOTS_VERSION		"robots_version"

#define	METHOD				"method"
#define	HTTP_VERSION		"http_version"
#define	CRAWLER_TYPE		"crawler_type"
#define	IS_COOKIE			"is_cookie"
#define	IS_REFERRER			"is_referrer"

#define	IS_SELECT			"is_select"
#define	WILL_DEL			"will_del"
#define	NOT_MOD				"not_mod"
#define	ORI_CODETYPE		"ori_codetype"
#define	PAGE_LEN			"page_len"
#define	ETAG_CHECKSUM		"etag_checksum"
#define	CONTENT_CHECKSUM	"content_checksum"
#define	HISTORY				"history"
#define	FAILNO				"failno"

#define	KEEP_ALIVE			"keep_alive"
#define	IN_TIME				"in_time"
#define	SEL_TIME			"sel_time"
#define	LAST_MERGE_TIME		"last_merge_time"
#define	LAST_CHANGE_TIME	"last_change_time"
#define	LAST_MOD_TIME		"last_mod_time"

#define	ATTR_RESERVED1		"attr_reserved1"
#define	ATTR_RESERVED2		"attr_reserved2"
#define	TIME_RESERVED1		"time_reserved1"
#define	TIME_RESERVED2		"time_reserved2"

#define	URL					"url"
#define	ANCHOR				"anchor"
#define	COOKIE				"cookie"
#define	REFERRER			"referrer"
#define	TO_URL				"to_url"
#define	IDENTIFICATION		"identification"

// crawler string macro

#define CRAWLER				0
#define SUNFLOWER			1

#define	DEFAULT_LINK_TYPE	65535
#define	DEFAULT_LINK_LEVEL	65535

///end

#define	MAX_URL_LEN				2048
#define	MAX_TOURL_LEN			4096
#define	MAX_ANCHOR_LEN			512
#define	MAX_COOKIE_LEN			1024
#define	MAX_REFERRER_LEN		1024
#define	MAX_IDENTIFICATION_LEN	512


#endif	/* ~ _VSSPIDER_H__ */
