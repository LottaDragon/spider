#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

#include "cooperate.h"
#include "misc.h"

int main (int argc, char **argv){
	spi_url_t spi_url;
	char url[SPI_MAX_URL_LEN];
	int ret;
	
	if(argc!=2||argv[1][0]=='-'){
	#ifdef VERSION
		printf("VERSION=%s\n",VERSION);
	#endif
		printf("Usage: %s <url>\n",argv[0]);
		return -1;
	}
	ret = spi_url_parse(argv[1],&spi_url);
	if(ret!=0){
		return -1;
	}
	spi_url_sprint(&spi_url,url,sizeof(url));
	printf("old=%s\nnew=%s\n",argv[1],url);
	spi_url_print(&spi_url);

    return 0;
}
