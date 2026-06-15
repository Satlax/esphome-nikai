#pragma once

#include "esphome/core/component.h"
#include "esphome/components/remote_receiver/remote_receiver.h"

namespace esphome {
namespace nikai {


class NikaiReceiver : public Component {

 public:

  void setup() override;

  void dump_config() override;


 private:

  remote_receiver::RemoteReceiverComponent *receiver_;

};


}
}
