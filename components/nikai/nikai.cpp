#include "nikai.h"

#include "esphome/core/log.h"


namespace esphome {
namespace nikai {


static const char *TAG = "nikai";


void NikaiReceiver::setup() {

  ESP_LOGI(TAG, "NIKAI receiver loaded");

}


void NikaiReceiver::dump_config() {

  ESP_LOGCONFIG(TAG, "NIKAI receiver");

}


}
}
