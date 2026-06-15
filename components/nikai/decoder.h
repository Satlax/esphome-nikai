#pragma once


#include "esphome/components/remote_receiver/remote_receiver.h"


namespace esphome {
namespace nikai {


class NikaiDecoder
{

 public:

  static bool decode(
      remote_base::RemoteReceiveData &data,
      uint32_t *result
  );

};


}
}
