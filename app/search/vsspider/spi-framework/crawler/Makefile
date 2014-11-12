include ../public/Makefile.env

WORKROOT=$(basedir)

ifeq ($(MAC), 64)
  USERFLAGS = -fPIC
  BASEOBJ = receiver.o handler.o init.o util.o ipdict.o
else
  USERFLAGS = -DKERNEL24
  BASEOBJ = receiver.o handler.o init.o epoll.o util.o ipdict.o 
endif

DEFINES = -D_XOPEN_SOURE=500 -D_GNU_SOURCE -ftemplate-depth-128 -D__VERSION_ID__="\"crawler 1.0.2.5\""

GCC = g++

CPPFLAGS = -g $(USERFLAGS) -finline-functions -Wall -Winline -pipe $(DEFINES)

INCLUDES = -I../public -I$(ULLIBINC) -I$(DICTINC) -I$(BSLINC) -I$(MCPACKINC) -I$(NSHEADINC) \
	   -I$(SPIDNSINC) -I$(PCREINC) -I$(URLPARSERINC) -I$(CCODEINC)

LDFLAGS = -L../public -lmisc $(BSL) $(MCPACK) $(NSHEAD) $(SPIDNS) $(DICT) $(PCRE) $(URLPARSER) $(CCODE) $(ULLIB) \
	  $(TCMALLOC) -lm -lpthread -lz -lcrypto

OUTPUT = crawler nullcrawler test mocker_pp filecrawler xclient shuffle_url

all	: $(OUTPUT) 
	rm -rf output;mkdir -p output
	cp $(OUTPUT) crawler.conf crwlog.conf comlogsvr* output
	rm -f *.o

%.o : %.cpp
	$(GCC) $(CPPFLAGS) -c $< $(INCLUDES)

crawler : crawler.o $(BASEOBJ)
	$(GCC) -o $@ $^  $(LDFLAGS) 

filecrawler : filecrawler.o $(BASEOBJ)
	$(GCC) -o $@ $^ $(LDFLAGS) 

nullcrawler : nullcrawler.o
	$(GCC) -o $@ $^ $(LDFLAGS) 

test : test.o $(BASEOBJ)
	$(GCC) -o $@ $^ $(LDFLAGS) 

mocker_pp : mocker_pp.o util.o 
	$(GCC) -o $@ $^ $(LDFLAGS) 

xclient : xclient.o util.o
	$(GCC) -o $@ $^ $(LDFLAGS) 

shuffle_url: shuffle_url.o util.o
	$(GCC) -o $@ $^ $(LDFLAGS) 

epoll.o: epoll.c
	gcc -c $< -o $@

clean:
	rm -f *.o $(OUTPUT)
	rm -rf output

tags : $(wildcard *.cpp *.c *.h)
	ctags -R *.cpp *.h $(PUBLIC) $(WORKROOT)/libsrc/ullib
