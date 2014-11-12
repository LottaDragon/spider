#include "handler.h"
#include "util.h"
#include "ipdict.h"
#include "misc.h"

void epool::show()
{
	_LOG_TRACE("[con_num:%d][max_conn:%d]",con_num,max_conn);	
}

int epool::finish_write(page_t* ppage, int64_t now)
{
	assert(ppage);
	assert(ppage->req);

	char *url = ppage->req->url;
	char *ip = ppage->req->ip;

	_LOG_NOTICE("result[%d,%s],url[%s],ip[%s]",ppage->res.result,
			Result_Name[ppage->res.result],url,ip);	

	int ret = send_2_pp(ppage, now);
	#ifdef _DEBUG
	ppage->show();
	#endif
	if( ret <0){
		_LOG_WARNING("send pack 2 pp fail.[url:%s][statu:%d][contlen:%d][contpos:%d]",
				ppage->req->url,ppage->res.statu,ppage->res.cont_len,ppage->cont_pos);	
	} else{
		_LOG_NOTICE("send pack 2 pp ok.[url:%s][statu:%d][contlen:%d][contpos:%d]",
				ppage->req->url,ppage->res.statu,ppage->res.cont_len,ppage->cont_pos);	
	}

	//释放请求资源
	if(ppage->req){
		free(ppage->req);	
		ppage->req = NULL;
	}

	//释放page buffer
	if(ppage->content){
		free(ppage->content);
		ppage->content = NULL;
	}

	free(ppage);

	return 0;
} 

int connect_pp(req_crawl_t* req)
{
	assert(req);

	int sock = ul_tcpconnecto_ms( req->pp_ip, req->pp_port, 1000);
	while(sock<0){
		_LOG_WARNING("[url:%s] cannot connect to pp[ip:%s][port:%d]",
				req->url,req->pp_ip,req->pp_port);
		Nanosleep(5);
		sock = ul_tcpconnecto_ms( req->pp_ip, req->pp_port, 1000);
	}

	return sock;
}

 
/**
 * @brief 非阻塞连接
 *
 * @param [in] req   : 请求结构体指针
 * @param [out] sock  :  传出的socket
 * @return  读取的结果
 * @retval  -1 失败 0读取成功 1 InProgress
 **/
int connect_url_nonblock( req_crawl_t* req, int *fd, int *err)
{
	assert(fd);
	int ret;

	//char host[256];
	//char sport[10]={0};
	//char file[MAX_URL_LEN];

	char* url = req->url;

//	spi_url_t spi_url;
//	if( spi_url_parse( url, &spi_url ) != 0 ){
//		_LOG_WARNING( "[URL:%s] is a bad url", url);
//		*err = ERR_BAD_URL;
//		return -1;
//	}

#if 0
	if (ul_parse_url( url, host, sport, file) == 0) {
		_LOG_WARNING( "[URL:%s] is a bad url", url);
		*err = ERR_BAD_URL;
		return -1;
	}
#endif

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if( sock < 0 ){
		_LOG_WARNING("url[%s] create socket failed. %m",req->url);
		*err = ERR_CONN;
		return -1;
	}

	if (ul_setsocktonoblock( sock )){
		_LOG_WARNING("url[%s] setnonblock failed. %m",req->url);
		close(sock);
		*err = ERR_CONN;
		return -1;
	}


	char *ip = req->ip;
	int port = req->port;
	/* 	
		if( nip != 0){
		struct in_addr addr;
		memmove(&addr,&nip,sizeof(uint32_t));
	//inet_ntoa is not posix safe
	inet_ntop(AF_INET, &addr, ip, sizeof(ip)); 
	}
	 */
	if( port == 0 ){
		WARNING("port=%d. url=%s",port,url);
		port = 80;		
	}


	struct sockaddr_in soin;
	soin.sin_family = AF_INET;
	soin.sin_port = htons((uint16_t)port);

	if( ip[0] == 0 ){
		ret = url2ip( url, ip, sizeof(req->ip) );	
		if( ret < 0 ){
			ip[0] = 0;
		}
	}

	if( ip[0] == 0 ){
		_LOG_WARNING("url[%s] libdns resolve failed. ",req->url);
		close(sock);
		*err = ERR_DNS_RESOLVE;
		return -1;
	}

	if ((soin.sin_addr.s_addr = inet_addr(ip)) != INADDR_NONE) {
		ret = connect(sock, (struct sockaddr *) &soin, sizeof(soin) );		
		if( ret == -1){
			if (errno == EINPROGRESS || 
					errno == EALREADY	/*||
					errno == EINTR */){
				_LOG_DEBUG("url[%s] connecting in progress...",req->url);
				*fd = sock;
				return 1;
			} else{
				// 再用url来一次
				_LOG_WARNING("url[%s] ip[%s] connect failed. %m",req->url,ip);
				close(sock);
				*err = ERR_CONN;
				return -1;
			}
		}else{
			_LOG_NOTICE("connect success [url:%s][ip:%s]", url, ip);
			*fd = sock;
			return 0;
		}
	}else{

		_LOG_WARNING("url[%s] [%s] inet_addr failed. ",req->url, ip);
		close(sock);
		*err = ERR_CONN;
		return -1;
	}



#if 0
	char buf[8192];
	struct hostent he, *p = NULL;
	int err = 0;

	if ((ret = ul_gethostbyname_r(host, &he, buf, sizeof(buf), &p, &err)) < 0) {
		_LOG_WARNING("host[%s] resolve failed!",host);
		close(sock);
		return -1;
	}

	memcpy(&soin.sin_addr.s_addr, he.h_addr, sizeof(soin.sin_addr.s_addr));

	ret = connect(sock, (struct sockaddr *) &soin, sizeof(soin) );		
	if( ret == -1){
		if (errno == EINPROGRESS || 
				errno == EALREADY	||
				errno == EINTR ){
			_LOG_DEBUG("url[%s] connecting in progress...",req->url);
			*fd = sock;
			return 1;
		} else{
			_LOG_WARNING("url[%s] connect failed. %m",req->url);
			close(sock);
			return -1;
		}
	}else{
		_LOG_NOTICE("resolve connect success [url:%s][ip:%s][host:%s]", url, ip, host);
		*fd = sock;
		return 0;
	}
#endif

}

// 返回连接上的socket
int connect_url( req_crawl_t* req )
{
/*
	char host[256];
	char sport[10]={0};
	char file[MAX_URL_LEN];
 */
	char* url = req->url;

	spi_url_t spi_url;
	if( spi_url_parse( url, &spi_url ) != 0 ){
		_LOG_WARNING( "[URL:%s] is a bad url", url);
		return -1;
	}

	char *ip = req->ip;

	//Connect to http server...
	int ts = -1;
	if (ip[0] != 0 && strncmp(ip,"0.0.0.0",strlen("0.0.0.0") ) != 0 ) {
		ts = ul_tcpconnecto_ms(ip, spi_url.port, g_config.conn_timeout*1000);
		if (ts < 0) {
			ts = ul_tcpconnecto_ms(spi_url.site, spi_url.port, g_config.conn_timeout*1000);
		}
		if (ts < 0) {
			_LOG_WARNING("connect error [url:%s][ip:%s][host:%s]", url, ip, spi_url.site);
			return -1;
		}
	} else {
		ts = ul_tcpconnecto_ms(spi_url.site, spi_url.port, g_config.conn_timeout*1000);
		if (ts < 0) {
			_LOG_WARNING("connect error [url:%s][host:%s]", url, spi_url.site);
			return -1;
		}
	}

	_LOG_NOTICE("connect was saved by host [url:%s][ip:%s][host:%s]", url, ip, spi_url.site);

	//这里需要设置非堵塞模式， 否则读写的时候会被hang住
	if (ul_setsocktonoblock( ts )) 
	{
		close(ts);
		return -1;
	}

	return ts;
}


