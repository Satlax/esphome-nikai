#include "nikai.h"
#include "esphome/core/log.h"

namespace esphome {
namespace remote_base {

static const char *TAG = "remote.nikai";

static const uint32_t NIKAI_FREQ = 38000;
static const uint32_t START = 4000;
static const uint32_t MARK = 500;
static const uint32_t ONE = 2000;
static const uint32_t ZERO = 1000;


void NikaiProtocol::encode(RemoteTransmitData *dst, const NikaiData &data) {

  dst->set_carrier_frequency(NIKAI_FREQ);

  dst->item(START, START);

  for (int i = 23; i >= 0; i--) {

    if ((data.data >> i) & 1)
      dst->item(MARK, ONE);
    else
      dst->item(MARK, ZERO);

  }

  dst->mark(MARK);
}


optional<NikaiData> NikaiProtocol::decode(RemoteReceiveData data) {

  NikaiData out{};

  if (!data.expect_item(START, START))
    return {};


  uint32_t value = 0;


  for (int i = 0; i < 24; i++) {

    if (!data.expect_mark(MARK))
      return {};


    if (data.expect_space(ONE)) {

      value = (value << 1) | 1;

    } else if (data.expect_space(ZERO)) {

      value <<= 1;

    } else {

      return {};

    }
  }


  out.data = value;

  return out;
}


void NikaiProtocol::dump(const NikaiData &data) {

  ESP_LOGD(TAG, "decoded 0x%06X", data.data);

}


}
}
