
CXXFLAGS += -I$(SRC_DIR)
CXXFLAGS += -std=c++11 -Wall -Werror -Wno-sign-compare -Wno-deprecated -g -c -o

CXX=g++

CPP_SOURCES :=  \
	fs.pb.cc \
	block_id.cc \
	block_manager.cc \
	block_manager_metrics.cc \
	block_manager_util.cc \
	data_dirs.cc \
	file_block_manager.cc \
	\
	cache.cc \
	file_cache.cc \
	cache_metrics.cc \
	nvm_cache.cc \
	\
	fs_manager.cc \
	log_block_manager.cc \

CPP_OBJECTS := $(CPP_SOURCES:.cc=.o)

STATIC_LIB=libfs.a

LIBS += $(STATIC_LIB)

# Protobuf
PROTO_SRC := $(wildcard *.proto)
PROTO_OBJ := $(PROTO_SRC:.proto=.pb.o)
PROTO_GEN := $(PROTO_SRC:.proto=.pb.cc) $(PROTO_SRC:.proto=.pb.h)


all: $(CPP_OBJECTS) $(LIBS)

$(STATIC_LIB): $(CPP_OBJECTS)
	@echo "  [LINK] $@"
	@ar crs $@ $(CPP_OBJECTS)

# 防止自动删除 *.pb.cc
.PRECIOUS: $(CPP_SOURCES)

.cc.o:
	@echo "  [CXX]  $@"
	@$(CXX) $(CXXFLAGS) $@ $<

%.pb.cc: %.proto
	@echo "  [GEN]  $@"
	@protoc  --cpp_out $(SRC_DIR) --proto_path $(SRC_DIR) --proto_path /usr/local/include $(CURDIR)/$<

%.service.pb.cc: %.proto
	protoc  --plugin=$(SRC_PREFIX)/rpc/protoc-gen-krpc --krpc_out $(SRC_DIR)  --proto_path $(SRC_DIR) --proto_path /usr/local/include $(CURDIR)/$<

%.proxy.pb.cc: %.proto
	protoc  --plugin=$(SRC_PREFIX)/rpc/protoc-gen-krpc --krpc_out $(SRC_DIR)  --proto_path $(SRC_DIR) --proto_path /usr/local/include $(CURDIR)/$<

clean:
	@rm -fr $(CPP_OBJECTS)
	@rm -fr $(LIBS)
	@rm -fr *.pb.h *.pb.cc *.o
	@rm -fr ./proto/*.o ./proto/*.pb.h ./proto/*.pb.cc
