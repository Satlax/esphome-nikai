#include "nikai_protocol.h"

#include "esphome/core/log.h"


namespace esphome {
namespace remote_base {


static const char *TAG = "NIKAI";


static const uint32_t NIKAI_FREQ = 38000;


static const uint32_t HEADER_MARK  = 4000;
static const uint32_t HEADER_SPACE = 4000;


static const uint32_t BIT_MARK = 500;


static const uint32_t ONE_SPACE  = 2000;
static const uint32_t ZERO_SPACE = 1000;


static const uint8_t BITS = 24;



void NikaiProtocol::encode(
    RemoteTransmitData *dst,
    const NikaiData &data
) {


  dst->set_carrier_frequency(
      NIKAI_FREQ
  );


  dst->item(
      HEADER_MARK,
      HEADER_SPACE
  );


  for(int i = BITS - 1; i >= 0; i--) {


    if((data.data >> i) & 1) {


      dst->item(
          BIT_MARK,
          ONE_SPACE
      );


    } else {


      dst->item(
          BIT_MARK,
          ZERO_SPACE
      );

    }

  }


  dst->mark(
      BIT_MARK
  );

}



optional<NikaiData> NikaiProtocol::decode(
    RemoteReceiveData data
) {


  NikaiData out{};


  if(
      !data.expect_item(
          HEADER_MARK,
          HEADER_SPACE
      )
  )
    return {};



  uint32_t value = 0;



  for(
      int i = 0;
      i < BITS;
      i++
  ) {


    if(
        !data.expect_mark(
            BIT_MARK
        )
    )
      return {};



    if(
        data.expect_space(
            ONE_SPACE
        )
    ) {


      value =
          (value << 1) | 1;



    } else if(
        data.expect_space(
            ZERO_SPACE
        )
    ) {


      value =
          (value << 1);



    } else {


      return {};

    }


  }



  // меняем порядок байтов под Tasmota

  out.data =
      ((value & 0xFF) << 16)
      |
      (value & 0xFF00)
      |
      ((value >> 16) & 0xFF);



  return out;

}



void NikaiProtocol::dump(
    const NikaiData &data
) {


  ESP_LOGD(
      TAG,
      "decoded 0x%05X",
      data.data
  );


}


} // namespace remote_base
} // namespace esphome
