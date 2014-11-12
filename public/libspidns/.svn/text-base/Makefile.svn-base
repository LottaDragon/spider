WORKROOT=./
VERSION="\"spidns 1.0.1.0
VERSION64_SIGN=64bit\""
VERSION32_SIGN=\""

ifeq ($(MAC),64)
        VERSION+=$(VERSION64_SIGN)
        ULIB=../../lib2-64
        INSTALL_PATH=$(WORKROOT)output
else
        VERSION+=$(VERSION32_SIGN)
        ULIB=../../lib2
        INSTALL_PATH=$(WORKROOT)output
endif

INC=-I./ \
        -I$(ULIB)/ullib/include/ \
	-I$(ULIB)/dict/include/ \
	-I$(ULIB)/others-ex/include/ \
	-I/usr/include/pcre/ 
        
LIB=-L$(ULIB)/dict/lib/ \
        -L$(ULIB)/ullib/lib/ \
	-L$(ULIB)/dict/lib/ \
	-luldict \
	-lpcre \
        -lcrypto \
        -lpthread \
	-lz	\
        -lullib

CPPFLAGS=$(INC)  -g -Wall -D__VERSION_ID__=$(VERSION)
ifeq "$(MAKECMDGOALS)" "notime"
CPPFLAGS=$(CPPFLAGS)  -g  -D__TIME__="\"no_time\"" -D__DATE__="\"no_date\""
endif

CC      = g++ 

EXE = test_spidns test_robots test_spidns_udp
LIB_BUILD = libspidns.a

all :   clean $(EXE) $(LIB_BUILD)
	@if [ -d output ]; then rm -rf output; fi
	@if [ -d output/include ]; then rm -rf output/include; fi
	@if [ -d output/lib ]; then rm -rf output/lib; fi
	@mkdir output
	@mkdir output/include
	@mkdir output/lib
	cp *.h output/include
	cp *.a output/lib
	cp $(EXE) output
	rm *.o

notime : all

libspidns.a : spidns.o spidns_connection.o spidns_data.o spidns_robots.o
	ar rcv libspidns.a $^


spidns_robots.o:spidns_robots.cpp
	$(CC) $(CPPFLAGS) $(INC) -c $^ -o $@

spidns_connection.o:spidns_connection.cpp
	$(CC) $(CPPFLAGS) $(INC) -c $^ -o $@

spidns_data.o:spidns_data.cpp
	$(CC) $(CPPFLAGS) $(INC) -c $^ -o $@

spidns.o:spidns.cpp
	$(CC) $(CPPFLAGS) $(INC) -c $^ -o $@

test_spidns.o:spidns.cpp
	$(CC) $(CPPFLAGS) $(INC) -DTEST_SPIDNS -c $^ -o $@

test_spidns:test_spidns.o spidns_robots.o spidns_connection.o spidns_data.o
	$(CC) $(CPPFLAGS) -o $@ $^ $(LIB)

test_spidns_udp.o:spidns.cpp
	$(CC) $(CPPFLAGS) $(INC) -DTEST_SPIDNS_UDP -c $^ -o $@

test_spidns_udp: test_spidns_udp.o spidns_robots.o spidns_connection.o spidns_data.o
	$(CC) $(CPPFLAGS) $(INC) $^ $(LIB) -o $@

test_robots.o:spidns.cpp
	$(CC) $(CPPFLAGS) $(INC) -D__TEST_ROBOTS -c $^ -o $@

test_robots:test_robots.o spidns_robots.o spidns_connection.o spidns_data.o
	$(CC) $(CPPFLAGS) -o $@ $^ $(LIB)

clean :
	rm -f $(LIB_BUILD)
	rm -f *.o $(EXE)
	rm -rf output
