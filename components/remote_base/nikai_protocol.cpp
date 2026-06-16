#include "nikai_protocol.h"
#include "esphome/core/log.h"

namespace esphome {
namespace remote_base {

static const char *TAG = "remote.nikai";

static const uint32_t NIKAI_FREQ        = 38000;
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
  
  // Start sequence
  dst->item(4000, 500);
  
  uint32_t wire_value = reverse_bits(data.data, NIKAI_BITS);
  
  for (int i = 0; i < NIKAI_BITS; i++) {
    if ((wire_value >> i) & 1)
      dst->item(520, 2000);
    else
      dst->item(520, 1000);
  }
  dst->mark(520);
}

optional<NikaiData> NikaiProtocol::decode(RemoteReceiveData data) {
  NikaiData out{};
  
  // ОТЛАДКА: Выводим первые 10 значений
  ESP_LOGD(TAG, "NIKAI decode attempt, first 10 values:");
  for (int i = 0; i < 10 && i < data.size(); i++) {
    ESP_LOGD(TAG, "  [%d] = %d", i, data[i]);
  }
  
  // Пробуем вариант 1: mark=4000, space=500
  if (data.expect_item(4000, 500)) {
    ESP_LOGD(TAG, "Variant 1 matched: mark=4000, space=500");
    
    uint32_t wire_value = 0;
    for (int i = 0; i < NIKAI_BITS; i++) {
      if (!data.expect_mark(520))
        return {};
      
      if (data.expect_space(2000)) {
        wire_value |= (1u << i);
      } else if (data.expect_space(1000)) {
        // bit is 0
      } else {
        return {};
      }
    }
    
    out.data = reverse_bits(wire_value, NIKAI_BITS);
    return out;
  }
  
  // Пробуем вариант 2: mark=4000, space=4000
  if (data.expect_item(4000, 4000)) {
    ESP_LOGD(TAG, "Variant 2 matched: mark=4000, space=4000");
    
    uint32_t wire_value = 0;
    for (int i = 0; i < NIKAI_BITS; i++) {
      if (!data.expect_mark(520))
        return {};
      
      if (data.expect_space(2000)) {
        wire_value |= (1u << i);
      } else if (data.expect_space(1000)) {
        // bit is 0
      } else {
        return {};
      }
    }
    
    out.data = reverse_bits(wire_value, NIKAI_BITS);
    return out;
  }
  
  ESP_LOGD(TAG, "No variant matched");
  return {};
}

void NikaiProtocol::dump(const NikaiData &data) {
  ESP_LOGI(TAG, "Received Nikai: data=0x%06" PRIX32, data.data);
}

}  // namespace remote_base
}  // namespace esphome