int epool::build_ab_stats_buff(page_t *ppage, char *buff, size_t buflen, int64_t now)
{
	assert(ppage&&buff);
	/**
	 * pack buff format
	 * "session_id:%u,url:%s,ip:%s,conn_start:%ld,write_start:%ld,read_start:%ld,"
	 * "end:%ld,statu:%d,pagelen:%d"
	 */
	char packbuff[8192],tmpbuf[8192];

	mc_pack_t *pp = mc_pack_open_w(2, packbuff, sizeof(packbuff), tmpbuf, sizeof(tmpbuf));

	if( MC_PACK_PTR_ERR(pp) ){
		_LOG_WARNING("can not create pack");
		return -1;                                    
	}   

	// 注意发指的int64_t在32和64平台下的打印
	// int64_t now = get_ms();

	int ret = mc_pack_put_uint32(pp,"session_id",ppage->req->session_id);
	if( ret){
		_LOG_WARNING("could not put session_id[%u]",ppage->req->session_id);     
		return -1;  
	}

	ret = mc_pack_put_str(pp,"url",ppage->req->url);
	if( ret){
		_LOG_WARNING("could not put url[%s] .ret=%d",ppage->req->url,ret);     
		return -1;  
	}
	
	ret = mc_pack_put_str(pp,"ip",ppage->req->ip);
	if( ret){
		_LOG_WARNING("could not put ip[%s]",ppage->req->ip);     
		return -1;  
	}

	ret = mc_pack_put_int64(pp,"conn_start",ppage->conn_start);
	if( ret){
		_LOG_WARNING("could not put conn_start[%ld]",ppage->conn_start);     
		return -1;  
	}

	ret = mc_pack_put_int64(pp,"write_start",ppage->write_start);
	if( ret){
		_LOG_WARNING("could not put write_start[%ld]",ppage->write_start);     
		return -1;  
	}

	ret = mc_pack_put_int64(pp,"read_start",ppage->read_start);
	if( ret){
		_LOG_WARNING("could not put read_start[%ld]",ppage->read_start);     
		return -1;  
	}

	ret = mc_pack_put_int64(pp,"end",now);
	if( ret){
		_LOG_WARNING("could not put end[%ld]",now);     
		return -1;  
	}

	ret = mc_pack_put_int32(pp,"statu",ppage->res.statu);
	if( ret){
		_LOG_WARNING("could not put statu[%d]",ppage->res.statu);     
		return -1;  
	}

	ret = mc_pack_put_int32(pp,"pagelen",ppage->res.cont_len);
	if( ret){
		_LOG_WARNING("could not put pagelen[%d]",ppage->res.cont_len);     
		return -1;  
	}
/*
	int ret = mc_pack_putf(pp,
			"session_id:%u,url:%s,conn_start:%lld,write_start:%lld,"
			"read_start:%lld,end:%lld,statu:%d,pagelen:%d",
			ppage->req->session_id,ppage->req->url,
			ppage->conn_start,ppage->write_start,
			ppage->read_start,now,
			ppage->res.statu,ppage->res.cont_len);

	if( ret != 8 ){
		_LOG_WARNING( "return value:%d "
				"{session_id:%u,url:%s,conn_start:%ld,write_start:%ld,"
				"read_start:%ld,end:%ld,statu:%d,pagelen:%d}",
				ret,ppage->req->session_id,ppage->req->url,
				ppage->conn_start,ppage->write_start,
				ppage->read_start,now,
				ppage->res.statu,ppage->res.cont_len);
		return -1;
	}	
//	mc_pack_close(pp);
*/
	if (mc_pack_pack2text(pp,buff,buflen,0) != 0 ){
		return -1;
	}

	return 0;
}

//command buffer 要够大
int makecmd(char *command, int cmdlen, req_crawl_t* req)
{
	char *url = req->url;
	int off,size;

	spi_url_t spi_url;
	if( spi_url_parse( url, &spi_url ) != 0 ){
		_LOG_WARNING( "[URL:%s] is a bad url", url);
		return -1;
	}
	
	off = 0;
	size = cmdlen-2;
	off += snprintf(command+off,size-off,"%s %s %s\r\n",req->method,spi_url.path,req->protocol);

	if(off<size){
		off += snprintf(command+off,size-off,"Host:%s\r\n",spi_url.site);
	}
	if(off<size){
		off += snprintf(command+off,size-off,"Accept:*/*\r\n");
	}
	if(off<size){
		off += snprintf(command+off,size-off,"Accept-Language:zh-cn\r\n");
	}
	if(off<size){
		off += snprintf(command+off,size-off,"Connection:close\r\n");
	}
	if(off<size){
		off += snprintf(command+off,size-off,"User-Agent:%s\r\n",g_config.user_agent);
	}
	if(off<size&&req->cookie[0]){
		off += snprintf(command+off,size-off,"Cookie:%s\r\n",req->cookie);
	}
	if(off<size&&req->referrer[0]){
		off += snprintf(command+off,size-off,"Referrer:%s\r\n",req->referrer);
	}
	if(off<size){
		sprintf(command+off,"\r\n");
	}else{
		sprintf(command+size-1,"\r\n");
	}
	TRACE("command=%s",command);	
	
	if(off>=size){
		command[size-1]=0;
		WARNING("off=%d,size=%d. url=%s,cookie=%s,referrer=%s,ua=%s",
			off,size,url,req->cookie,req->referrer,g_config.user_agent);
		return -1;
	}
	
	return 0;
}

//// makecmd_debug版本 保持长连接
//int makecmd_x1(char *command, int cmdlen, req_crawl_t* req)
//{
//	char *url = req->url;
//
//	spi_url_t spi_url;
//	if( spi_url_parse( url, &spi_url ) != 0 ){
//		_LOG_WARNING( "[URL:%s] is a bad url", url);
//		return -1;
//	}
//
//	/*
//	   char host[256];
//	   char sport[16];
//	   char file[MAX_URL_LEN];
//	 */
//#if 0
//	if ( ul_parse_url(url, host, sport, file) == 0 ) {
//		_LOG_WARNING("[url:%s] is a bad url", url);
//		return -1;
//	}
//#endif
//
//	snprintf (command, cmdlen,
//			"%s %s HTTP/1.1\r\n"
//			"Host: %s\r\n"
//			"Accept: */*\r\n"
//			"Accept-Language: zh-cn\r\n"
//			"Keep-Alive: 300\r\n"
//			"Connection: keep-alive\r\n",
//			//			"User-Agent: Test\r\n", 
//			//			"User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.0.8) Gecko/2009032609 Firefox/3.0.8\r\n",
//			//		"Referer: www.baidu.com\r\n",
//			//		"Cookie: xsnews_supe_refresh_items=0_u2205\r\n",
//			req->method, url_t.path, /*req->protocol,*/ url_t.host /*,g_config.user_agent*/);
//	command[cmdlen-1]=0;
//
//	_LOG_TRACE("%s",command);
//
//	int len = strlen(command);
//	if( req->cookie[0] != 0){
//		int	tmp = strlen(req->cookie);
//		if( len + tmp + (int)strlen("Cookie: \r\n") >= cmdlen ){
//			_LOG_WARNING("makecmd cookie truncated [url:%s][cookie:%s]",url,req->cookie);
//			return -1;	
//		}
//		sprintf(command+len,"Cookie: %s\r\n",req->cookie);
//		len = len + tmp+ (int)strlen("Cookie: \r\n");
//	}
//
//	if( req->referrer[0] != 0){
//		int tmp = strlen(req->referrer);
//		if( len + tmp + (int)strlen("Referer: \r\n") >= cmdlen){
//			_LOG_WARNING("makecmd referer truncated [url:%s][referer:%s]",url,req->referrer);
//			return -1;
//		} 
//		sprintf(command+len,"Referer: %s\r\n",req->referrer);
//		len = len + tmp + (int)strlen("Referer: \r\n");
//	}
//
//	// 还有\r\n
//	if( len + 2 >= cmdlen){
//		_LOG_WARNING("No buffer space for \"\\r\\n\"");
//		return -1;
//	}
//	strncpy(command+len,"\r\n",2);
//	*(command+len+2) = 0;	
//
//	_LOG_DEBUG("%s",command);
//	return 0;
//}
#if 0 
//从buffer中读取抓取包送入
void* fill_thread_func(void* arg)
{
	ul_logstat_t log_state;

	log_state.spec = 0;
	log_state.to_syslog = 0;
	log_state.events = UL_LOG_DEBUG;

	if (ul_openlog_r("fill_thread_func", &log_state) < 0) {
		return NULL;
	}

	epool* ep = (epool*)arg;

	_LOG_NOTICE("start to fill sockets...");

	while( g_running ){
		req_crawl_t* req = NULL;
		int ret = crawl_queue.pop( req);		
		if( ret == -1){
			_LOG_DEBUG("no ready req_crawl_t, sleep 1 ms");
			Nanosleep(1);
			continue;	
		}

		assert(req);

		ret = ep->add_sock(req);
		if( ret == 0){
			_LOG_DEBUG("add url:%s in the epool",req->url)	;
		}else{
			_LOG_DEBUG("cannot add url:%s in the epool",req->url)	;
		}

	}

	ul_closelog_r(0);	
	return NULL;
}
#endif

