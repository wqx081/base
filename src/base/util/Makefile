
CXXFLAGS += -I$(SRC_DIR)
CXXFLAGS += -std=c++11 -Wall -Werror -Wno-sign-compare -Wno-deprecated -g -c -o

CXX=g++

CPP_SOURCES :=  \
	atomic.cc \
	base64.cc	\
	condition_variable.cc \
	errno.cc \
	env.cc \
	env_posix.cc \
	faststring.cc \
	path_util.cc \
	locks.cc \
	malloc.cc \
	monotime.cc \
	mutex.cc \
	once.cc \
	semaphore.cc \
	slice.cc	\
	status_callback.cc	\
	status.cc	\
	thread.cc \
	threadpool.cc \
	threadlocal.cc \
	thread_restrictions.cc \

CPP_OBJECTS := $(CPP_SOURCES:.cc=.o)

STATIC_LIB=libutil.a

LIBS += $(STATIC_LIB)

all: $(CPP_OBJECTS) $(LIBS)

$(STATIC_LIB): $(CPP_OBJECTS)
	@echo "  [LINK] $@"
	@ar rs $@ $(CPP_OBJECTS)

.cc.o:
	@echo "  [CXX]  $@"
	@$(CXX) $(CXXFLAGS) $@ $<

%.pb.cc: %.proto
	@echo "  [GEN]  $@"
	@protoc  --cpp_out $(SRC_DIR) --proto_path $(SRC_DIR) --proto_path /usr/local/include $(CURDIR)/$<

clean:
	@rm -fr $(CPP_OBJECTS)
	@rm -fr $(LIBS)
	@rm -fr *.pb.h *.pb.cc
