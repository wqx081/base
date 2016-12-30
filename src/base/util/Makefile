
CXXFLAGS += -I$(SRC_DIR)
CXXFLAGS += -std=c++11 -Wall -Werror -Wno-sign-compare -Wno-deprecated -g -c -o

CXX=g++

CPP_SOURCES :=  \
	memory/arena.cc \
	memory/memory.cc \
	memory/overwrite.cc \
	url_coding.cc \
	os_util.cc \
	string_case.cc \
	trace.cc \
	trace_metrics.cc \
	pb_util.pb.cc \
	user.cc \
	coding.cc \
	crc32c.cc \
	pb_util-internal.cc \
	pb_util.cc \
	mem_tracker.cc \
	histogram.pb.cc \
	hdr_histogram.cc \
	metrics.cc \
	striped64.cc \
	atomic.cc \
	base64.cc	\
	condition_variable.cc \
	errno.cc \
	env.cc \
	env_util.cc \
	env_posix.cc \
	faststring.cc \
	path_util.cc \
	jsonreader.cc \
	jsonwriter.cc \
	locks.cc \
	malloc.cc \
	monotime.cc \
	mutex.cc \
	once.cc \
	random_util.cc \
	rw_mutex.cc \
	rwc_lock.cc \
	semaphore.cc \
	slice.cc	\
	status_callback.cc	\
	status.cc	\
	subprocess.cc \
	test_util.cc \
	test_util_prod.cc \
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