//抓取线程
void* crawl_thread_func(void* arg)
{
#if 0
	ul_logstat_t log_state;

	log_state.spec = g_logstat.spec;
	log_state.to_syslog = g_logstat.to_syslog;
	log_state.events = g_logstat.events;

	if (ul_openlog_r("crawl_thread_func", &log_state) < 0) {
		return NULL;
	}
#endif 
	com_openlog_r();
	
	epool* ep = (epool*)arg;
	
	ep->is_running = 1;
	
	_LOG_NOTICE("start to crawl...");

	while( g_running ){	
		ep->handle_sock();
	}

	com_closelog_r();
	
	ep->is_running = 0;

#if 0
	ul_closelog_r(0);	
#endif

	return NULL;	
}

#if 0
// 超时判断，先已合并到crawl_thread_func
void* mon_thread_func(void* arg)
{
	ul_logstat_t log_state;

	log_state.spec = 0;
	log_state.to_syslog = 0;
	log_state.events = UL_LOG_DEBUG;

	if (ul_openlog_r("mon_thread_func", &log_state) < 0) {
		return NULL;
	}

	epool* ep = (epool*)arg;

	_LOG_NOTICE("start to mon connection timeout...");


	while( g_running ){
		_LOG_NOTICE("sleep 5 seconds... and check_item ");
		sleep( 5 );	
		int64_t now = get_ms();
		ep->check_item(now);
	}

	ul_closelog_r(0);	
	return NULL;
}
#endif

/**
 * @brief 从buffer里面读出一行
 *
 * @param [in] buff   : string buffer
 * @param [in] buff   : string buffer length
 * @param [out] pos   : string buffer read pos/ line length
 * @param [out] line   :  读取的一行内容
 * @return  读取的结果
 * @retval  -1 失败；0 成功读取到一行; 1 buff数据不足以构成一行
 **/
int read_line(char* buff, int len, int *pos, char* line);

/**
 * @brief 处理抓取的http头
 *
 * @param [in/out] paget_t   : 抓取状态
 * @return  处理的结果
 * @retval  -1 失败 0读取成功，1头结束符为读取到
 **/
int handle_http_head( page_t* ppage)
{
	char line[8192], tmpstr[128];
	int  ret, line_len, tmpn;

	char *url = ppage->req->url;
	line_len = sizeof(line);

	char *buff = ppage->read_buf.buff + ppage->read_buf.read_pos;
	while( (ret = read_line( buff, ppage->read_buf.buff_len - ppage->read_buf.read_pos, 
					&line_len,line) ) == 0)
	{
#ifdef _DEBUG
		_LOG_DEBUG("line:%s",line);
#endif
		ppage->read_buf.read_pos += line_len;
		buff += line_len;


		if( (line_len == 1 && line[0] == '\n') ||
				(line_len == 2 && line[0] == '\r' && line[1] == '\n') )
		{
			//读完头部了，如果找到content-length

			if( ppage->res.cont_len > 0 ){     
				if( ppage->res.cont_len >  g_config.max_page_len){
					_LOG_TRACE("Content-length[%d] overflow Config.max_page_len[%d]",
						ppage->res.cont_len, g_config.max_page_len);
				}
				
				int nlen = ppage->res.cont_len > g_config.max_page_len ? 
					g_config.max_page_len : ppage->res.cont_len;
				ppage->content = (char*)malloc( nlen + 1 );
				ppage->res.cont_len = nlen;


			} else{
				ppage->content = (char*)malloc( g_config.max_page_len + 1);
				ppage->res.cont_len = g_config.max_page_len;
			}

			_LOG_DEBUG("malloc content [%d][%s] plus 1 byte", ppage->res.cont_len, url);

			ppage->content[ ppage->res.cont_len] = 0;

			//内存都没有就死掉算了
			assert(ppage->content);

			//将多读的content拷贝到content buffer里面去
			//不能写坏buffer
			int remaining = ppage->read_buf.buff_len - ppage->read_buf.read_pos;
			int cp_len = remaining < ppage->res.cont_len
						? remaining : ppage->res.cont_len;

			if( cp_len > 0 )
			{
				memmove( ppage->content, buff, cp_len);
				ppage->cont_pos = cp_len;
			}

			//for debug
			*(ppage->read_buf.buff + ppage->read_buf.read_pos) = 0;
			_LOG_DEBUG("http header:\n%s",ppage->read_buf.buff);
			return 0;
		}

		if ( strncasecmp(line,"HTTP/",strlen("HTTP/")) == 0){
			if ( sscanf(line, "%s %d", tmpstr, &tmpn) == 2 ){
				ppage->res.statu = tmpn;
				//status line
				if( tmpn / 100 == 2 || tmpn / 100 == 3 ){
					_LOG_DEBUG("response status succ [url:%s][statu:%d]", url, tmpn);
				} else{
					_LOG_DEBUG("response status fail [url:%s][statu:%d]", url, tmpn);
					return -1;
				}
			}else{
				_LOG_WARNING("why not get response code");
				return -1;
			}
		}

		if ( strncasecmp(line, "Last-Modified:", strlen("Last-Modified:")) == 0) {
			char *p = line+strlen("Last-Modified:");
			p = str_trim_ex(p);
			_LOG_DEBUG("[url:%s] Last-Modified : %s", url, p);
			strncpy( ppage->res.last_mod, p, MAX_PAGE_ITEM);
			ppage->res.last_mod[MAX_PAGE_ITEM-1] = 0;
		} 

		if ( strncasecmp(line, "Content-Length:", strlen("Content-Length:")) == 0) {
			if ( sscanf(line, "%s %d", tmpstr, &tmpn) == 2 ) {
				ppage->res.cont_len = tmpn;
				_LOG_DEBUG("[url:%s] content-length : %d",url, tmpn);
			}else{
				_LOG_WARNING("why not get content-length");
				return -1;
			}
		} 

		if ( strncasecmp(line, "Content-Type:", strlen("Content-Type:") ) == 0) {
			char *p = line+strlen("Content-Type:");
			p = str_trim_ex(p);
			_LOG_DEBUG("[url:%s] Content-Type: %s", url, p);
			strncpy( ppage->res.cont_type, p, MAX_PAGE_ITEM);
			ppage->res.cont_type[MAX_PAGE_ITEM-1] = 0;

			char *q = strcasestr(line,"Charset:");
			if( q) {
				q += strlen("Charset:");	
				q = str_trim_ex(q);
				_LOG_DEBUG("[url:%s] Charset: %s", url, q);
				strncpy( ppage->res.charset, q, MAX_PAGE_ITEM);
				ppage->res.charset[MAX_PAGE_ITEM-1] = 0;
			} 
		}

		if ( strncasecmp(line, "Location:", strlen("Location:")) == 0) 
		{
			char *p = line+strlen("Location:");
			p = str_trim_ex(p);
			_LOG_DEBUG("[url:%s] Location : %s", url, p);

			strncpy( ppage->res.redir, p, MAX_URL_LEN);
			ppage->res.redir[MAX_URL_LEN-1] = 0;    
		} 

		if ( strncasecmp(line, "Etag:", strlen("Etag:") ) == 0) {
			char *p = line+strlen("Etag:");
			p = str_trim_ex(p);
			_LOG_DEBUG("[url:%s] Etag: %s", url, p);
			strncpy( ppage->res.etag, p, MAX_PAGE_ITEM);
			ppage->res.etag[MAX_PAGE_ITEM-1] = 0;
		}		

		if ( strncasecmp(line, "Set-Cookie:", strlen("Set-Cookie:") ) == 0) {
			char *p = line+strlen("Set-Cookie:");
			p = str_trim_ex(p);
			_LOG_DEBUG("[url:%s] Set-Cookie: %s", url, p);
			strncpy( ppage->res.cookie, p, MAX_COOKIE_LEN);
			ppage->res.cookie[MAX_COOKIE_LEN-1] = 0;
		}		

		if ( strncasecmp(line, "Cache-Control:", strlen("Cache-Control:") ) == 0) {
			char *p = line+strlen("Cache-Control:");
			p = str_trim_ex(p);
			_LOG_DEBUG("[url:%s] Cache-Control: %s", url, p);
			strncpy( ppage->res.cache_control, p, MAX_PAGE_ITEM);
			ppage->res.cache_control[MAX_PAGE_ITEM-1] = 0;
		}

		if ( strncasecmp(line, "Expires:", strlen("Expires:") ) == 0) {
			char *p = line+strlen("Expires:");
			p = str_trim_ex(p);
			_LOG_DEBUG("[url:%s] Expires: %s", url, p);
			strncpy( ppage->res.expires, p, MAX_PAGE_ITEM);
			ppage->res.expires[MAX_PAGE_ITEM-1] = 0;
		}										

		line_len = sizeof(line);
	}

	//头还没有读完
	return ret;
}

