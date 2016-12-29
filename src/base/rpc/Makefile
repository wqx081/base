
CXXFLAGS += -I$(SRC_DIR)
CXXFLAGS += -std=c++11 -Wall -Werror -Wno-sign-compare -Wno-deprecated -g -c -o

CXX=g++

CPP_SOURCES :=  \
	rpc_header.pb.cc \
	rpc_introspection.pb.cc \
    acceptor_pool.cc    \
	blocking_ops.cc \
	connection.cc   \
	constants.cc    \
	inbound_call.cc \
	messenger.cc    \
	negotiation.cc  \
	outbound_call.cc    \
	proxy.cc    \
	reactor.cc  \
	remote_method.cc    \
	request_tracker.cc  \
	result_tracker.cc   \
	rpc.cc  \
	rpc_context.cc  \
	rpc_controller.cc   \
	rpcz_store.cc   \
	sasl_client.cc  \
	sasl_common.cc  \
	sasl_helper.cc  \
	sasl_server.cc  \
	serialization.cc    \
	service_if.cc   \
	service_pool.cc \
	service_queue.cc    \
	transfer.cc

CPP_OBJECTS := $(CPP_SOURCES:.cc=.o)

STATIC_LIB=librpc.a

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
.PRECIOUS: $(PROTO_GEN)

.cc.o:
	@echo "  [CXX]  $@"
	@$(CXX) $(CXXFLAGS) $@ $<

%.pb.cc: %.proto
	@echo "  [GEN]  $@"
	@protoc  --cpp_out $(SRC_DIR) --proto_path $(SRC_DIR) --proto_path /usr/local/include $(CURDIR)/$<

clean:
	@rm -fr $(CPP_OBJECTS)
	@rm -fr $(LIBS)
	@rm -fr *.pb.h *.pb.cc *.o
