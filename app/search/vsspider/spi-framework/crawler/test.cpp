#include <assert.h>
#include <iostream>
#include <string.h>
#include <signal.h>

#include <pthread.h>

#include "util.h"
using namespace std;

#include "uln_url.h"

#include "spidns.h"

#include "init.h"
#include "ipdict.h"
#include "block_queue.h"

void t_dns(char *path){

	signal(SIGPIPE,SIG_IGN);

	strcpy(g_config.dns_server,DEFAULT_DNS_SERVER);
	g_config.dns_udp_port = DEFAULT_DNS_UDP_PORT;

	init_ipdict();

	FILE *fp = fopen(path,"r");
	if(!fp) return;
	
	char line[1024];
	while( fgets(line,1024,fp) != NULL ){
		char *p = str_trim_ex(line);		
		printf("seek ip [%s]\n",p);	
		char ip[32];
		
		if( url2ip(p,ip,32) == 0 ){
			printf("%s\n",ip);		
		}else{
			printf("NONE\n");	
		}
	}

	fclose(fp);
	info_ipdict();	
}

void t4()
{
	char buf[2048];
	char *baseurl = "http://www.baidu.com/x/xx.html";
	cout << baseurl << endl;

	char *r;
	r = "../debug.html";
	cout<< r << endl;
	cout<< get_abs_url(baseurl,r,buf,sizeof(buf)) << endl;
	r = "debug.html";
	cout<< r << endl;
	cout<< get_abs_url(baseurl,r,buf,sizeof(buf)) << endl;
	r = "./debug.html";
	cout<< r << endl;
	cout<< get_abs_url(baseurl,r,buf,sizeof(buf)) << endl;
	r = "http://www.sina.com.cn/index.html";
	cout<< r << endl;
	cout<< get_abs_url(baseurl,r,buf,sizeof(buf)) << endl;
	r = "a/b/c/../bb/debug.html";
	cout<< r << endl;
	cout<< get_abs_url(baseurl,r,buf,sizeof(buf)) << endl;


	r = "  debug.html";
	cout<< r << endl;
	cout<< get_abs_url(baseurl,r,buf,sizeof(buf)) << endl;
	r = "%20debug.html";
	cout<< r << endl;
	cout<< get_abs_url(baseurl,r,buf,sizeof(buf)) << endl;
	r = "°Ù¶È";
	cout<< r << endl;
	cout<< get_abs_url(baseurl,r,buf,sizeof(buf)) << endl;
	r = "www.sina.com.cn/index.html";
	cout<< r << endl;
	cout<< get_abs_url(baseurl,r,buf,sizeof(buf)) << endl;
	r = "a/b/,.^&*/debug.html";
	cout<< r << endl;
	cout<< get_abs_url(baseurl,r,buf,sizeof(buf)) << endl;

	cout << endl;
}


void t3()
{

	char *buff = 
		"<html><head>"
		"<meta   \r\n http-equiv=\"Refresh\" \t  \n\r content=\"3 ;    url = debug.html   \">"
		"</head><body>"
		"<p>Please follow <a href=\"http://www.baidu.com/\">link</a>!</p>"
		"</body></html>";
	char redir[2048];
	assert( is_meta_refresh(buff,redir,2048) == 0  );
	cout<<redir<<endl;

	char *url = "news.baidu.com/test.html";
	uln_url_t url_t;
	if( uln_parse_url(url, &url_t) != 0 ){
		cout<<"Can not uln_parse_url "<<url<<endl;
		return;
	}

	char buf[2048];
	if( uln_resolve_url(&url_t, redir, buf, sizeof(buf)) != 0 ){
		cout<<"Can not resolve url ["<<url<<","<<redir<<"]"<<endl;
		return;
	}	
	cout<< buf<<endl;
}