/**
 * @brief 非堵塞读数据，只管读出数据，用于threadpool
 *
 * @param [in] sock   : socket
 * @param [in] buff  :  buffer
 * @param [in/out] write_pos : 当前写的位置  
 * @param [in] size : buff长度
 * @return  写的结果
 * @retval  -1 失败 0写完成功 1已经写到EAGAIN  2对方关闭
 **/
int write_buffer(int sock, char *buff, int *write_pos, int size)                                                               
{
	assert(buff);
	assert(write_pos);

	int ret;
	while (1) {  
		ret = write(sock, buff+(*write_pos), size-(*write_pos)); 
		if (-1 == ret && (EINTR == errno)) {
			continue;
		}
		break;                
	}  

	if (ret < 0){  
		if (EAGAIN == errno){ 
			return 1;
		}
		//写数据出错了        
		return -1;
	}  

	if (ret == 0){
		return 2;
	}             

	*write_pos += ret;         
	if (*write_pos == size){   
		return 0;
	}

	return 1;
}

/**
 * @brief 非堵塞读数据，只管读出数据，用于threadpool
 *
 * @param [in] sock   : socket
 * @param [out] buff  :  buffer
 * @param [in ] buff_len : buff长度
 * @param [out ] read_len : 读取长度
 * @return  读取的结果
 * @retval  -1 失败 0读取成功 1已经读到EAGAIN  2对方关闭
 **/
int read_buffer(int sock, char* buff, int buff_len, int* read_len)
{
	if( buff_len <= 0){
		return -1;	
	}

	//不论kernel里面是否还有数据，只读一次
	int ret = -1;
	int pos = 0;
	while(1){
		ret = read(sock, buff+pos, buff_len-pos);
		//非堵塞读出现EINTR是正常现象, 再读一次即可
		if( ret == -1 && errno == EINTR){
			continue;
		}
		pos+=ret;
		break;
	}

	if( ret == -1){
		if (EAGAIN == errno) {
			//出现EAGAIN表示读的时候，网络底层栈里没数据,返回1等待下次读取
			_LOG_DEBUG("read EAGAIN and wait for next time");
			return 1;
		}
		//读数据出错了
		_LOG_WARNING("read buff fail %m. ");
		return -1;
	}

	if ( ret == 0 ) {
		_LOG_DEBUG("read 0, close it");
		//读到0一般是对端close主动断开
		return 2;
	}

	*read_len = pos;

	return 0;

}


/**
 * @brief 从buffer里面读出一行
 *
 * @param [in] buff   : string buffer
 * @param [in] buff   : string buffer length
 * @param [out] pos   : string buffer read pos/ line length
 * @param [out] line   :  读取的一行内容
 * @return  读取的结果
 * @retval  -1 失败；0 成功读取到一行; 1 buff数据不足以构成一行
 **/
int read_line(char* buff, int len, int *pos, char* line)
{
	int  read_idx ;
	if( buff == NULL || len < 0  || pos == NULL || line == NULL ){
		return -1;	
	}

	for( read_idx = 0; read_idx < len ; read_idx++ ){
		if( buff[ read_idx ] == '\n'){ 
			break;
		}
	}

	if( buff[read_idx] == '\n'){

		if( read_idx + 1 + 1 > *pos ){
			_LOG_WARNING("read_line buffer overflowed [%d][%d]",read_idx+2,*pos );
			return -1;
		}

		memcpy(line,buff,read_idx+1);
		line[read_idx+1] = 0;
		*pos = read_idx+1;

		return 0;

	} else {
		line[0] = 0;
		return 1;
	}
}

int epool::send_2_pp(page_t* ppage, int64_t now)
{
	nshead_t *nh= (nshead_t *)pack_buff;

	char *buf= (char*)(nh+1);
	int	buflen = MAX_PACK_LEN - sizeof(nshead_t);
	char tmpbuf[8192];

	mc_pack_t *pp = mc_pack_open_w(2, buf, buflen, tmpbuf, sizeof(tmpbuf));

	if( MC_PACK_PTR_ERR(pp) ){     
		_LOG_WARNING("can not create crawler_2_pp pack");
		return -1;                      
	}

#if 0
	mc_pack_t *pp = mc_pack_create( (char*)(nh+1), MAX_PACK_LEN - sizeof(nshead_t) );
	if( ! mc_pack_valid(pp, MAX_PACK_LEN-sizeof(nshead_t) ) ){
		_LOG_WARNING("not valid crawler_2_pp pack");
		return -1;
	}
#endif

	req_crawl_t* req = ppage->req;

	int ret = mc_pack_text2pack( req->req_buff, pp);
	if( ret!=0 ){
		_LOG_WARNING("mc_pack_text2pack failed");

#ifdef _DEBUG
		_LOG_DEBUG("req_buff:%s",req->req_buff);
#endif

		return -1;  		
	}

	ret = mc_pack_mod_uint32(pp,"Pack_type", PACK_CR2PP);	
	if(ret){
		_LOG_WARNING("Pack_type mod PACK_CR2PP failed");
		return -1;
	}

	http_res_t res = ppage->res;

	ret = mc_pack_put_int64(pp,"t_crw_in",ppage->conn_start);	
	if(ret){
		_LOG_WARNING("mc_pack put crw_in time failed");
		return -1;
	}

	ret = mc_pack_put_int64(pp,"t_crw_out",now);	
	if(ret){
		_LOG_WARNING("mc_pack put crw_out time failed");
		return -1;
	}

	ret = mc_pack_put_int32(pp,"c_result",res.result);	
	if(ret){
		_LOG_WARNING("mc_pack put result failed");
		return -1;
	}

	ret = mc_pack_put_int32(pp,"c_statu",res.statu);
	if(ret){
		_LOG_WARNING("mc_pack put statu failed");
		return -1;
	}

	ret = mc_pack_put_int32(pp,"c_page_len",res.cont_len);
	if(ret){
		_LOG_WARNING("mc_pack put cont_len failed");
		return -1;
	}

	if( res.last_mod[0] != '\0'){
		ret = mc_pack_put_str(pp,"c_last_mod",res.last_mod);
		if(ret){
			_LOG_WARNING("mc_pack put last_mod failed");
			return -1;
		}
	}

	if( res.cont_type[0] != '\0'){
		ret = mc_pack_put_str(pp,"c_cont_type",res.cont_type);
		if(ret){
			_LOG_WARNING("mc_pack put cont_type failed");
			return -1;
		}
	}

	if( res.charset[0] != '\0'){
		ret = mc_pack_put_str(pp,"c_charset",res.charset);
		if(ret){
			_LOG_WARNING("mc_pack put charset failed");
			return -1;
		}
	}

	if( res.redir[0] != '\0'){
		ret = mc_pack_put_str(pp,"c_redir",res.redir);
		if(ret){
			_LOG_WARNING("mc_pack put redir failed");
			return -1;
		}
	}

	if( ppage->redir_trace[0] != '\0'){
		ret = mc_pack_put_str(pp,"c_redir_trace",ppage->redir_trace);
		if(ret){
			_LOG_WARNING("mc_pack put redir trace failed");
			return -1;
		}
	}

	if( res.etag[0] != '\0'){
		ret = mc_pack_put_str(pp,"c_new_etag",res.etag);
		if(ret){
			_LOG_WARNING("mc_pack put new_etag failed");
			return -1;
		}
	}

	if( res.cookie[0] != '\0'){
		ret = mc_pack_put_str(pp,"c_new_cookie",res.cookie);
		if(ret){
			_LOG_WARNING("mc_pack put new_cookie failed");
			return -1;
		}
	}

	if( res.cache_control[0] != '\0'){
		ret = mc_pack_put_str(pp,"c_cache_control",res.cache_control);
		if(ret){
			_LOG_WARNING("mc_pack put cache_control failed");
			return -1;
		}
	}

	if( res.expires[0] != '\0'){
		ret = mc_pack_put_str(pp,"c_expires",res.expires);
		if(ret){
			_LOG_WARNING("mc_pack put expires failed");
			return -1;
		}
	}

	if( res.cont_len > 0 && ppage->content[0] != '\0'){
		ret = mc_pack_put_raw(pp,"c_page",ppage->content,res.cont_len);
		if(ret){
			_LOG_WARNING("mc_pack put page failed");
			return -1;
		}

	}

	if( ppage->read_buf.read_pos > 0 && ppage->read_buf.buff[0] != '\0'){
		ret = mc_pack_put_str(pp,"c_header",ppage->read_buf.buff);
		if(ret){
			_LOG_WARNING("mc_pack put header failed");
			return -1;
		}
	}

	mc_pack_close(pp);    
	nh->body_len = mc_pack_get_size(pp);

	int sock = connect_pp(req);
	ret = nshead_write(sock, nh, 1000);

	if( g_config.retry_times <= 0){

		while( ret ){
			_LOG_WARNING("[url:%s] write to pp[ip:%s][port:%d] failed",req->url,req->pp_ip,req->pp_port);
			close( sock );
			sock = connect_pp(req);
			ret = nshead_write(sock, nh, 1000);
		}

	}else{
		int retry_cnt = 0;
		while( ret ){
			if( ++retry_cnt > g_config.retry_times ){
				_LOG_WARNING("[url:%s] retry [%d], write to pp[ip:%s][port:%d] failed and lost",
						req->url, g_config.retry_times, req->pp_ip, req->pp_port);
				break;
			}

			_LOG_WARNING("[url:%s] write to pp[ip:%s][port:%d] failed",req->url,req->pp_ip,req->pp_port);
			close( sock );
			sock = connect_pp(req);
			ret = nshead_write(sock, nh, 1000);
		}
	}

	//短连接	
	close(sock);

	if( ret == 0 ){
		_LOG_NOTICE("vsmon=sid:%u;c_crw2pp:1", req->session_id);
	}else{
		_LOG_WARNING("url write to pp failed![%s]", req->url);
	}

	return 0;
}

