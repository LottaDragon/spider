#
# Makefile.public
#
# $Id: Makefile,v 1.21 2009/06/30 10:55:43 zhuhk Exp $
#

include Makefile.env

INCLUDE		= -I. -I../public \
		  -I$(NSHEADINC) \
		  -I$(MCPACKINC) \
		  -I$(PCREINC) \
		  -I$(ULLIBINC) \
		  $(NULL)

LIB		= -L. -L../public -lmisc \
		  $(NSHEAD) \
		  $(MCPACK) \
		  $(PCRE) \
		  $(ULLIB) \
		  -lcrypto -lpthread -lm

CXXFLAGS	= -W -Wall -g -pipe \
		  -DVERSION="\"1.0.0.3\"" \
		  $(INCLUDE) \
		  $(NULL)

CFLAGS		= $(CXXFLAGS)
LDFLAGS		= $(LIB)

target = libmisc.a t_misc

all: $(target) 
	$(MAKE) -C json_spirit

libmisc.a : misc.o mcpack.o ep.o pub_Config.o nsAutoPtr.o common_func.o mmapfile.o \
	mkd.o cooperate.o sendpack.o serv.o rewrite.o
	ar cr $@ $^
	ranlib $@
	make -C json_spirit 

t_misc: t_misc.o
	$(CXX) -o $@ $^ $(LDFLAGS)

t_co: t_co.o
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf *.o *~ *.orig core.* $(target) output
	cd json_spirit;rm -f *.a *.o
	make -C json_spirit clean

tags : $(wildcard *.cpp *.c *.h)
	ctags -R *.cpp *.h ../public

# EOF
