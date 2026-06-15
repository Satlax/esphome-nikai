#include "nikai_protocol.h"

#include "esphome/core/log.h"


namespace esphome {
namespace remote_base {


static const char *TAG = "remote.nikai";


static const uint32_t START = 4000;

static const uint32_t BIT_MARK = 500;

static const uint32_t ONE = 2000;

static const uint32_t ZERO = 1000;


void NikaiProtocol::encode(
    RemoteTransmitData *dst,
    const NikaiData &data
) {

  dst->set_carrier_frequency(38000);


  dst->item(
      START,
      START
  );


  for (int i = 23; i >= 0; i--) {

    if ((data.data >> i) & 1) {

      dst->item(
          BIT_MARK,
          ONE
      );

    } else {

      dst->item(
          BIT_MARK,
          ZERO
      );

    }

  }


  dst->mark(BIT_MARK);

}



optional<NikaiData> NikaiProtocol::decode(
    RemoteReceiveData data
) {


  NikaiData result{};


  if (!data.expect_item(START, START))
    return {};


  uint32_t value = 0;


  for (int i = 0; i < 24; i++) {


    if (!data.expect_mark(BIT_MARK))
      return {};


    if (data.expect_space(ONE)) {

      value = (value << 1) | 1;

    }

    else if (data.expect_space(ZERO)) {

      value = value << 1;

    }

    else {

      return {};

    }

  }


  result.data = value;


  return result;

}



void NikaiProtocol::dump(
    const NikaiData &data
) {

  ESP_LOGI(
      TAG,
      "Received Nikai: 0x%06X",
      data.data
  );

}


}
}