#if 0
//把抓取结果反馈给pp
int epool::send_2_pp1(page_t* ppage)
{
	nshead_t *nh= (nshead_t *)pack_buff1;

	mc_pack_t *pp = mc_pack_create( (char*)(nh+1), MAX_PACK_LEN - sizeof(nshead_t) );
	if( MC_PACK_PTR_ERR(pp) ){     
		_LOG_WARNING("can not create crawler_2_pp pack");
		return -1;                      
	}
	if( ! mc_pack_valid(pp, MAX_PACK_LEN-sizeof(nshead_t) ) ){
		_LOG_WARNING("not valid crawler_2_pp pack");
		return -1;
	}

	req_crawl_t* req = ppage->req;

	int ret = mc_pack_text2pack( req->req_buff, pp);
	if( ret!=0 ){
		_LOG_WARNING("mc_pack_text2pack failed");
		return -1;  		
	}

	http_res_t res = ppage->res;
	ret = mc_pack_put_int32(pp,"result",res.result);	
	if(ret){
		_LOG_WARNING("mc_pack put result failed");
		return -1;
	}

	ret = mc_pack_put_int32(pp,"statu",res.statu);
	if(ret){
		_LOG_WARNING("mc_pack put statu failed");
		return -1;
	}

	ret = mc_pack_put_int32(pp,"page_len",res.cont_len);
	if(ret){
		_LOG_WARNING("mc_pack put cont_len failed");
		return -1;
	}

	if( res.last_mod[0] != '\0'){
		ret = mc_pack_put_str(pp,"last_mod",res.last_mod);
		if(ret){
			_LOG_WARNING("mc_pack put last_mod failed");
			return -1;
		}
	}

	if( res.cont_type[0] != '\0'){
		ret = mc_pack_put_str(pp,"cont_type",res.cont_type);
		if(ret){
			_LOG_WARNING("mc_pack put cont_type failed");
			return -1;
		}
	}

	if( res.charset[0] != '\0'){
		ret = mc_pack_put_str(pp,"charset",res.charset);
		if(ret){
			_LOG_WARNING("mc_pack put charset failed");
			return -1;
		}
	}

	if( res.redir[0] != '\0'){
		ret = mc_pack_put_str(pp,"redir",res.redir);
		if(ret){
			_LOG_WARNING("mc_pack put redir failed");
			return -1;
		}
	}

	if( res.etag[0] != '\0'){
		ret = mc_pack_put_str(pp,"new_etag",res.etag);
		if(ret){
			_LOG_WARNING("mc_pack put new_etag failed");
			return -1;
		}
	}

	if( res.cookie[0] != '\0'){
		ret = mc_pack_put_str(pp,"new_cookie",res.cookie);
		if(ret){
			_LOG_WARNING("mc_pack put new_cookie failed");
			return -1;
		}
	}

	if( res.cache_control[0] != '\0'){
		ret = mc_pack_put_str(pp,"cache_control",res.cache_control);
		if(ret){
			_LOG_WARNING("mc_pack put cache_control failed");
			return -1;
		}
	}

	if( res.expires[0] != '\0'){
		ret = mc_pack_put_str(pp,"expires",res.expires);
		if(ret){
			_LOG_WARNING("mc_pack put expires failed");
			return -1;
		}
	}

	if( res.cont_len > 0 && ppage->content[0] != '\0'){
		ret = mc_pack_put_raw(pp,"page",ppage->content,res.cont_len);
		if(ret){
			_LOG_WARNING("mc_pack put page failed");
			return -1;
		}
	}


	mc_pack_close(pp);    
	nh->body_len = mc_pack_get_size(pp);

	int sock = connect_pp(req);
	ret = nshead_write(sock, nh, 1000);
	while( ret ){
		_LOG_WARNING("[url:%s] write to pp[ip:%s][port:%d] failed",req->url,req->pp_ip,req->pp_port);
		close( sock );
		Nanosleep(1);
		sock = connect_pp(req);
		ret = nshead_write(sock, nh, 1000);
	}

	//短连接	
	close(sock);
	return 0;
}

//把抓取结果反馈给pp 用于finish_write
int epool::send_2_pp2(page_t* ppage)
{
	nshead_t *nh= (nshead_t *)pack_buff2;

	mc_pack_t *pp = mc_pack_create( (char*)(nh+1), MAX_PACK_LEN - sizeof(nshead_t) );
	if( MC_PACK_PTR_ERR(pp) ){     
		_LOG_WARNING("can not create crawler_2_pp pack");
		return -1;                      
	}
	if( ! mc_pack_valid(pp, MAX_PACK_LEN-sizeof(nshead_t) ) ){
		_LOG_WARNING("not valid crawler_2_pp pack");
		return -1;
	}

	req_crawl_t* req = ppage->req;

	int ret = mc_pack_text2pack( req->req_buff, pp);
	if( ret!=0 ){
		_LOG_WARNING("mc_pack_text2pack failed");
		return -1;  		
	}

	http_res_t res = ppage->res;
	ret = mc_pack_put_int32(pp,"result",res.result);	
	if(ret){
		_LOG_WARNING("mc_pack put result failed");
		return -1;
	}

	mc_pack_close(pp);    
	nh->body_len = mc_pack_get_size(pp);

	int sock = connect_pp(req);
	ret = nshead_write(sock, nh, 1000);
	while( ret ){
		_LOG_WARNING("[url:%s] write to pp[ip:%s][port:%d] failed",req->url,req->pp_ip,req->pp_port);
		close( sock );
		Nanosleep(1);
		sock = connect_pp(req);
		ret = nshead_write(sock, nh, 1000);
	}

	//短连接	
	close(sock);
	return 0;
}
#endif

