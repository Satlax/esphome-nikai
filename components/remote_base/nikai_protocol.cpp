#include "nikai_protocol.h"
#include "esphome/core/log.h"


namespace esphome {
namespace remote_base {


static const char *TAG = "remote.nikai";


static const uint32_t NIKAI_FREQ = 38000;


static const uint32_t NIKAI_HEADER_MARK  = 4000;
static const uint32_t NIKAI_HEADER_SPACE = 4000;


static const uint32_t NIKAI_BIT_MARK = 500;
static const uint32_t NIKAI_ONE_SPACE = 2000;
static const uint32_t NIKAI_ZERO_SPACE = 1000;


static const uint8_t NIKAI_BITS = 24;



void NikaiProtocol::encode(
    RemoteTransmitData *dst,
    const NikaiData &data
) {

  dst->set_carrier_frequency(NIKAI_FREQ);


  dst->item(
      NIKAI_HEADER_MARK,
      NIKAI_HEADER_SPACE
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

}



optional<NikaiData> NikaiProtocol::decode(
    RemoteReceiveData data
) {

  NikaiData out{};


  if (!data.expect_item(
        NIKAI_HEADER_MARK,
        NIKAI_HEADER_SPACE
      )) {

    return {};

  }


  uint32_t value = 0;



  for (int i = 0; i < NIKAI_BITS; i++) {


    if (data.expect_item(
          NIKAI_BIT_MARK,
          NIKAI_ONE_SPACE
        )) {


      value |= (1UL << (23 - i));


    } else if (
        data.expect_item(
          NIKAI_BIT_MARK,
          NIKAI_ZERO_SPACE
        )
    ) {


    } else {


      return {};

    }

  }



  out.data = value;


  return out;

}



void NikaiProtocol::dump(
    const NikaiData &data
) {

  ESP_LOGI(
      TAG,
      "Received Nikai: data=0x%06" PRIX32,
      data.data
  );

}



}
}
