#
# Makefile.json_test
#
# $Id: Makefile,v 1.1 2009/02/25 05:51:48 pengdw Exp $
#

include ../../../Makefile.env

INCLUDE		= -I$(JSON_SPIRITINC) \
		  $(NULL)

LIB		= $(JSON_SPIRIT) \
		  $(NULL)

CXXFLAGS	= -W -Wall -g -pipe \
		  -DVERSION="\"1.0.0.0\"" \
		  $(INCLUDE) \
		  $(NULL)

CFLAGS		= $(CXXFLAGS)
LDFLAGS		= $(LIB)

target = json_test

all: $(target) output

json_test : json_spirit_reader_test.o  json_spirit_utils_test.o  json_spirit_value_test.o  json_spirit_writer_test.o  json_test.o
	$(CXX) -o $@ $^ $(LDFLAGS)

output:
	rm -rf output
	mkdir output
	cp $(target) output

clean:
	rm -rf *.o *~ $(target) output

tags : $(wildcard *.cpp *.c *.h)
	ctags -R *.cpp *.h

# EOF