/**
 * @brief 读完一个连接，可能是成功读完/读失败/读超时，发送给pp并释放资源
 *
 **/
int epool::finish_read(page_t* ppage, int64_t now)
{
	assert(ppage);
	assert(ppage->req);

	char *url = ppage->req->url;
	char *ip = ppage->req->ip;

	_LOG_NOTICE("result[%d,%s],url[%s],ip[%s]",ppage->res.result,
			Result_Name[ppage->res.result],url,ip);	

#if 0
	//更新content的长度
	if( ppage->res.cont_len == g_config.max_page_len ){
		ppage->res.cont_len = ppage->cont_pos;
	}
#endif
	int orilen = ppage->res.cont_len;

	ppage->res.cont_len = ppage->cont_pos;


	// 发送统计信息到AB
	//#ifdef _LOGNET
	if(g_config.log2ab){
		char buff[8192];
		if( build_ab_stats_buff(ppage,buff,sizeof(buff),now) == 0 ){
		    TRACE("buff=%s",buff);	
		    _LOG_NET("%s%s%s",_SPLIT,buff,_SPLIT);
		}
	}
	//#endif

	int ret = send_2_pp(ppage,now);

#ifdef _DEBUG
	ppage->show();
#endif

	if( ret <0){
		_LOG_WARNING("send pack 2 pp fail.[url:%s][statu:%d][contlen:%d][contpos:%d]",
				ppage->req->url,ppage->res.statu,ppage->res.cont_len,ppage->cont_pos);	

	} else{
		_LOG_NOTICE("send pack 2 pp ok.[url:%s][statu:%d][contlen:%d][contpos:%d]", ppage->req->url,ppage->res.statu,ppage->res.cont_len,ppage->cont_pos);	
	}

	//只在有连接的情况下才会进入sock!=-1的复合体
	int sock = ppage->fd;
	void* ptr;

	if( sock_map.get(sock, &ptr) != bsl::HASH_EXIST ){
		_LOG_FATAL("why sock[%d] is missing in sock_map",sock);
	}

	if( ptr != (void*)ppage ){
		_LOG_FATAL("why Pointer of page is not equal ptr[%p] ppage[%p]",ptr,ppage);
	}

#if 0
	assert(sock_map.get(sock,&ptr) == bsl::HASH_EXIST);
	assert( ptr == (void*)ppage );
#endif

	//map中删除
	sock_map.erase( sock );			
	close(sock);

	//finish_write没有con_num--，因为还没有建立连接
	con_num--;

	//释放page buffer
	if(ppage->content){
		free(ppage->content);
		_LOG_DEBUG("free content url[%s][orilen:%d][readlen:%d] plus 1 byte", 
				url, orilen, ppage->res.cont_len);
		ppage->content = NULL;
	}

	//释放请求资源
	if(ppage->req){
		free(ppage->req);	
		ppage->req = NULL;
	}

	free(ppage);

	return 0;
} 

epool::epool(){
	is_running = 0;
}

int	epool::init(int _max_conn)
{
	/*
	   pthread_mutex_init(&fill_mutex,NULL);
	   pthread_cond_init(&fill_cond,NULL);	

	   pthread_mutex_init(&map_mutex,NULL);
	 */
	max_conn = _max_conn;
	epfd = epoll_create(max_conn);
	if( epfd < 0 ){
		_LOG_FATAL("creat epoll[%d] err[%m]", epfd	);
		assert(0);
	}

	con_num = 0;
	pack_buff = (char*)malloc(MAX_PACK_LEN);	
	assert(pack_buff);	
	/*	
		pack_buff1 = (char*)malloc(MAX_PACK_LEN);	
		assert(pack_buff1);	

		pack_buff2 = (char*)malloc(MAX_PACK_LEN);	
		assert(pack_buff2);	
	 */
	//	ps = (page_t*)calloc(max_conn,sizeof(page_t));		
	//	assert(ps);

	events = (struct epoll_event *)calloc( max_conn,sizeof(struct epoll_event) );
	assert(events);

	//	fd置位-1
	//	for(int i = 0 ; i<max_conn; i++){
	//		ps[i].fd = -1;	
	//	}

	last_check_time = get_ms();
	show();

	return sock_map.create(20005);
	//return sock_map.create(2*max_conn+5);
}

/*
   epool::epool(int _max_conn):max_conn(_max_conn),con_num(0)
   {

   epfd = epoll_create(max_conn);
   if( epfd < 0 ){
   _LOG_FATAL("creat epoll[%d] err[%m]", epfd	);
   assert(0);
   }

   pack_buff = (char*)malloc(MAX_PACK_LEN);	
   assert(pack_buff);	
//	ps = (page_t*)calloc(max_conn,sizeof(page_t));		
//	assert(ps);

events = (struct epoll_event *)calloc( max_conn,sizeof(struct epoll_event) );
assert(events);

//	fd置位-1
//	for(int i = 0 ; i<max_conn; i++){
//		ps[i].fd = -1;	
//	}

sock_map.create(2*max_conn+5);

pthread_mutex_init(&mutex,NULL);
pthread_cond_init(&cond,NULL);
}
 */

epool::~epool()
{
	free(events);
	close(epfd);
	//	free(ps);
	/*
	   free(pack_buff1);
	   free(pack_buff2);
	   pthread_mutex_destroy(&fill_mutex);
	   pthread_cond_destroy(&fill_cond);

	   pthread_mutex_destroy(&map_mutex);
	 */
}

// 遇到3xx重定向和meta refresh
int epool::handle_redir(page_t *ppage, int64_t now)
{

	// reach MAX redir times and return
	if( ppage->redir_num >= g_config.redir_times){
		_LOG_WARNING("redir too many times[%d]url=%s --> [%s]",ppage->redir_num,ppage->req->url,ppage->redir_trace);

		ppage->res.result = ERR_REDIR;	
		finish_read( ppage, now);
		return -1;
	}

	// send the redir command but no redir url
	if( ppage->res.redir[0] == 0 ){
		_LOG_WARNING("where is your redir[%d]url=%s --> [%s]",ppage->redir_num,ppage->req->url,ppage->redir_trace);

		ppage->res.result = ERR_REDIR;	
		finish_read( ppage, now);
		return -1;
	}

	// change to absolute url
	char tmpurl[MAX_URL_LEN], *p;
	p = get_abs_url(ppage->req->url, ppage->res.redir, tmpurl, sizeof(tmpurl) );	
	if( !p ){
		_LOG_WARNING("related url -> absoluted url failed [%s][%s]",
				ppage->req->url,ppage->res.redir);

		ppage->res.result = ERR_REDIR;	
		finish_read( ppage, now);
		return -1;
	}

	_LOG_TRACE("statu[%d], refresh [%s] --> [%s]",
			ppage->res.statu, ppage->req->url, tmpurl);

	// ip_flag == 0, should udp to get ip, ip_flag = 1, not udp
	int ip_flag = 0 ;
	if( is_same_site(ppage->req->url, tmpurl) ){
		ip_flag = 1;	
	}

	// copy redir to request and make new http request command
	strncpy( ppage->req->url, tmpurl, MAX_URL_LEN);
	ppage->req->url[MAX_URL_LEN-1]=0;

	ppage->redir_num++;

	int trace_len = strlen(ppage->redir_trace);
	if( trace_len + strlen(" --> ") + strlen( ppage->res.redir) < 
			sizeof(ppage->redir_trace) ){
		sprintf(ppage->redir_trace+trace_len," --> %s",ppage->res.redir);	
	}

	if( ip_flag == 0){
		// set ip info
		char *ipaddr = ppage->req->ip;
		ipaddr[0] = 0;

		int iplen = sizeof(ppage->req->ip);

		spi_url_t spi_url;
		if( spi_url_parse( tmpurl, &spi_url ) != 0 ){
			_LOG_WARNING( "[URL:%s] is a bad url", tmpurl);
		}else{
			int robots;	
			if( get_ip_and_robots(tmpurl,ipaddr,iplen, &robots) == 0 ){
				ppage->req->s_robots_real = robots;
			}
			// set port
			ppage->req->port = spi_url.port;
		}
	}

	// clear data
	ppage->read_buf.reset();	
	ppage->cmd_buf.reset();
	//	ppage->res.reset();
	ppage->res.reset_ex();

	if( ppage->content ){
		free(ppage->content);
		ppage->content = 0;
	}

	ppage->cont_pos = 0;
	ppage->crawl_statu = CRAWL_NONE;

	/**
	 * close socket and open new socket
	 * reset the ppage info
	 */
	void* ptr;
	int sock = ppage->fd;

#if 0
	assert( sock_map.get(sock,&ptr) == bsl::HASH_EXIST );
	assert( ptr == (void*)ppage );
#endif

	if( sock_map.get(sock, &ptr) != bsl::HASH_EXIST ){
		_LOG_FATAL("why sock[%d] is missing in sock_map",sock);
	}

	if( ptr != (void*)ppage ){
		_LOG_FATAL("why Pointer of page is not equal ptr[%p] ppage[%p]",ptr,ppage);
	}

	sock_map.erase( sock );			
	close(sock);

	con_num--;

	/** 
	 * handle again 
	 */
	return add_1_sock(ppage, now);
}


