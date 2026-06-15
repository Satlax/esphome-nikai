#pragma once

#include "esphome/components/remote_base/remote_base.h"

namespace esphome {
namespace remote_base {

struct NikaiData {
  uint32_t data;
};

class NikaiProtocol : public RemoteProtocol<NikaiData> {
 public:
  void encode(RemoteTransmitData *dst, const NikaiData &data) override;
  optional<NikaiData> decode(RemoteReceiveData data) override;
  void dump(const NikaiData &data) override;
};

DECLARE_REMOTE_PROTOCOL(Nikai)

} 
}
