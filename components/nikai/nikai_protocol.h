#pragma once
#include "esphome/components/remote_base/remote_base.h"

namespace esphome {
namespace remote_base {

struct NikaiData {
  uint32_t data;
  bool operator==(const NikaiData &rhs) const { return data == rhs.data; }
};

class NikaiProtocol : public RemoteProtocol<NikaiData> {
 public:
  void encode(RemoteTransmitData *dst, const NikaiData &data) override;
  optional<NikaiData> decode(RemoteReceiveData data) override;
  void dump(const NikaiData &data) override;
};

DECLARE_REMOTE_PROTOCOL(Nikai)

template<typename... Ts>
class NikaiAction : public RemoteTransmitterActionBase<Ts...> {
  TEMPLATABLE_VALUE(uint32_t, data)
 public:
  void set_data(uint32_t data) { data_ = data; }
  void encode(RemoteTransmitData *dst, Ts... x) override {
    NikaiData d{};
    d.data = this->data_.value(x...);
    NikaiProtocol().encode(dst, d);
  }
};

}  // namespace remote_base
}  // namespace esphome
