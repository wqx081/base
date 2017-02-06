
CXXFLAGS += -I$(SRC_DIR)
CXXFLAGS += -std=c++11 -Wall -Werror -Wno-sign-compare -Wno-deprecated -g -c -o

ANT_LIBS := $(SRC_PREFIX)/net/libnet.a $(SRC_PREFIX)/util/libutil.a $(SRC_PREFIX)/core/libcore.a

TEST_LIBS := -L/usr/local/lib -lgtest -lgtest_main -lpthread
COMMON_LIBS := -lglog -lgflags -levent  -lpthread -lssl -lcrypto -lz -lev -lsasl2 -lpcre \
	-lprotobuf -lprotoc

CXX=g++

CPP_SOURCES := \


CPP_OBJECTS := $(CPP_SOURCES:.cc=.o)

tests := threadpool_unittest \

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


threadpool_unittest: threadpool_unittest.o
	@echo "  [LINK]  $@"
	@$(CXX) -o $@ $< $(CPP_OBJECTS) $(ANT_LIBS) $(COMMON_LIBS) $(TEST_LIBS)

clean:
	rm -fr *.o *.pb.h *.pb.cc
	rm -fr $(tests)
