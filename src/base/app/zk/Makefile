
CXXFLAGS += -I$(SRC_DIR)
CXXFLAGS += -std=c++11 -Wall -Werror -Wno-sign-compare -Wno-deprecated -g -c -o

ANT_LIBS := $(SRC_PREFIX)/rpc/librpc.a $(SRC_PREFIX)/net/libnet.a $(SRC_PREFIX)/util/libutil.a $(SRC_PREFIX)/core/libcore.a


COMMON_LIBS := -lglog -lgflags -levent  -lpthread -lssl -lcrypto -lz -lev -lsasl2 -lpcre \
	-L/usr/local/lib -lgtest -lgtest_main -lpthread \
	-lprotobuf -lprotoc -L/usr/local/lib/ -lleveldb -lsnappy -lpthread

CXX=g++

CPP_SOURCES := \
	ins_node.pb.cc \
	ins_node.service.pb.cc \
	ins_node.proxy.pb.cc \
	storage/binlog.cc \
	storage/meta.cc \
	storage/storage_manage.cc \
	server/flags.cc \
	server/user_manage.cc \

CPP_OBJECTS := $(CPP_SOURCES:.cc=.o)

tests := server_unittest \
	binlog_unittest \
	storage_manage_unittest \
	user_manage_unittest \

all: $(CPP_OBJECTS) $(tests)

.PRECIOUS: $(CPP_SOURCES)

.cc.o:
	@echo "  [CXX] $@"
	@$(CXX) $(CXXFLAGS) $@ $<

%.pb.cc: %.proto
	protoc  --cpp_out $(SRC_DIR) --proto_path $(SRC_DIR) --proto_path /usr/local/include $(CURDIR)/$<
%.service.pb.cc: %.proto
	protoc  --plugin=$(SRC_PREFIX)/rpc/protoc-gen-krpc --krpc_out $(SRC_DIR)  --proto_path $(SRC_DIR) --proto_path /usr/local/include $(CURDIR)/$<
%.proxy.pb.cc: %.proto
	protoc  --plugin=$(SRC_PREFIX)/rpc/protoc-gen-krpc --krpc_out $(SRC_DIR)  --proto_path $(SRC_DIR) --proto_path /usr/local/include $(CURDIR)/$<


server_unittest: server_unittest.o
	@echo "  [LINK]  $@"
	@$(CXX) -o $@ $< $(CPP_OBJECTS) $(ANT_LIBS) $(COMMON_LIBS)

binlog_unittest: binlog_unittest.o
	@echo "  [LINK]  $@"
	@$(CXX) -o $@ $< $(CPP_OBJECTS) $(ANT_LIBS) $(COMMON_LIBS)

storage_manage_unittest: storage_manage_unittest.o
	@echo "  [LINK]  $@"
	@$(CXX) -o $@ $< $(CPP_OBJECTS) $(ANT_LIBS) $(COMMON_LIBS)

user_manage_unittest: user_manage_unittest.o
	@echo "  [LINK]  $@"
	@$(CXX) -o $@ $< $(CPP_OBJECTS) $(ANT_LIBS) $(COMMON_LIBS)

clean:
	rm -fr *.o *.pb.h *.pb.cc
	rm -fr $(tests) $(CPP_OBJECTS)
