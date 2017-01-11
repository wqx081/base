#ifndef BASE_COMMON_WIRE_PROTOCOL_H_
#define BASE_COMMON_WIRE_PROTOCOL_H_
#include <vector>
#include <boost/optional.hpp>

#include "base/common/wire_protocol.pb.h"
#include "base/util/status.h"

using boost::optional;

namespace base {

class Sockaddr;
class HostPortPB;
class HostPort;

void StatusToPB(const Status& status, AppStatusPB* pb);
Status StatusFromPB(const AppStatusPB& pb);


Status HostPortToPB(const HostPort& host_port, HostPortPB* host_port_pb);

Status HostPortFromPB(const HostPortPB& host_port_pb, HostPort* host_port);

Status AddHostPortPBs(const std::vector<Sockaddr>& addrs,
                      google::protobuf::RepeatedPtrField<HostPortPB>* pbs);

} // namespace base
#endif // BASE_COMMON_WIRE_PROTOCOL_H_
