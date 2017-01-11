
include build_config.mk
export

SRC_PREFIX=${CURDIR}/base

all:
	make -C ${SRC_PREFIX}/core
	make -C ${SRC_PREFIX}/util
	make -C ${SRC_PREFIX}/net
	make -C ${SRC_PREFIX}/http
	make -C ${SRC_PREFIX}/rpc
	make -C ${SRC_PREFIX}/fs
	make -C ${SRC_PREFIX}/common
	make -C ${SRC_PREFIX}/raft

#	make -C ${SRC_PREFIX}/app/zk
#	make -C ${SRC_PREFIX}/common
#	make -C ${SRC_PREFIX}/server
#	make -C ${SRC_PREFIX}/master
#	make -C ${SRC_PREFIX}/worker_server
#	make -C ${SRC_PREFIX}/tests/master
	make -C ${SRC_PREFIX}/tests/rpc
#	make -C ${SRC_PREFIX}/tests/util

clean:
	make -C ${SRC_PREFIX}/core clean
	make -C ${SRC_PREFIX}/util clean
	make -C ${SRC_PREFIX}/net clean
	make -C ${SRC_PREFIX}/http clean
	make -C ${SRC_PREFIX}/rpc clean
	make -C ${SRC_PREFIX}/fs clean
	make -C ${SRC_PREFIX}/common clean
	make -C ${SRC_PREFIX}/raft clean
#	make -C ${SRC_PREFIX}/common clean
#	make -C ${SRC_PREFIX}/server clean
#	make -C ${SRC_PREFIX}/master clean
#	make -C ${SRC_PREFIX}/worker_server clean
	make -C ${SRC_PREFIX}/tests/rpc clean
#	make -C ${SRC_PREFIX}/tests/util clean

