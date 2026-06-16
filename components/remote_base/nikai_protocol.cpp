#include "nikai_protocol.h"
#include "esphome/core/log.h"

namespace esphome {
namespace remote_base {

static const char *TAG = "remote.nikai";

static const uint32_t NIKAI_FREQ        = 38000;

static const uint32_t NIKAI_START_MARK  = 4000;
static const uint32_t NIKAI_START_SPACE = 4000;

static const uint32_t NIKAI_BIT_MARK    = 450;
static const uint32_t NIKAI_ONE_SPACE   = 2050;
static const uint32_t NIKAI_ZERO_SPACE  = 1050;

static const uint8_t NIKAI_BITS = 24;


void NikaiProtocol::encode(RemoteTransmitData *dst, const NikaiData &data) {
  dst->set_carrier_frequency(NIKAI_FREQ);

  dst->item(
      NIKAI_START_MARK,
      NIKAI_START_SPACE
  );

  for (int i = NIKAI_BITS - 1; i >= 0; i--) {

    if ((data.data >> i) & 1) {

      dst->item(
          NIKAI_BIT_MARK,
          NIKAI_ONE_SPACE
      );

    } else {

      dst->item(
          NIKAI_BIT_MARK,
          NIKAI_ZERO_SPACE
      );

    }
  }

  dst->mark(NIKAI_BIT_MARK);
}


optional<NikaiData> NikaiProtocol::decode(RemoteReceiveData data) {

  NikaiData out{};

  if (!data.expect_item(
          NIKAI_START_MARK,
          NIKAI_START_SPACE
      )) {
    return {};
  }


  uint32_t value = 0;


  for (int i = 0; i < NIKAI_BITS; i++) {

    if (!data.expect_mark(NIKAI_BIT_MARK)) {
      return {};
    }


    if (data.expect_space(NIKAI_ONE_SPACE)) {

      value = (value << 1) | 1;


    } else if (data.expect_space(NIKAI_ZERO_SPACE)) {

      value <<= 1;


    } else {

      return {};

    }
  }


  out.data = value;

  return out;
}


void NikaiProtocol::dump(const NikaiData &data) {

  ESP_LOGI(
      TAG,
      "Received Nikai: data=0x%06X",
      data.data
  );

}


}  // namespace remote_base
}  // namespace esphome