void t1()
{
	char *buff = 
		"<html><head>"
		"<meta   \r\n http-equiv=\"Refresh\" \t  \n\r content=\"3 ;    url = debug.html   \">"
		"</head><body>"
		"<p>Please follow <a href=\"http://www.baidu.com/\">link</a>!</p>"
		"</body></html>";

	char *buff1 = 
		"<html><head>"
		"<meta http-equiv=Refresh content=\"3;url=debug.html\">"
		"</head><body>"
		"<p>Please follow <a href=\"http://www.baidu.com/\">link</a>!</p>"
		"</body></html>";

	char *buff2 = 
		"<html><head>"
		"<meta do what you wanna>"
		"<meta http-equiv=Refresh content=3;urlformat=>"
		"<meta http-equiv=Refresh content=3;url=debug.html>"
		"</head><body>"
		"<p>Please follow <a href=\"http://www.baidu.com/\">link</a>!</p>"
		"</body></html>";

	char *buff3 = 
		"<html><head>"
		"<meta http-equiv=Refresh content=3;url=debug.html do=what you wanna>"
		"</head><body>"
		"<p>Please follow <a href=\"http://www.baidu.com/\">link</a>!</p>"
		"</body></html>";
	
	char *buff4 = 
		"<html><head>"
		"<meta http-equiv=Refresh \"urlcontent=3\">"
		"<meta http-equiv=Refresh content=3; url = 'debug.html' >"
		"</head><body>"
		"<p>Please follow <a href=\"http://www.baidu.com/\">link</a>!</p>"
		"</body></html>";

	char *buff5 = 
		"<html><head>"
		"<meta haha \"urlcontent=3\">"
		"<meta haha content=3; url = 'debug.html' >"
		"</head><body>"
		"<p>Please follow <a href=\"http://www.baidu.com/\">link</a>!</p>"
		"</body></html>";

	char *buff6 = 
		"<html><head>"
		"</head><body>"
		"<p>Please follow <a href=\"http://www.baidu.com/\">link</a>!</p>"
		"</body></html>";

	char redir[2048];
	assert( is_meta_refresh(buff,redir,2048) == 0  );
	cout<<redir<<endl;
	assert( is_meta_refresh(buff1,redir,2048) == 0  );
	cout<<redir<<endl;
	assert( is_meta_refresh(buff2,redir,2048) == 0  );
	cout<<redir<<endl;
	assert( is_meta_refresh(buff3,redir,2048) == 0  );
	cout<<redir<<endl;
	assert( is_meta_refresh(buff4,redir,2048) == 0  );
	cout<<redir<<endl;
	assert( is_meta_refresh(buff5,redir,2048) == -1  );
	cout<<redir<<endl;
	assert( is_meta_refresh(buff6,redir,2048) == -1  );
	cout<<redir<<endl;
}

void t2()
{
	char s[20] = "\t\n\r    ";
	assert(str_trim(s) == NULL);
	char s1[20] = "  \tangel\r\n ";
	char* r1 = str_trim(s1);
	cout<< r1 << "len:"<<strlen(r1)<<endl;
	assert( *r1 == 'a');
	assert( *s1 == 'a' );

	char s2[20] = " \tangel\r\n ";
	char *r2 = str_trim_ex(s2);

	cout<< r2 << "len:"<<strlen(r2)<<endl;
	assert( *r2 == 'a');
	assert( *s2 == ' ');


	char str[100] = "ab ab ab ab ab ab ab";
	str[8] = 0;

	char *r = strstr(str,"ab");
	cout << r << endl;

	r = strstr( str+8, "ab" );
	cout << r << endl;

	char term[10] = {0};
	assert( strstr(term,"angel") == NULL );
	assert( strcasestr(term,"angel") == NULL );

}

void t_q()
{
	block_queue_t<int> q(1000);		
	for(int i = 0 ; i < 2000; i++){
		int ret = q.push(i);
		if( ret == 0 ){
			cout<<"push:"<<i<<endl;
			q.info();
		}
	}
}

void* provide(void *arg)
{
	block_queue_t<int> *q = (block_queue_t<int> *)arg;
	while(1){
		srand(time(NULL));
		int i = rand()%100;
		q->push(i);
		_LOG_NOTICE("pid:%d,pop:%d",getpid(),i);
		q->info();
	}

	return NULL;
}

