#include "nikai_protocol.h"


namespace esphome {
namespace nikai {


bool NikaiProtocol::decode(
    remote_base::RemoteReceiveData data
) {

  auto timing = data.get_data();

  if (timing.size() < 25)
    return false;


  // NIKAI:
  // header ~4000/4000
  // bit0 ~500/1000
  // bit1 ~500/2000


  if (abs(timing[0] - 4000) > 800)
    return false;


  uint32_t value = 0;


  for (int i=0;i<24;i++) {

    int idx = 2 + i*2;

    if (idx >= timing.size())
      return false;


    uint32_t bit =
      timing[idx+1] > 1500;


    value <<= 1;
    value |= bit;

  }


  this->result.protocol = "NIKAI";
  this->result.address = 0;
  this->result.command = value & 0xFFFFFF;
  this->result.data = value;


  return true;

}


}
}
