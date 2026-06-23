#include "nec_fan_light.h"
#include "esphome/core/log.h"

namespace esphome {
namespace nec_fan_light {

static const char *const TAG = "nec_fan_light";

void NecFanLight::dump_config() {
  ESP_LOGCONFIG(TAG, "NEC Fan Light:");
  ESP_LOGCONFIG(TAG, "  Command delay: %u ms", this->command_delay_);
}

void NecFan::dump_config() { ESP_LOGCONFIG(TAG, "NEC Fan: speed_count=6, direction=yes, preset=natural_wind"); }

void NecFan::control(const fan::FanCall &call) {
  if (call.get_state().has_value()) {
    bool new_state = *call.get_state();
    if (new_state) {
      this->parent_->send_command(CMD_FAN_ON);
      if (this->current_speed_ == 0)
        this->current_speed_ = 1;
    } else {
      this->parent_->send_command(CMD_FAN_OFF);
      this->current_speed_ = 0;
    }
    this->state = new_state;
  }

  if (call.get_speed().has_value()) {
    int target = *call.get_speed();
    if (target < 1)
      target = 1;
    if (target > 6)
      target = 6;

    if (this->current_speed_ == 0) {
      // Fan is off — turn on first (starts at speed 1)
      this->parent_->send_command(CMD_FAN_ON);
      this->current_speed_ = 1;
      this->state = true;
    }

    int delta = target - this->current_speed_;
    if (delta > 0) {
      for (int i = 0; i < delta; i++)
        this->parent_->send_command(CMD_FAN_ON);
    } else if (delta < 0) {
      for (int i = 0; i < -delta; i++)
        this->parent_->send_command(CMD_FAN_SPEED_DOWN);
    }
    this->current_speed_ = target;
    this->speed = target;
    this->state = true;
  }

  if (call.get_direction().has_value()) {
    this->parent_->send_command(CMD_FAN_REVERSE);
    this->direction = *call.get_direction();
  }

  if (call.has_preset_mode()) {
    const char *mode = call.get_preset_mode();
    if (mode != nullptr && std::string(mode) == "natural_wind") {
      this->parent_->send_command(CMD_FAN_NATURAL);
    }
  }

  this->publish_state();
}

void NecLight::write_state(light::LightState *state) {
  bool target;
  state->current_values_as_binary(&target);

  if (target == this->is_on_)
    return;

  if (target) {
    this->parent_->send_command(CMD_LIGHT_ON);
  } else {
    this->parent_->send_command(CMD_LIGHT_OFF);
  }
  this->is_on_ = target;
}

}  // namespace nec_fan_light
}  // namespace esphome