int epool::add_1_sock(page_t *ppage, int64_t now)
{
	char *cmd = ppage->cmd_buf.buff;
	req_crawl_t *req = ppage->req;	
	ppage->conn_start = now; 

	// robots forbidden
	if( req->s_robots_real != 0 && req->s_robots_disable == 0 ){
//		_LOG_WARNING("[url:%s][s_robots_real:%d][s_robots_disable:%d] robots forbiden!",
//				req->url, req->s_robots_real, req->s_robots_disable);
		ppage->res.result = ERR_ROBOTS;

		finish_write(ppage, now);
		return -1;
	}

	//int ret = makecmd_x1(cmd,8192,req);
	int ret = makecmd(cmd,8192,req);

	if( ret < 0){
//		_LOG_WARNING("url[%s] makecmd failed!",req->url);
		ppage->res.result = ERR_BAD_URL;

		finish_write(ppage, now);
		return -1;
	}

	ppage->cmd_buf.write_pos=0;
	ppage->cmd_buf.buff_len=strlen(cmd);

	int sock;
	ret = connect_url_nonblock(req,&sock,&(ppage->res.result));
	if( ret < 0 ){
		_LOG_WARNING("url[%s] connect failed!",req->url);
		//ppage->res.result = ERR_CONN;
		finish_write(ppage, now);
		return -1;
	}

	struct epoll_event ev;
	//把要处理的数据记住了
	ev.data.ptr = (void*)ppage; 

	//int64_t now = get_ms();
	if( ret == 1){
		_LOG_TRACE("url[%s] connect in progress",req->url);	
		ppage->crawl_statu = CRAWL_CONNECT;

		//设置要处理的事件类型
		ev.events = EPOLLIN | EPOLLOUT /*| EPOLLHUP | EPOLLERR | EPOLLET*/;
	}else{
		_LOG_TRACE("url[%s] connect success",req->url);	
		ppage->crawl_statu = CRAWL_WRITE_CMD;
		ppage->write_start = now; 

		//设置要处理的事件类型
		ev.events = EPOLLOUT /*| EPOLLHUP | EPOLLERR | EPOLLET*/;
	}

	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &ev);
	if ( ret < 0) {
		_LOG_FATAL("epoll_ctl add [socket:%d] failed. %m", sock);
		close(sock);
		ppage->res.result = ERR_OTHER;

		finish_write(ppage, now);
		return -1;
	}

	ppage->fd = sock;
	sock_map.set(sock,(void*)ppage);
	con_num++;		

	return 0;
}


#define SOCK_FETCH_NUM 1000

