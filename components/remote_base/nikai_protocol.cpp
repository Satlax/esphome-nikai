#include "nikai_protocol.h"
#include "esphome/core/log.h"

namespace esphome {
namespace remote_base {

static const char *TAG = "remote.nikai";

static const uint32_t NIKAI_FREQ        = 38000;
static const uint32_t NIKAI_START_MARK  = 4000;
static const uint32_t NIKAI_START_SPACE = 4000;
static const uint32_t NIKAI_BIT_MARK    = 520;
static const uint32_t NIKAI_ONE_SPACE   = 2000;
static const uint32_t NIKAI_ZERO_SPACE  = 1000;
static const uint8_t  NIKAI_BITS        = 24;

static uint32_t reverse_bits(uint32_t value, uint8_t nbits) {
  uint32_t result = 0;
  for (uint8_t i = 0; i < nbits; i++) {
    result = (result << 1) | (value & 1);
    value >>= 1;
  }
  return result;
}

void NikaiProtocol::encode(RemoteTransmitData *dst, const NikaiData &data) {
  dst->set_carrier_frequency(NIKAI_FREQ);
  dst->item(NIKAI_START_MARK, NIKAI_START_SPACE);
  
  // data.data is stored MSB-first (matches Tasmota "Data" representation)
  // wire format is LSB-first, so reverse first
  uint32_t wire_value = reverse_bits(data.data, NIKAI_BITS);
  
  for (int i = 0; i < NIKAI_BITS; i++) {
    if ((wire_value >> i) & 1)
      dst->item(NIKAI_BIT_MARK, NIKAI_ONE_SPACE);
    else
      dst->item(NIKAI_BIT_MARK, NIKAI_ZERO_SPACE);
  }
  dst->mark(NIKAI_BIT_MARK);
}

optional<NikaiData> NikaiProtocol::decode(RemoteReceiveData data) {
  NikaiData out{};
  if (!data.expect_item(NIKAI_START_MARK, NIKAI_START_SPACE))
    return {};
  
  uint32_t wire_value = 0;
  for (int i = 0; i < NIKAI_BITS; i++) {
    if (!data.expect_mark(NIKAI_BIT_MARK))
      return {};
    
    if (data.expect_space(NIKAI_ONE_SPACE)) {
      wire_value |= (1u << i);
    } else if (data.expect_space(NIKAI_ZERO_SPACE)) {
      // bit is 0, do nothing
    } else {
      return {};
    }
  }
  
  out.data = reverse_bits(wire_value, NIKAI_BITS);
  return out;
}

void NikaiProtocol::dump(const NikaiData &data) {
  ESP_LOGI(TAG, "Received Nikai: data=0x%05" PRIX32, data.data);
}

}  // namespace remote_base
}  // namespace esphome
