
CXXFLAGS += -I$(SRC_DIR)
CXXFLAGS += -std=c++11 -Wall -Werror -Wno-sign-compare -Wno-deprecated -g -c -o

CXX=g++

CFLAGS +=  -std=c99 -O2 -W -Wall -pedantic -pthread -pipe -c -o
CC=gcc

CPP_SOURCES := \
	webserver_options.cc \
	webserver.cc \

CPP_OBJECTS := $(CPP_SOURCES:.cc=.o)

C_SOURCES := \
	squeasel.c

C_OBJECTS := $(C_SOURCES:.c=.o)

STATIC_LIB=libhttp.a

LIBS += $(STATIC_LIB)

OBJECTS += $(CPP_OBJECTS)
OBJECTS += $(C_OBJECTS)

all: $(OBJECTS) $(LIBS)

$(STATIC_LIB): $(OBJECTS)
	@echo "  [LINK] $@"
	@ar rs $@ $(OBJECTS)

.cc.o:
	@echo "  [CXX]  $@"
	@$(CXX) $(CXXFLAGS) $@ $<

.c.o:
	@echo "  [CC]   $@"
	@$(CC) $(CFLAGS) $@ $<

clean:
	@rm -fr $(OBJECTS)
	@rm -fr $(LIBS)
	@rm -fr *.pb.h *.pb.cc