int epool::add_n_sock(int64_t now)
{
	int fetch_num = 0;
	for( ; fetch_num < SOCK_FETCH_NUM ; fetch_num++){
		if( con_num >= max_conn){
			_LOG_WARNING("to many connections now");
			show();
			break;
		}

		req_crawl_t* req = NULL;
		int ret = crawl_queue.pop( req);		
		if( ret == -1){
			_LOG_DEBUG("no ready req_crawl_t");
			break;	
		}

		// allocate page_t memory here
		page_t* ppage = (page_t*)calloc(1,sizeof(page_t));
		assert( ppage);
		// ppage->content = 0;
		// ppage->cont_len = 0;
		ppage->req = req;

		// connect...
		add_1_sock(ppage, now);
	}

	if( fetch_num == SOCK_FETCH_NUM){
		_LOG_NOTICE("fetch enough connections[%d] in round of add_sock",fetch_num);
	}

	return 0;
}
const char *strevents (unsigned events,char *buf,int buf_size){
	int off = 0;
	
#define strevents_cat(var) \
	if((events&var)&&(off<buf_size)){ \
		off += snprintf(buf+off,buf_size-off,"%s%s",(off>0?"|":""), #var); \
	}

	strevents_cat(EPOLLIN);
	strevents_cat(EPOLLOUT);
	strevents_cat(EPOLLHUP);
	strevents_cat(EPOLLERR);

	if(off>=buf_size){
		off = buf_size-1;	
	}
	buf[off] = 0;
	return buf;
}

int epool::handle_sock()
{
	//	pthread_mutex_lock(&mutex);

	int64_t now = get_ms();

	add_n_sock(now);

	// 5秒钟checkitem一次，用于干掉超时的socket
	if( now - last_check_time > 5000){
		check_item(now);
		last_check_time = now;
#if 0
		show();
#endif
	}

	int ret;
	int nfds = epoll_wait(epfd, events, max_conn, 10);
	if( nfds < 0 ){
		_LOG_WARNING(" epoll_wait 10ms fail. %m");
		return -1;
	}

	if( nfds == 0){
		_LOG_DEBUG(" epoll_wait timeout in 10 ms");
		return -1;		
	}

	for(int i = 0 ; i < nfds ; i++ )
	{
		page_t *ppage = (page_t*)(events[i].data.ptr);
		assert(ppage);
		char *url = ppage->req->url;
		int sock = ppage->fd;

		// EPOLLHUP or EPOLLERR 
//		if( events[i].events & ( EPOLLHUP | EPOLLERR ) ){
//			_LOG_WARNING("[url:%s] EPOLLHUP | EPOLLERR [err:%d]",
//					url, events[i].events & ( EPOLLHUP | EPOLLERR) );
//			ppage->res.result = ERR_OTHER;
//			finish_read( ppage, now );
//			continue;
//		}

		// for connect
		if( ((events[i].events & EPOLLIN) || (events[i].events & EPOLLOUT)) &&
				ppage->crawl_statu == CRAWL_CONNECT ){
			int error;
			int len = sizeof(error);
			if ( getsockopt(sock, SOL_SOCKET, SO_ERROR, &error, (socklen_t*)&len) < 0) {
				_LOG_WARNING("[url:%s] connect failed! %m",url);
				ppage->res.result = ERR_CONN;
				finish_read( ppage, now);
			}else{
				_LOG_TRACE("[url:%s] connect success",url);
				ppage->crawl_statu = CRAWL_WRITE_CMD;

				struct epoll_event ev;
				//把要处理的数据记住了
				ev.data.ptr = (void*)ppage; 
				ev.events = EPOLLOUT;
				epoll_ctl(epfd, EPOLL_CTL_MOD, sock, &ev);
				ppage->write_start = now;
			}
			continue;
		}

		// for write command
		if( events[i].events & EPOLLOUT ){
			if( ppage->crawl_statu == CRAWL_WRITE_CMD){
				char *cmd = ppage->cmd_buf.buff;
				int wpos = ppage->cmd_buf.write_pos;
				int wlen = ppage->cmd_buf.buff_len;
				ret = write_buffer(sock,cmd,&wpos,wlen);
				if( ret == 0){
					ppage->crawl_statu = CRAWL_READ_HEAD;
					ppage->read_start = now;

					struct epoll_event ev;
					//把要处理的数据记住了
					ev.data.ptr = (void*)ppage; 
					ev.events = EPOLLIN;
					epoll_ctl(epfd, EPOLL_CTL_MOD, sock, &ev);
					continue;
				} else if( ret == 1){
					ppage->cmd_buf.write_pos = wpos;
					continue;
				} else if( ret == -1){
				//	_LOG_WARNING("[url:%s] write failed! %m",url);
					ppage->res.result = ERR_WRITE_FAIL;
					finish_read( ppage, now);
					continue;
				} else{
				//	_LOG_WARNING("[url:%s] write and peer close! %m",url);
					ppage->res.result = ERR_PEER_CLOSE;
					finish_read( ppage, now);
					continue;
				}
			}

			_LOG_WARNING("statu[%d] EPOLLOUT MUST not be here!!!",ppage->crawl_statu);
		}

		if( events[i].events & EPOLLIN){
			int status = ppage->crawl_statu;
			int	read_len;

			//读http response header
			if( status == CRAWL_READ_HEAD ){	
				ret = read_buffer(sock, ppage->read_buf.buff + ppage->read_buf.buff_len , 
						MAX_BUFF_LEN - ppage->read_buf.buff_len, &read_len);
				if( ret == -1 ){
					//除了读socket错误，也有可能是8k的buffer读完了，
					//还没读完http header部分，那就干掉吧
					ppage->res.result = ERR_READ_FAIL;
					finish_read( ppage, now);
					continue;
				} else if ( ret == 2 ){
					ppage->res.result = ERR_PEER_CLOSE;
					finish_read( ppage, now);
					continue;
				} else if( ret == 1){
					//read EAGAIN and continue
					continue;
				}

				//头部信息长度更新
				ppage->read_buf.buff_len += read_len;

				ret = handle_http_head(ppage);
				//状态不是2xx/3xx
				if( ret == -1){
					//_LOG_WARNING("handle_http_head fail and statu code[%d]",ppage->res.statu);
					if( ppage->res.statu / 100 == 4 ){
						ppage->res.result = ERR_4XX;
					}else if( ppage->res.statu / 100 == 5){
						ppage->res.result = ERR_5XX;
					}else{
						ppage->res.result = ERR_OTHER;
					}
					finish_read( ppage, now);
					continue;
				} else if( ret == 1){
					continue;
				}

				// 处理redir
				if( ppage->res.statu / 100 == 3){
					_LOG_TRACE("3xx refresh [%s][http res code:%d]",
							url,ppage->res.statu);
					handle_redir( ppage, now);	
					continue;
				}

				//成功读到头，开始读内容
				status = ppage->crawl_statu = CRAWL_READ_BODY;
			}

			//读body
			if(status == CRAWL_READ_BODY){
				char *meta_redir = ppage->res.redir;
				int	redir_len = sizeof(ppage->res.redir);

				// 可能一次就把response header和html page buffer读完了
				if( ppage->cont_pos >= ppage->res.cont_len ){

					// 处理meta跳转
					if( is_meta_refresh( ppage->content, meta_redir, redir_len) == 0){
						handle_redir( ppage, now);	
						continue;
					} 

					if( ppage->res.statu / 100 == 3 ){
						ppage->res.result = RES_3XX;
					}else{
						ppage->res.result = RES_2XX;
					}

					finish_read( ppage, now);
					continue;
				}

				ret = read_buffer(sock, ppage->content+ppage->cont_pos, 
						ppage->res.cont_len-ppage->cont_pos, &read_len);

				//一直读到对方关闭，读错误或者buffer空
				if( ret == -1){

					// 处理meta跳转
					if( is_meta_refresh( ppage->content, meta_redir, redir_len) == 0){
						_LOG_DEBUG("meta refresh [%s]",url);
						handle_redir( ppage, now);	
						continue;
					} 

					ppage->res.result = ERR_READ_FAIL;
					finish_read( ppage, now);
					continue;
				} else if( ret == 2){

					// 处理meta跳转
					if( is_meta_refresh( ppage->content, meta_redir, redir_len) == 0){
						handle_redir( ppage, now);	
						continue;
					} 

					ppage->res.result = ERR_PEER_CLOSE;
					finish_read( ppage, now);
					continue;
				} else if( ret == 1){
					continue;
				}

				ppage->cont_pos += read_len;
				//抓完了 还有一大部分是对端主动close掉，因为没有content-length
				if( ppage->cont_pos >= ppage->res.cont_len ){

					// 处理meta跳转
					if( is_meta_refresh( ppage->content, meta_redir, redir_len) == 0){
						handle_redir( ppage, now);	
						continue;
					} 

					if( ppage->res.statu / 100 == 3 ){
						ppage->res.result = RES_3XX;
					}else{
						ppage->res.result = RES_2XX;
					}

					finish_read( ppage, now);
					continue;
				} else {
					continue;
				}

			}

			_LOG_WARNING("statu[%d] EPOLLIN MUST not be here!!!",status);
		}
		if (events[i].events & (~ (EPOLLIN | EPOLLOUT))){		
			int socket_error;
			char buf[50];
			socklen_t socket_error_len = sizeof (socket_error);
			WARNING("checkfd:%d, events %X, \"%s\" url=%s",sock,events[i].events,strevents(events[i].events,buf,sizeof(buf)),url);
			
			if (0!=getsockopt(sock, SOL_SOCKET, SO_ERROR, &socket_error, &socket_error_len)) {
				WARNING("getsockopt on sock %d failed, %m url=%s", sock,url);
			}else if(socket_error != 0){
				WARNING("checkfd:%d, socket error(%d) \"%s\" url=%s",sock,socket_error,strerror(socket_error),url);
			}
			ppage->res.result = ERR_OTHER;
			finish_read(ppage,now);
			continue;
		}
	}
	//	pthread_mutex_unlock(&mutex);	

	return 0;		
}


// 放在处理线程做了，每5秒做一次
int	epool::check_item( int64_t now)
{
	for(sock_map_t::iterator itrt = sock_map.begin(); itrt!=sock_map.end();){ 
		page_t* ppage = (page_t*)(itrt->second);
		if( ppage->crawl_statu == CRAWL_NONE ||
				ppage->crawl_statu == CRAWL_CONNECT ){

			if( now - ppage->conn_start> 1000 * g_config.conn_timeout ){

				//_LOG_TRACE("url[%s] connect timeout",ppage->req->url);

				itrt++;
				ppage->res.result = ERR_CONN_TIMEOUT;
				finish_read(ppage, now);
			}else{
				itrt++;
			}

		}else if( ppage->crawl_statu == CRAWL_WRITE_CMD){

			if( now - ppage->write_start > 1000 * g_config.write_timeout ){
				//_LOG_WARNING("url[%s] write cmd timeout,write pos[%d]",
				//		ppage->req->url,ppage->cmd_buf.write_pos);

				itrt++;
				ppage->res.result = ERR_WRITE_TIMEOUT;
				finish_read(ppage, now);
			}else{
				itrt++;
			}


		}else{

			if( now - ppage->read_start > 1000 * g_config.read_timeout ){
				//_LOG_TRACE("url[%s] read timeout,header_off[%d],body_off[%d]",
				//	ppage->req->url,ppage->read_buf.read_pos,ppage->cont_pos);

				itrt++;
				ppage->res.result = ERR_READ_TIMEOUT;
				finish_read(ppage, now);
			}else{
				itrt++;
			}
		}
	}
	return 0;
}


void epool::do_run()
{
	pthread_create(&crawl_thread, NULL, crawl_thread_func, (void*)this);	
	/*
	   pthread_create(&fill_thread, NULL, fill_thread_func, (void*)this);
	   pthread_t mon_thread;
	   pthread_create(&mon_thread, NULL, mon_thread_func, (void*)this);
	 */
}

void epool::wait_end()
{
	/*
	   pthread_join(fill_thread,NULL);
	 */
	if(is_running){
	    pthread_join(crawl_thread,NULL);
	}
}

