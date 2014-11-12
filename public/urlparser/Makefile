WORKROOT = ../../
ifeq ($(MAC),64)
	LIBPATH=$(WORKROOT)lib2-64
else
	LIBPATH=$(WORKROOT)lib2
endif

INCLUDE = -I./ -I$(LIBPATH)/ullib/include/

LIB     = -L./ \
	-L$(LIBPATH)/ullib/lib/ \
	-lullib -lpthread

COMMON_DEFINES = -DLINUX -D_REENTERANT

ifeq "$(MAKECMDGOALS)" "release"
	DEFINES=$(COMMON_DEFINES) -DNDEBUG
	CPPFLAGS=
	CFLAGS= -O2 -fPIC -W -Wall $(DEFINES) $(INCLUDE)  
	CXXFLAGS= -O2 -fPIC -W -Wall $(DEFINES) $(INCLUDE) 
else
	ifeq "$(MAKECMDGOALS)" "withpg"
		DEFINES=$(COMMON_DEFINES)
		CPPFLAGS=
		CFLAGS= -g -pg -fPIC -W -Wall $(DEFINES) $(INCLUDE)  
		CXXFLAGS= -g -pg -fPIC -W -Wall $(DEFINES) $(INCLUDE) 
	else
		DEFINES=$(COMMON_DEFINES)
		CPPFLAGS= 
		CFLAGS= -g -Wall -W -fPIC $(DEFINES) $(INCLUDE)  
		CXXFLAGS= -g -Wall -W -fPIC $(DEFINES) $(INCLUDE) 
	endif
endif

CC  = g++
CPP = g++
CXX = g++ 
AR  = ar
#=========================================================================

LIBS            = libnewurl.a
RELEASE_LIBS    =
EXECUTABLE      = 
TEST_EXEC       = 
OUTPUT_PATH     = ./output
URL_OBJS        = uln_url.o
EXPORT_HEADFILE = uln_url.h
REFER_FILES     = *.h *.cpp

all	: clean output
clean   :
	/bin/rm -f *.o
	/bin/rm -f $(LIBS)
	/bin/rm -f $(EXECUTABLE) $(TEST_EXEC)
	/bin/rm -rf $(OUTPUT_PATH)
	
rebuild : clean all

deps :
	g++ -MM -MG *.cpp >depends

cleanall : clean 
	rm -f *~

output : $(LIBS) $(EXECUTABLE) $(TEST_EXEC) 
	@if [ -d $(OUTPUT_PATH) ]; then rm -rf $(OUTPUT_PATH); fi
	mkdir $(OUTPUT_PATH)
	mkdir $(OUTPUT_PATH)/include
	mkdir $(OUTPUT_PATH)/lib
	cp -f $(EXPORT_HEADFILE) $(OUTPUT_PATH)/include
	cp -f $(LIBS) $(OUTPUT_PATH)/lib

libnewurl.a : $(URL_OBJS)
	$(AR) rcv $@ $^

test : test.o mc_cache.o 
	$(CC) $(CPPFLAGS) -o $@ $^ $(LIB)

-include depends