void* consume(void *arg)
{
	block_queue_t<int> *q = (block_queue_t<int> *)arg;
	while(1){
		int n;
		q->pop(n);
		srand(time(NULL));
		int i = rand()%100;
		usleep(i);
		_LOG_NOTICE ("pid:%d,push:%d,",getpid(),n);
		q->info();
	}

	return NULL;
}


#if 0 
int main(int argc,char **argv)
{
//	t3();
//	cout<<"finish t3()"<<endl;
//	t4();
	//t2();
	//t1();
	
	//t_dns(argv[1]);
	//t_q();
	block_queue_t<int> q(1000);		
	for(int i = 0 ; i < 900; i++){
		q.push(i);	
	}

	pthread_t pt,ct;
	pthread_create(&pt,NULL,provide,&q);
	pthread_create(&ct,NULL,consume,&q);

	pthread_join(pt,NULL);
	pthread_join(ct,NULL);

	return 0;
}
#endif

void t_refresh()
{
	char s[256] = " http://www.nbxybz.com/";
	char p[256] = " cn/index.html ";
	printf("%s\n",s);
	printf("%s\n",p);

	char *q = str_trim_ex(p);
	printf("%s\n",q);
	
	char redir[1024];
	get_abs_url(s,q,redir,sizeof(redir));
	printf("%s\n",redir);
}

void t_s()
{
	char *a = "www.baidu.com/1.htm";
	char *b = "www.baidu.com/1.jsp";
	printf("(%s,%s) = %d\n", a,b,is_same_site(a,b));

	a = "www.baid.com/1.htm";
	b = "www.baidu.com/1.jsp";
	printf("(%s,%s) = %d\n", a,b,is_same_site(a,b));

	a = " www.baid.com/1.htm";
	b = "www.baidu.com/1.jsp/2/3/../4.htm";
	printf("(%s,%s) = %d\n", a,b,is_same_site(a,b));

	a = "www.baidu.com/1.htm";
	b = "www.baidu.com/1.jsp/2/3/../4.htm";
	printf("(%s,%s) = %d\n", a,b,is_same_site(a,b));
}

void t_url()
{
	char *a = "http://www.baidu.com/1.htm";
	char *b = "www.baidu.com/1.htm";
	char *c = "http://www.sina.com.cn:8080/12/1212.htm";
	char *d = "http://www.sina.com.cn/12/1212.htm?a=123&b=89";

	int ret = is_same_site(a,b);
	printf("%d\n",ret);
	ret = is_same_site(a,c);
	printf("%d\n",ret);

	uln_url_t t;
	if( uln_parse_url( c, &t ) == 0 ){
		printf("%s\n",c);	
		printf("host:%s\n",t.host);
		printf("port:%d\n",t.port);
		printf("path:%s\n",t.path);
		printf("query:%s\n",t.query);
	}

	if( uln_parse_url( d, &t ) == 0 ){
		printf("%s\n",d);	
		printf("host:%s\n",t.host);
		printf("port:%d\n",t.port);
		printf("path:%s\n",t.path);
		printf("query:%s\n",t.query);
	}

}

int t_url1(int argc, char **argv)
{
	uln_url_t t;
	char path[2048];
	if( uln_parse_url( argv[1], &t ) == 0 ){
		printf("url:%s\n",argv[1]);	
		printf("scheme:%s\n",t.scheme);
		printf("userinfo:%s\n",t.userinfo);
		printf("host:%s\n",t.host);
		printf("port:%d\n",t.port);
		printf("path:%s\n",t.path);
		printf("query:%s\n",t.query);
		printf("fragment:%s\n",t.fragment);
	}

	int ret = uln_get_url_segment(argv[1], SEG_URL_FILE, path, sizeof(path));
	if(ret == 0 ){
		printf("file path:%s\n",path);
	}

	return 0;
}

int main(int argc, char **argv)
{
//	t_refresh();

//	t_s();
//	t_url();
	t_url1(argc,argv);

	return 0;
}

