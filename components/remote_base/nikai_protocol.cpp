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

// Статическая переменная для запоминания последней команды
static uint32_t last_decoded_data = 0;

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
  out.is_repeat = false;
  
  // Попытка 1: Декодирование полного кадра (с start mark)
  if (data.expect_item(NIKAI_START_MARK, NIKAI_START_SPACE)) {
    uint32_t wire_value = 0;
    for (int i = 0; i < NIKAI_BITS; i++) {
      if (!data.expect_mark(NIKAI_BIT_MARK))
        return {};
      
      if (data.expect_space(NIKAI_ONE_SPACE)) {
        wire_value |= (1u << i);
      } else if (data.expect_space(NIKAI_ZERO_SPACE)) {
        // bit is 0
      } else {
        return {};
      }
    }
    
    out.data = reverse_bits(wire_value, NIKAI_BITS);
    last_decoded_data = out.data;  // Запоминаем последнюю команду
    return out;
  }
  
  // Попытка 2: Распознавание repeat-кода (без start mark)
  // Reset data pointer to beginning
  data.reset();
  
  // Проверяем, что нет start mark (это не полный кадр)
  int first_mark = abs(data[0]);
  if (first_mark >= 3000) {
    // Есть длинный start mark, это не repeat
    return {};
  }
  
  // Ищем последовательность из 24+ валидных битов NIKAI
  int valid_pairs = 0;
  
  for (size_t i = 0; i + 1 < data.size(); i += 2) {
    int mark = abs(data[i]);
    int space = abs(data[i+1]);
    
    // Проверяем, похож ли этот pair на бит NIKAI
    if (mark >= 400 && mark <= 700) {
      if ((space >= 800 && space <= 1300) || (space >= 1700 && space <= 2300)) {
        valid_pairs++;
      } else {
        valid_pairs = 0;  // Сбрасываем счетчик
      }
    } else {
      valid_pairs = 0;
    }
    
    // Если нашли достаточно валидных битов, это repeat!
    if (valid_pairs >= NIKAI_BITS) {
      if (last_decoded_data != 0) {
        out.data = last_decoded_data;
        out.is_repeat = true;
        ESP_LOGD(TAG, "Detected NIKAI repeat code: data=0x%06" PRIX32, last_decoded_data);
        return out;
      }
    }
  }
  
  return {};
}

void NikaiProtocol::dump(const NikaiData &data) {
  if (data.is_repeat) {
    ESP_LOGI(TAG, "Received Nikai REPEAT: data=0x%06" PRIX32, data.data);
  } else {
    ESP_LOGI(TAG, "Received Nikai: data=0x%06" PRIX32, data.data);
  }
}

}  // namespace remote_base
}  // namespace esphome
