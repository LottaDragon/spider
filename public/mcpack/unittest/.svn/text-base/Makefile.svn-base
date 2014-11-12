WORKROOT=../../../
CXX	= $(WORKROOT)/public/test/cxxtest/

MARCS=
ifeq ($(MAC),64)
	LIBPATH=$(WORKROOT)lib2-64
else
	LIBPATH=$(WORKROOT)lib2
endif

PUBLIC=$(WORKROOT)/public

INCLUDE= -I../output/include -I$(CXX)
#XFLAG = -Wall -W -pipe -Wno-unused-parameter -g -DBSL_DEBUG_MOD
#XXFLAG = -Wall -W -pipe -Wno-unused-parameter -g -fprofile-arcs -ftest-coverage
XFLAG = -Wall -W -pipe -Wno-unused-parameter -g -D_XOPEN_SOURE=500 -D_GNU_SOURCE
LIB=  -L../output/lib -lmcpack

CFLAG	=	$(XFLAG) -D_REENTRANT -D_FILTER -DNDEBUG $(INCLUDE) 
CPPFLAG	=	$(XFLAG) -D_REENTRANT -D_FILTER $(INCLUDE)

CXXTEST	= 	$(CXX)/cxxtestgen.pl
OUTPUT	= 	--error-printer --have-std --have-eh 

CC		= gcc
CPP		= g++
PERL	= perl
PY		= python

ALLTESTS=$(wildcard *.h)
CPPFILE=$(ALLTESTS:.h=.cpp)
EXE=$(basename $(ALLTESTS))

#$(CPPFILE) : $(ALLTESTS)
.h.cpp :
	$(CXXTEST) $(OUTPUT) -o $*.cpp $*.h 

	
all	   : $(EXE)
	
$(EXE) : $(CPPFILE) 
	$(CPP) $(CPPFLAG) -o $@   $@.cpp $(LIB) 

#=========================================================================
.PHONY	: clean  all test

clean	:
		rm -f  $(CPPFILE) $(EXE) $(XDATA)
test	: $(EXE) 
		@failed=0;
		@for exe in $(EXE) ; do echo; echo -n doing $$exe ">>>>>>>>";echo;echo;\
			./$$exe ; \
			echo done ; \
			echo ;\
		done;

