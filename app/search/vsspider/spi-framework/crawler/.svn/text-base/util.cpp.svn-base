#include <time.h>
#include <sys/time.h>
#include <ctype.h>
#include <string.h>

#include "init.h"
#include "util.h"

//
//int Nanosleep(int milidelay) 
//{
//	struct timespec s_time;
//
//	if (milidelay <= 0) return 0;
//
//	s_time.tv_sec = milidelay / 1000;
//	s_time.tv_nsec = (milidelay % 1000) * 1000000;
//
//	return nanosleep(&s_time, NULL);
//}

#define is_space_char(ch) (ch=='\r'||ch=='\n'||ch=='\t')
//
//char *str_trim(char *str)
//{
//	if (str == NULL){
//		return NULL;
//	}
//	char *start = str;
//	char *tmp = str;
//	while( *str != 0 && is_space_char(*str) ){
//		str++;
//	}
//	if( *str == 0 ){
//		return str;
//	}
//	while( *str != 0 && !is_space_char(*str) ){
//		*tmp++ = *str++;
//	}
//	*tmp = 0;
//	return start;
//}

char* str_trim_ex(char *str)
{
	if (str == NULL){
		return NULL;
	}
	while( *str != 0 && isspace(*str) ){
		str++;
	}
	if( *str == 0 ){
		return str;
	}

	char *start = str;
	int line_len = strlen(start);
	while ( line_len > 0 && isspace(start[line_len-1]) ){
		line_len --;
		start[line_len] = 0;
	}
	 
	return start;
}

int64_t get_ms()
{
	struct timeval now;
	gettimeofday(&now,NULL);
	return (int64_t)now.tv_sec * 1000 + now.tv_usec/1000;
}

int is_meta_refresh(char *content, char *redir, int redir_len)
{
	if( content == NULL || redir == NULL || redir_len <=0){
		return -1;
	}

	char metabuf[8192];
	char *buf, *start, *end, *tmp;
	start = buf = content;

	while( (start = strcasestr(buf,"<meta")) != NULL ){
	
		end = strchr(start, '>');
		if( end == NULL ){
			_LOG_WARNING("no close tag \">\" for\"<meta\"");
			return -1;
		}

		int len = end - start+1;
		if( len >= 8192 ){
			return -1;
		}
		memcpy(metabuf,start,len);
		metabuf[len] = 0;
		buf = end + 1;
		// puts(metabuf);

		tmp = strcasestr(metabuf,"http-equiv");
		if( tmp == NULL){
			continue;
		}
		tmp += strlen("http-equiv");

		// skip space
		while( *tmp != 0 && isspace(*tmp)){
			tmp++;
		}
		if( *tmp == 0){
			continue;
		}
		if( *tmp != '='){
			continue;
		}
		// skip '='
		tmp ++;

		tmp = strcasestr(tmp,"refresh");
		if(tmp == NULL){
			continue;
		}
		tmp += strlen("refresh");
		
		tmp = strcasestr(tmp,"url");
		if( tmp == NULL ){
			continue;
		}
		tmp += strlen("url");

		// skip space
		while( *tmp != 0 && isspace(*tmp)){
			tmp++;
		}
		if( *tmp == 0){
			continue;
		}
		if( *tmp != '='){
			continue;
		}
		// skip '='
		tmp ++;

		while( *tmp != 0 && 
				(*tmp == '\'' || *tmp == '\"'|| isspace(*tmp)) ){
			tmp++;
		}
		if( *tmp == 0){
			continue;
		} 

		char *stop = strpbrk(tmp, "\'\" \t\n>");
		len = stop - tmp + 1;
		if( len <= 1 || redir_len < len){
			return -1;
		}
		memmove(redir,tmp,len);
		// there is a break char in the end
		redir[len-1] = 0;
		return 0;
	}
	return -1;
}

char* get_abs_url(const char* baseurl, const char* childurl, char *urlbuf, size_t buflen)
{
	uln_url_t url_t;
	int ret;
	
	ret = uln_parse_url( baseurl, &url_t );
	if( ret!=0 ){
    		WARNING("uln_parse_url(baseurl,&url_t)failed.ret=%d,baseurl=%s",ret,baseurl);	    
	    return NULL;
	}

	ret =  uln_resolve_url( &url_t, childurl, urlbuf, buflen);
	if( ret != 0 ){
		WARNING("uln_resolve_url(&url_t,childurl,urlbuf,%lu)failed.ret=%d,baseurl=%s,childurl=%s",
			buflen,ret,baseurl,childurl);	
    	    return NULL;
	}	

	return urlbuf;
}

int get_line(FILE* fp, char* line, int len)
{
	if( fp == NULL || line == NULL || len <= 0 ){
		return -1;  
	}
	if( fgets(line,len,fp) == NULL ){
		return -1;
	}
	line[len-1]=0;
	int line_len = strlen(line);
	while ( line_len > 0 && (line[line_len-1] == '\n' || line[line_len -1] == '\r') ){
		line_len --;
		line[line_len] = 0;
	}
	if( line_len <= 0 ){
		return -2;
	}
	return 0;
}

void p_mcpack(mc_pack_t *mcpack, char *descript)
{
	static char tmpbuf[1280*1024];

	*tmpbuf = '\0';
	mc_pack_pack2text(mcpack, tmpbuf, sizeof(tmpbuf), 0 );
	printf("%s ~@@@~%s~@@@~\n", descript?descript:"(mcpack)", tmpbuf);
}

int is_same_site(char *lhs, char *rhs)
{
/*
	char lhs_site[256];
	char rhs_site[256];
	if( ul_get_site_ex(lhs,lhs_site,sizeof(lhs_site) ) &&
		ul_get_site_ex(rhs,rhs_site,sizeof(rhs_site) ) ){
		if( strcmp(lhs_site,rhs_site) == 0 ){
			return 1;
		}
		return 0;
	}
*/
	spi_url_t lt,rt;
	if( spi_url_parse( lhs, &lt ) != 0 ){
		_LOG_WARNING( "[lhs URL:%s] is a bad url", lhs);
		return 0;
	}

	if( spi_url_parse( rhs, &rt ) != 0 ){
		_LOG_WARNING( "[rhs URL:%s] is a bad url", rhs);
		return 0;
	}

	if( strcmp( lt.site,rt.site ) == 0 ){
		return 1;
	}

	return 0;

}

/**
 * @brief parse a url, ul_parse_url不支持大于256的url
 */
int parse_a_url(const char *url, spi_url_t *out)
{
	assert(url);
	assert(out);

	if( spi_url_parse( url, out ) != 0 ){
		return -1;
	}

	return 0;
}
