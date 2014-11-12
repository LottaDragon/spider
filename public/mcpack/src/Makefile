##
## $Id: Makefile,v 1.1.2.20 2009/03/02 09:26:51 yufan Exp $
## mc_pack Makefile
##

CC = gcc
C++ = g++
LIB = libmcpack.a
SHELL = /bin/sh
OBJS = mc_pack.o mc_pack2.o mc_pack1.o mc_pack_core.o mc_pack_text.o mc_pack_ex.o 
RANLIB = ranlib

INCFLAGS = -I../include
CFLAGS = -g -O2 -Wall -minline-all-stringops -Winline -pipe -Wreturn-type -Wtrigraphs -Wformat -Wparentheses -Wpointer-arith -Wsign-compare -fPIC \
		 -DVERSION="\"mcpack 1.0.1.4\""	-DCVSTAG="\"mcpack_1-0-1-4_PD_BL\"" -D_GNU_SOURCE -Werror
CPPCFLAGS = -g -O2 -Wall -minline-all-stringops -Winline -pipe -Wreturn-type -Wtrigraphs -Wformat -Wparentheses -Wpointer-arith -Wsign-compare -fPIC \
		 -DVERSION="\"mcpack 1.0.1.4\""	-DCVSTAG="\"mcpack_1-0-1-4_PD_BL\"" -D_GNU_SOURCE -Werror
LDFLAGS =

CPPUNIT_INC = 
CPPUNIT_LD = -L./ -ldl -lmcpack

all: lib

ccp:
	ccp --formatter vim
mc_pack.o: mc_pack.cpp ../include/mc_pack_bits/mc_pack2.h \
  ../include/mc_pack_bits/../mc_pack.h \
  ../include/mc_pack_bits/../mc_pack_def.h \
  ../include/mc_pack_bits/../mc_pack_c.h \
  ../include/mc_pack_bits/../mc_pack_text.h \
  ../include/mc_pack_bits/../mc_pack.h \
  ../include/mc_pack_bits/../mc_pack_ex.h \
  ../include/mc_pack_bits/../mc_pack_core.h \
  ../include/mc_pack_bits/mc_pack_cpp.h \
  ../include/mc_pack_bits/../mc_pack_def.h \
  ../include/mc_pack_bits/../mc_pack_text.h \
  ../include/mc_pack_bits/../mc_pack_ex.h \
  ../include/mc_pack_bits/allocator.h ../include/mc_pack_bits/typelist.h \
  ../include/mc_pack_bits/indexer.h ../include/mc_pack_bits/mc_pack1.h \
  ../include/mc_pack_bits/../mc_pack_core.h
mc_pack1.o: mc_pack1.cpp ../include/mc_pack.h ../include/mc_pack_def.h \
  ../include/mc_pack_c.h ../include/mc_pack_text.h ../include/mc_pack.h \
  ../include/mc_pack_ex.h ../include/mc_pack_core.h \
  ../include/mc_pack_bits/mc_pack1.h ../include/mc_pack_bits/../mc_pack.h \
  ../include/mc_pack_bits/mc_pack_cpp.h \
  ../include/mc_pack_bits/../mc_pack_def.h \
  ../include/mc_pack_bits/../mc_pack_text.h \
  ../include/mc_pack_bits/../mc_pack_ex.h \
  ../include/mc_pack_bits/allocator.h ../include/mc_pack_bits/typelist.h \
  ../include/mc_pack_bits/indexer.h \
  ../include/mc_pack_bits/../mc_pack_core.h ../include/mc_pack_core.h
mc_pack2.o: mc_pack2.cpp ../include/mc_pack_bits/mc_pack2.h \
  ../include/mc_pack_bits/../mc_pack.h \
  ../include/mc_pack_bits/../mc_pack_def.h \
  ../include/mc_pack_bits/../mc_pack_c.h \
  ../include/mc_pack_bits/../mc_pack_text.h \
  ../include/mc_pack_bits/../mc_pack.h \
  ../include/mc_pack_bits/../mc_pack_ex.h \
  ../include/mc_pack_bits/../mc_pack_core.h \
  ../include/mc_pack_bits/mc_pack_cpp.h \
  ../include/mc_pack_bits/../mc_pack_def.h \
  ../include/mc_pack_bits/../mc_pack_text.h \
  ../include/mc_pack_bits/../mc_pack_ex.h \
  ../include/mc_pack_bits/allocator.h ../include/mc_pack_bits/typelist.h \
  ../include/mc_pack_bits/indexer.h
mc_pack_factory.o: mc_pack_factory.cpp \
  ../include/mc_pack_bits/mc_pack_factory.h \
  ../include/mc_pack_bits/mc_pack1.h ../include/mc_pack_bits/../mc_pack.h \
  ../include/mc_pack_bits/../mc_pack_def.h \
  ../include/mc_pack_bits/../mc_pack_c.h \
  ../include/mc_pack_bits/../mc_pack_text.h \
  ../include/mc_pack_bits/../mc_pack.h \
  ../include/mc_pack_bits/../mc_pack_ex.h \
  ../include/mc_pack_bits/../mc_pack_core.h \
  ../include/mc_pack_bits/mc_pack_cpp.h \
  ../include/mc_pack_bits/../mc_pack_def.h \
  ../include/mc_pack_bits/../mc_pack_text.h \
  ../include/mc_pack_bits/../mc_pack_ex.h \
  ../include/mc_pack_bits/allocator.h ../include/mc_pack_bits/typelist.h \
  ../include/mc_pack_bits/indexer.h \
  ../include/mc_pack_bits/../mc_pack_core.h \
  ../include/mc_pack_bits/mc_pack2.h ../include/mc_pack_bits/allocator.h
mc_pack_core.o: mc_pack_core.c ../include/mc_pack_core.h \
  ../include/mc_pack.h ../include/mc_pack_def.h ../include/mc_pack_c.h \
  ../include/mc_pack_text.h ../include/mc_pack_ex.h \
  ../include/mc_pack_core.h
mc_pack_ex.o: mc_pack_ex.c ../include/mc_pack_ex.h \
  ../include/mc_pack_core.h ../include/mc_pack.h ../include/mc_pack_def.h \
  ../include/mc_pack_c.h ../include/mc_pack_text.h \
  ../include/mc_pack_ex.h
mc_pack_text.o: mc_pack_text.c ../include/mc_pack_text.h \
  ../include/mc_pack.h ../include/mc_pack_def.h ../include/mc_pack_c.h \
  ../include/mc_pack_text.h ../include/mc_pack_ex.h \
  ../include/mc_pack_core.h ../include/mc_pack_core.h

lib: $(LIB)
	mkdir -p ../lib/
	cp $(LIB) ../lib/

$(LIB): $(OBJS)
	-rm -f $@
	ar cr $@ $(OBJS)
	$(RANLIB) $@

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@ $(INCFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCFLAGS)

%.o: %.cpp
	$(C++) $(CFLAGS) -c $< -o $@ $(INCFLAGS)


clean:
	rm -f $(OBJS) $(LIB)
