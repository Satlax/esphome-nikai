#pragma once

#include "esphome/components/remote_base/remote_base.h"


namespace esphome {
namespace nikai {


class NikaiProtocol : public remote_base::RemoteReceiver {

 public:

  bool decode(
      remote_base::RemoteReceiveData data
  ) override;


};


}
}
