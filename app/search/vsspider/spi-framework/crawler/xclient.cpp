/**
 * 演示怎么写一个crawler的请求发送上游和页面接受下游
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "ul_url.h"
#include "ul_net.h"

#include "nshead.h"
#include "mc_pack.h"

//#include "../public/vs-spider.h"
#include "vs-spider.h"
#include "util.h"
#include "init.h"

#include "spidns.h"


volatile int b_send;
volatile int n_send;
volatile int n_recv;


char *crw_ip; 
int crw_port;
char *pp_ip;
int pp_port;

FILE *fpout;


const char *dns_ip = "db-spi-dns4.db01.baidu.com";
const int dns_port = 1280;

// dns服务器地址
static sockaddr_in serv_addr;

/**
 * @brief 词典初始化
 * @return int 初始化结果
 * @retval -1失败；0成功
 **/
static int init(){

	bzero(&serv_addr, sizeof(sockaddr_in));
	if (ul_gethostipbyname_r( dns_ip, &serv_addr, BD_NET, NULL) != 0){
		_LOG_FATAL("Resolve DNS server name (%s) fail. %m", dns_ip);
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons( dns_port);

	return 0;
}



void usage()
{
	fprintf(stdout,
			"usage: ./xclient $urlfile $crawler_ip $crawler_port $pp_ip $pp_port\n");
	exit(1);
}


int connect_crawler()
{
	int sock = ul_tcpconnecto_ms(crw_ip,crw_port,1000);
	while( sock < 0){
		_LOG_WARNING("connenct crawler [%s][%d] failed. %m",
				crw_ip,crw_port);
		sock = ul_tcpconnecto_ms(crw_ip,crw_port,1000);
	}
	return sock;
}

void* sendurl(void* arg)
{
	int sock = connect_crawler();
	char line[2048];
	char pack_buff[16*1024];

	char tmpbuf[10240];

	FILE *fp = (FILE*)arg;

	while( get_line(fp,line,sizeof(line)) == 0){

		//控制一下速度，悠着点
		if(n_send % 125 ==0){
			sleep(1);
		}

		//usleep(15000);

		nshead_t* nh = (nshead_t*)pack_buff;

		char *buf = (char*)(nh+1);
		int buflen = sizeof(pack_buff)-sizeof(nshead_t);

		mc_pack_t *pp = mc_pack_open_w(2, buf, buflen, tmpbuf, sizeof(tmpbuf));

		//		mc_pack_t *pp = mc_pack_create( (char*)(nh+1), sizeof(pack_buff) - sizeof(nshead_t) );
		if( MC_PACK_PTR_ERR(pp) ){     
			_LOG_WARNING("create pack failed");
			continue;
		}  

		//		if( ! mc_pack_valid(pp, sizeof(pack_buff) - sizeof(nshead_t))){
		//			_LOG_WARNING("not valid cc_2_crawler pack");
		//			continue;
		//		}                      

		char site[256];
		if( ul_get_site_ex(line,site,256) ){
			siteinfo_t siteinfo;
			init_siteinfo(siteinfo);

			int ret = spidns_seeksite_udp((sockaddr*)(&serv_addr),sizeof(serv_addr),
					site,siteinfo,200);

			if( ret == 0 && siteinfo.ip_num > 0){
				char iptmp[64];
				struct in_addr addr;
				addr.s_addr = siteinfo.ip[0];

				if( inet_ntop(AF_INET,&addr,iptmp, INET_ADDRSTRLEN) ){

					mc_pack_put_uint32(pp,"Pack_type",/*PACK_LINKDB_SEEK*/PACK_CC2CR);
					mc_pack_put_uint32(pp,"session_id",1);
					mc_pack_put_int32(pp,"site_id",1);
					mc_pack_put_str(pp,"url",line);

					mc_pack_put_str(pp,"pp_ip",pp_ip);
					mc_pack_put_int32(pp,"pp_port",pp_port);

					mc_pack_put_str(pp,"s_ipaddr",iptmp);
					mc_pack_put_int32(pp,"s_port",80);

					mc_pack_put_str(pp,"http_version","HTTP/1.0");
					mc_pack_close(pp);
					nh->body_len = mc_pack_get_size(pp);

					int ret = nshead_write(sock, nh, 1000);
					while( ret ){
						close( sock );
						sock = connect_crawler();
						ret = nshead_write(sock, nh, 1000);
					}

					n_send++;
					_LOG_NOTICE("send url[%s][%d] to crawler",line,n_send);
				}
			}
		}
	}

	b_send = 0;

	return NULL;
}

char _buff[1200*1024];
void handle_crawl_result(int sock)
{
	nshead_t *nh = (nshead_t *)_buff;                         
	char tmpbuf[10240];

	int ret = nshead_read( sock, nh, sizeof(_buff), 5000);
	if( ret < 0 ){            
		_LOG_WARNING("read failed!%s,%s",nshead_error(ret),strerror(errno));                    
		return;                    
	}  

	char *buf = (char*)(nh+1);
	int buflen = sizeof(_buff)-sizeof(nshead_t);

	mc_pack_t *p = mc_pack_open_r( buf, buflen, tmpbuf, sizeof(tmpbuf));

	if( MC_PACK_PTR_ERR(p) ){     
		_LOG_WARNING("create pack failed");
		return;
	}  

	//	mc_pack_t *p = (mc_pack_t *)(nh+1);                     
	//
	//	if(!mc_pack_valid(p, nh->body_len)){
	//		_LOG_WARNING("not a valid pack!");
	//		return;
	//	}  

	p_mcpack(p,"crawl result:");

	char *url;
	int statu;
	if( mc_pack_getf(p,"c_statu:%d,url:%p",&statu,&url) == 2 ){
		_LOG_NOTICE("url:%s,statu:%d,no:%d",url,statu,n_recv);	

		char *sp = "+++++++++++++++++++++++++++++++++\n";
		fwrite(sp,strlen(sp),1,fpout);

		char meta[1024];
		snprintf(meta,sizeof(meta),"url:%s,statu:%d,no:%d\n",
				url,statu,n_recv);
		meta[1023] = 0;
		fwrite(meta,strlen(meta),1,fpout);

		if( statu / 100 == 2 || statu / 100 == 3){
			uint32_t pagelen;
			const void *rbuf = mc_pack_get_raw(p,"c_page",&pagelen);	
			if( MC_PACK_PTR_ERR(rbuf) != 0 ){
				_LOG_WARNING("get c_page failed url[%s]",url);
				return;
			}else{
				fwrite(rbuf,pagelen,1,fpout);
#if 0
				char tmpstr[64];
				snprintf(tmpstr,64,"%d.htm",n_recv);
				FILE *fp = fopen(tmpstr,"w");
				if(!fp){
					return;
				}
				fwrite(rbuf,pagelen,1,fp);
				fclose(fp);
#endif
			}
		}
		fwrite(sp,strlen(sp),1,fpout);
	}else{
		_LOG_WARNING("no c_statu & url, are you kidding me");	
		return;
	}
}

int main(int argc, char **argv)
{
	if( argc != 6 ){
		usage();	
	}	

	FILE *fp = fopen(argv[1],"r");
	if(!fp){
		fprintf(stderr,"open file[%s] failed. %m",argv[1]);
		exit(1);
	}

	init();

	fpout = fopen("all.html","a");
	if(!fpout){
		fprintf(stderr,"open file [all.html] failed. %m");
		exit(1);
	}

	signal(SIGHUP, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);

	int ret = com_loadlog(".", "conf/comlog.conf");
	if( ret < 0){
		exit(1);
	}

	crw_ip = argv[2];
	crw_port = atoi(argv[3]);

	pp_ip = argv[4];
	pp_port = atoi(argv[5]);

	b_send = 1;
	n_send = n_recv = 0;

	int pp_sock = ul_tcplisten( pp_port, 20 );
	if( pp_sock < 0 ){
		fprintf(stderr,"listen on socket[%d] failed. %m\n",pp_port);
		exit(1);
	}
	_LOG_NOTICE("listen on socket[%d] ... ",pp_port);

	pthread_t thr;
	if( pthread_create(&thr,NULL,sendurl,fp) != 0 ){
		fprintf(stderr,"create sendurl thread failed. %m\n");
		close(pp_sock);
		exit(1);
	}

	_LOG_NOTICE("create thread for send url ... ");

	struct sockaddr_in addr;
	socklen_t addrlen;

	while ( b_send || n_recv < n_send) {
		int new_sock = accept (pp_sock, (struct sockaddr *)&addr, &addrlen);
		if (new_sock == -1) {
			switch (errno) {
				case EINTR:
					continue;
				default:
					break;
			}
			fprintf(stderr,"accept failed, %m\n");
			continue;
		}

		n_recv++;

		handle_crawl_result(new_sock);
		//短连接
		close(new_sock);
	}


	close(pp_sock);
	fclose(fp);
	fclose(fpout);

	return 0;
}

