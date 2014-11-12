/**
 * 混排url，随机分布url列表
 * 利用hash特点输出
 */


#include "stdio.h"

#include "util.h"
#include "ipdict.h" /// for bstring_hash

#include "bsl/containers/hash/bsl_hashset.h"

int main(int argc, char **argv)
{
	if( argc < 2 ){
		fprintf(stderr,"usage:%s $urlfile\n",argv[0]);
		exit(1);
	}
	FILE *fp = fopen(argv[1],"r");
	if(!fp){
		fprintf(stderr,"can not open file[%s]\n",argv[1]);
		exit(1);
	}

	typedef bsl::hashset<bstring,bstring_hash> url_set_t;
	url_set_t urlset;
	urlset.create(3003001);

	char line[2048];
	while( get_line(fp,line,sizeof(line) ) ==0 ){
		bstring str(line);		
		if( urlset.get(str) == bsl::HASH_NOEXIST ){
			urlset.set(str);
		}
	}	

	for(url_set_t::iterator itrt = urlset.begin();
		itrt != urlset.end();
		itrt ++){
		printf("%s\n",(*itrt).c_str());	
	}

	fclose(fp);	
	return 0;
}

