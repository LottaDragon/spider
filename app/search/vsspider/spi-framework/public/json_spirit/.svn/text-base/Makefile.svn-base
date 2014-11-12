#
# Makefile.json_spirit
#
# $Id: Makefile,v 1.3 2009/06/11 03:29:08 zhuhk Exp $
#

CXXFLAGS	= -O2
CFLAGS		= $(CXXFLAGS)

target = libjson_spirit.a

all: $(target) output

libjson_spirit.a: json_spirit_reader.o json_spirit_value.o json_spirit_writer.o
	ar cr $@ $^
	ranlib $@

output:
	cp $(target) *.h ../

clean:
	rm -rf *.o *~ $(target) output
	rm -f ../json_spirit*.h

tags: $(wildcard *.cpp *.c *.h)
	ctags -R *.cpp *.h

# EOF
