#include "nikai_protocol.h"
#include "esphome/core/log.h"


namespace esphome {
namespace remote_base {


static const char *TAG = "remote.nikai";


static const uint32_t NIKAI_FREQ = 38000;

static const uint32_t START_MARK  = 4000;
static const uint32_t START_SPACE = 4000;

static const uint32_t BIT_MARK = 550;

static const uint32_t ONE_SPACE  = 2000;
static const uint32_t ZERO_SPACE = 1000;

static const uint8_t BITS = 24;



static uint32_t reverse_bits(uint32_t value, uint8_t bits)
{
  uint32_t result = 0;

  for(uint8_t i=0;i<bits;i++)
  {
    result <<= 1;
    result |= value & 1;
    value >>= 1;
  }

  return result;
}



void NikaiProtocol::encode(RemoteTransmitData *dst,
                           const NikaiData &data)
{

  dst->set_carrier_frequency(NIKAI_FREQ);

  dst->item(START_MARK, START_SPACE);


  uint32_t value = reverse_bits(data.data,BITS);


  for(int i=0;i<BITS;i++)
  {

    if(value & (1<<i))
      dst->item(BIT_MARK,ONE_SPACE);
    else
      dst->item(BIT_MARK,ZERO_SPACE);

  }


  dst->mark(BIT_MARK);

}




optional<NikaiData> NikaiProtocol::decode(RemoteReceiveData data)
{

  NikaiData out{};


  if(!data.expect_item(START_MARK,START_SPACE))
    return {};


  uint32_t value=0;


  for(int i=0;i<BITS;i++)
  {

    if(!data.expect_mark(BIT_MARK))
      return {};


    if(data.expect_space(ONE_SPACE))
    {
      value |= (1<<i);
    }
    else if(data.expect_space(ZERO_SPACE))
    {

    }
    else
    {
      return {};
    }

  }


  out.data = reverse_bits(value,BITS);


  ESP_LOGD(TAG,
      "Decoded Nikai frame: 0x%06" PRIX32,
      out.data);


  return out;

}




void NikaiProtocol::dump(const NikaiData &data)
{

 ESP_LOGI(TAG,
 "Received Nikai: data=0x%06" PRIX32,
 data.data);

}


}
}
