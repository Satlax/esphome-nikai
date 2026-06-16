#include "nikai_protocol.h"
#include "esphome/core/log.h"

namespace esphome {
namespace remote_base {

static const char *TAG = "remote.nikai";

static const uint32_t NIKAI_FREQ        = 38000;
static const uint32_t NIKAI_START_MARK  = 4000;
static const uint32_t NIKAI_START_SPACE = 500;
static const uint32_t NIKAI_ONE_MARK    = 2000;
static const uint32_t NIKAI_ZERO_MARK   = 1000;
static const uint32_t NIKAI_BIT_SPACE   = 500;
static const uint8_t  NIKAI_BITS        = 24;

void NikaiProtocol::encode(RemoteTransmitData *dst, const NikaiData &data) {
  dst->set_carrier_frequency(NIKAI_FREQ);
  dst->item(NIKAI_START_MARK, NIKAI_START_SPACE);
  for (int i = 0; i < NIKAI_BITS; i++) {
    if ((data.data >> i) & 1)
      dst->item(NIKAI_ONE_MARK, NIKAI_BIT_SPACE);
    else
      dst->item(NIKAI_ZERO_MARK, NIKAI_BIT_SPACE);
  }
}

optional<NikaiData> NikaiProtocol::decode(RemoteReceiveData data) {
  NikaiData out{};
  if (!data.expect_item(NIKAI_START_MARK, NIKAI_START_SPACE))
    return {};
  uint32_t value = 0;
  for (int i = 0; i < NIKAI_BITS; i++) {
    if (data.expect_item(NIKAI_ONE_MARK, NIKAI_BIT_SPACE)) {
      value |= (1u << i);
    } else if (data.expect_item(NIKAI_ZERO_MARK, NIKAI_BIT_SPACE)) {
      // bit stays 0
    } else {
      return {};
    }
  }
  out.data = value;
  return out;
}

void NikaiProtocol::dump(const NikaiData &data) {
  ESP_LOGI(TAG, "Received Nikai: data=0x%05" PRIX32, data.data);
}

}  // namespace remote_base
}  // namespace esphome
