#pragma once

#include "esphome/core/component.h"
#include "esphome/components/fan/fan.h"
#include "esphome/components/light/light_output.h"
#include "esphome/components/remote_base/nec_protocol.h"
#include "esphome/components/remote_base/remote_base.h"

#ifdef USE_BUTTON
#include "esphome/components/button/button.h"
#endif

#include <queue>

namespace esphome {
namespace nec_fan_light {

static const uint16_t NEC_ADDRESS = 0x7F80;
static const uint16_t CMD_FAN_ON = 0xE41B;
static const uint16_t CMD_FAN_OFF = 0xE01F;
static const uint16_t CMD_FAN_SPEED_DOWN = 0xEE11;
static const uint16_t CMD_FAN_REVERSE = 0xF807;
static const uint16_t CMD_FAN_NATURAL = 0xB34C;
static const uint16_t CMD_LIGHT_ON = 0xE21D;
static const uint16_t CMD_LIGHT_OFF = 0xE31C;

static const uint32_t DEFAULT_COMMAND_DELAY_MS = 500;

class NecFanLight : public Component, public remote_base::RemoteTransmittable {
 public:
  void set_command_delay(uint32_t delay_ms) { this->command_delay_ = delay_ms; }

  void send_command(uint16_t command) {
    this->queue_.push(command);
    if (!this->sending_) {
      this->process_queue_();
    }
  }

  void dump_config() override;

 protected:
  void process_queue_() {
    if (this->queue_.empty()) {
      this->sending_ = false;
      return;
    }
    this->sending_ = true;
    uint16_t cmd = this->queue_.front();
    this->queue_.pop();

    remote_base::NECData data{.address = NEC_ADDRESS, .command = cmd, .command_repeats = 1};
    this->transmit_<remote_base::NECProtocol>(data);

    if (!this->queue_.empty()) {
      this->set_timeout("ir_queue", this->command_delay_, [this]() { this->process_queue_(); });
    } else {
      this->sending_ = false;
    }
  }

  std::queue<uint16_t> queue_;
  bool sending_{false};
  uint32_t command_delay_{DEFAULT_COMMAND_DELAY_MS};
};

class NecFan : public Component, public fan::Fan {
 public:
  NecFan(NecFanLight *parent) : parent_(parent) {}

  fan::FanTraits get_traits() override {
    auto traits = fan::FanTraits();
    traits.set_speed(true);
    traits.set_supported_speed_count(6);
    traits.set_direction(true);
    return traits;
  }

  void setup() override { this->set_supported_preset_modes({"natural_wind"}); }
  void dump_config() override;

  void control(const fan::FanCall &call) override;

 protected:
  NecFanLight *parent_;
  int current_speed_{0};
};

class NecLight : public Component, public light::LightOutput {
 public:
  NecLight(NecFanLight *parent) : parent_(parent) {}

  light::LightTraits get_traits() override {
    auto traits = light::LightTraits();
    traits.set_supported_color_modes({light::ColorMode::ON_OFF});
    return traits;
  }

  void write_state(light::LightState *state) override;
  void setup_state(light::LightState *state) override { this->state_ = state; }

 protected:
  NecFanLight *parent_;
  light::LightState *state_{nullptr};
  bool is_on_{false};
};

#ifdef USE_BUTTON
class NecButton : public Component, public button::Button {
 public:
  NecButton(NecFanLight *parent, uint16_t command) : parent_(parent), command_(command) {}

  void press_action() override { this->parent_->send_command(this->command_); }

 protected:
  NecFanLight *parent_;
  uint16_t command_;
};
#endif

}  // namespace nec_fan_light
}  // namespace esphome
