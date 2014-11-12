#include "receiver.h"
#include "handler.h"
#include "init.h"
#include "misc.h"

// 版本和编译时间信息
#ifdef __VERSION_ID__
static const char version_id[] = __VERSION_ID__;
#else
static const char version_id[] = "unknonw";
#endif

#if defined (__DATE__) && defined (__TIME__)
static const char time_built[] = __DATE__ " " __TIME__;
#else
static const char time_built[] = "unknown";
#endif

void print_version()
{
	printf ("--------------------------------------\n");
	printf ("     Module: vs-spider crawler\n");
	printf ("    Version: %s\n", version_id);
	printf (" Time Built: %s\n", time_built);
	printf (" Bug Report: yueyifan@baidu.com\n" );
	printf ("--------------------------------------\n");
}

int main(int argc, char **argv)
{

	char opt;
	int sock=-1;
	epool *pool = NULL;
	while( (opt = getopt(argc,argv,"v")) != -1 ) {
		switch( opt ) {
			case 'v':
				print_version();	
				exit(1);
			default:
				break;
		}
	}


	// 1.初始化
	int ret = initial();
	if( ret < 0 ){
		_LOG_FATAL("load config failed");
		return -1;
	}

	sock = ul_tcplisten(g_config.listen_port, 5);
	if ( sock < 0 ) {
		_LOG_FATAL("ul_tcplisten failed, %m");
		goto err_quit;
	}

	// 2.创建并初始化抓取池，并启动抓取线程
	pool = (epool*)calloc( g_config.thread_num,sizeof(epool) );
	if(pool==NULL){
		FATAL("calloc(%d,%lu)failed.%m",g_config.thread_num,sizeof(epool));
		goto err_quit;
	}
	for(int i =0; i < g_config.thread_num; i++){
		if( pool[i].init( g_config.max_conn_num) == 0 ){
			pool[i].do_run();
		}else{
			_LOG_FATAL("pool[%d] init failed and program exit!",i);
			goto err_quit;
		}
	}

	_LOG_NOTICE("CRAWLER listen successfully ...");

	// 3.上游数据管理模块初始化
	init_conn_mgr(sock);

	// 4.接受数据并丢进阻塞队列共抓取池处理
	do_receive();

	// 5.优雅带超时退出comlog
	com_closelog(50000);

	// 你不能够到这里
	// assert(0);

	return 0;

err_quit:
	g_running = 0;
	if(pool){
		for(int i=0;i<g_config.thread_num;i++){
			pool[i].wait_end();
		}
	}	    
	com_closelog(50000);
	if(sock>0){
		close(sock);
	}
	return -1;
}
