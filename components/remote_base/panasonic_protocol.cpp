#include "panasonic_protocol.h"
#include "nikai_protocol.h"
#include "esphome/core/log.h"

namespace esphome::remote_base {

static const char *const TAG = "remote.panasonic";
namespace esphome {
namespace remote_base {

static constexpr uint32_t HEADER_HIGH_US = 3502;
static constexpr uint32_t HEADER_LOW_US = 1750;
static constexpr uint32_t BIT_HIGH_US = 502;
static constexpr uint32_t BIT_ZERO_LOW_US = 400;
static constexpr uint32_t BIT_ONE_LOW_US = 1244;

void PanasonicProtocol::encode(RemoteTransmitData *dst, const PanasonicData &data) {
  dst->reserve(100);
  dst->item(HEADER_HIGH_US, HEADER_LOW_US);
  dst->set_carrier_frequency(35000);
static const char *TAG = "remote.nikai";

  uint32_t mask;
  for (mask = 1UL << 15; mask != 0; mask >>= 1) {
    if (data.address & mask) {
      dst->item(BIT_HIGH_US, BIT_ONE_LOW_US);
    } else {
      dst->item(BIT_HIGH_US, BIT_ZERO_LOW_US);
    }

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

  for (mask = 1UL << 31; mask != 0; mask >>= 1) {
    if (data.command & mask) {
      dst->item(BIT_HIGH_US, BIT_ONE_LOW_US);
    } else {
      dst->item(BIT_HIGH_US, BIT_ZERO_LOW_US);
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
  dst->mark(BIT_HIGH_US);


  dst->mark(BIT_MARK);

}
optional<PanasonicData> PanasonicProtocol::decode(RemoteReceiveData src) {
  PanasonicData out{
      .address = 0,
      .command = 0,
  };
  if (!src.expect_item(HEADER_HIGH_US, HEADER_LOW_US))




optional<NikaiData> NikaiProtocol::decode(RemoteReceiveData data)
{

  NikaiData out{};


  if(!data.expect_item(START_MARK,START_SPACE))
return {};

  uint32_t mask;
  for (mask = 1UL << 15; mask != 0; mask >>= 1) {
    if (src.expect_item(BIT_HIGH_US, BIT_ONE_LOW_US)) {
      out.address |= mask;
    } else if (src.expect_item(BIT_HIGH_US, BIT_ZERO_LOW_US)) {
      out.address &= ~mask;
    } else {

  uint32_t value=0;


  for(int i=0;i<BITS;i++)
  {

    if(!data.expect_mark(BIT_MARK))
return {};


    if(data.expect_space(ONE_SPACE))
    {
      value |= (1<<i);
}
  }
    else if(data.expect_space(ZERO_SPACE))
    {

  for (mask = 1UL << 31; mask != 0; mask >>= 1) {
    if (src.expect_item(BIT_HIGH_US, BIT_ONE_LOW_US)) {
      out.command |= mask;
    } else if (src.expect_item(BIT_HIGH_US, BIT_ZERO_LOW_US)) {
      out.command &= ~mask;
    } else {
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
void PanasonicProtocol::dump(const PanasonicData &data) {
  ESP_LOGI(TAG, "Received Panasonic: address=0x%04X, command=0x%08" PRIX32, data.address, data.command);




void NikaiProtocol::dump(const NikaiData &data)
{

 ESP_LOGI(TAG,
 "Received Nikai: data=0x%06" PRIX32,
 data.data);

}

}  // namespace esphome::remote_base

}
}
