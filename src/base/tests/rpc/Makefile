
CXXFLAGS += -I$(SRC_DIR)
CXXFLAGS += -std=c++11 -Wall -Werror -Wno-sign-compare -Wno-deprecated -g -c -o

ANT_LIBS := $(SRC_PREFIX)/rpc/librpc.a $(SRC_PREFIX)/net/libnet.a $(SRC_PREFIX)/util/libutil.a $(SRC_PREFIX)/core/libcore.a

TEST_LIBS := -L/usr/local/lib -lgtest -lgtest_main -lpthread
COMMON_LIBS := -lglog -lgflags -levent  -lpthread -lssl -lcrypto -lz -lev -lsasl2 -lpcre \
	-lprotobuf -lprotoc

CXX=g++

CPP_SOURCES := \
	rtest_diff_package.pb.cc \
	rtest.pb.cc \
	rtest.service.pb.cc \
	rtest.proxy.pb.cc \
	echo.pb.cc \
	echo.service.pb.cc \
	echo.proxy.pb.cc \
	rpct.pb.cc \
	rpct.service.pb.cc \
	rpct.proxy.pb.cc \

CPP_OBJECTS := $(CPP_SOURCES:.cc=.o)

tests := rpc_unittest \
	reactor_unittest \
	rpc_stub_unittest \
	service_queue_unittest \
	mt_rpc_unittest \
	rpct_unittest \

all: $(CPP_OBJECTS) $(tests)

.PRECIOUS: $(CPP_SOURCES)

.cc.o:
	@$(CXX) $(CXXFLAGS) $@ $<

%.pb.cc: %.proto
	protoc  --cpp_out $(SRC_DIR) --proto_path $(SRC_DIR) --proto_path /usr/local/include $(CURDIR)/$<
%.service.pb.cc: %.proto
	protoc  --plugin=$(SRC_PREFIX)/rpc/protoc-gen-krpc --krpc_out $(SRC_DIR)  --proto_path $(SRC_DIR) --proto_path /usr/local/include $(CURDIR)/$<
%.proxy.pb.cc: %.proto
	protoc  --plugin=$(SRC_PREFIX)/rpc/protoc-gen-krpc --krpc_out $(SRC_DIR)  --proto_path $(SRC_DIR) --proto_path /usr/local/include $(CURDIR)/$<


rpc_unittest: rpc_unittest.o
	@echo "  [LINK]  $@"
	@$(CXX) -o $@ $< $(CPP_OBJECTS) $(ANT_LIBS) $(COMMON_LIBS) $(TEST_LIBS)

reactor_unittest: reactor_unittest.o
	@echo "  [LINK]  $@"
	@$(CXX) -o $@ $< $(CPP_OBJECTS) $(ANT_LIBS) $(COMMON_LIBS) $(TEST_LIBS)

rpc_stub_unittest: rpc_stub_unittest.o
	@echo "  [LINK]  $@"
	@$(CXX) -o $@ $< $(CPP_OBJECTS) $(ANT_LIBS) $(COMMON_LIBS) $(TEST_LIBS)

service_queue_unittest: service_queue_unittest.o
	@echo "  [LINK]  $@"
	@$(CXX) -o $@ $< $(CPP_OBJECTS) $(ANT_LIBS) $(COMMON_LIBS) $(TEST_LIBS)

mt_rpc_unittest: mt_rpc_unittest.o
	@echo "  [LINK]  $@"
	@$(CXX) -o $@ $< $(CPP_OBJECTS) $(ANT_LIBS) $(COMMON_LIBS) $(TEST_LIBS)


rpct_unittest: rpct_unittest.o
	@echo "  [LINK]  $@"
	@$(CXX) -o $@ $< $(CPP_OBJECTS) $(ANT_LIBS) $(COMMON_LIBS) -L/usr/local/lib -lgtest -lpthread

clean:
	rm -fr *.o *.pb.h *.pb.cc
	rm -fr $(tests)
