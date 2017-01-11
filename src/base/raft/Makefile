
CXXFLAGS += -I$(SRC_DIR)
CXXFLAGS += -std=c++11 -Wall -Werror -Wno-sign-compare -Wno-deprecated -g -c -o

CXX=g++

CPP_SOURCES :=  \
	proto/maintenance_manager.pb.cc \
	proto/opid.pb.cc \
	proto/metadata.pb.cc \
	proto/consensus.pb.cc \
	proto/consensus.service.pb.cc \
	proto/consensus.proxy.pb.cc \
	\
	time_manager.cc \
	resettable_heartbeater.cc \
	\
	proto/log.pb.cc \
	opid_util.cc \
	log_index.cc \
	log_metrics.cc \
	log_util.cc \
	log_reader.cc \

CPP_OBJECTS := $(CPP_SOURCES:.cc=.o)

STATIC_LIB=libconsensus.a

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
