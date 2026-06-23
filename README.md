# esphome-nec-fan-light

ESPHome external component for NEC IR ceiling fan+light combos.

Creates native `fan` and `light` entities in Home Assistant with full control over speed, direction, and lighting.

## Compatible Devices

- Inspire Estepa (Leroy Merlin)
- Brilliant Mazzaro
- Brilliant Salerno
- Other OEM ceiling fans using NEC address `0x7F80` / LG format `0x01FE`

## Features

- **Fan**: on/off, 6 speeds, direction toggle, natural wind preset
- **Light**: on/off
- **Buttons**: brightness ±, CCT warm/cold, RGB mode, RGB speed ±, timer, off 60s

## Usage

```yaml
external_components:
  - source: github://andredp/esphome-nec-fan-light@main
    components: [nec_fan_light]

remote_transmitter:
  pin: GPIO4
  carrier_duty_percent: 50%

nec_fan_light:
  id: ceiling_fan

fan:
  - platform: nec_fan_light
    name: "Bedroom Fan"
    nec_fan_light_id: ceiling_fan

light:
  - platform: nec_fan_light
    name: "Bedroom Fan Light"
    nec_fan_light_id: ceiling_fan

button:
  - platform: nec_fan_light
    nec_fan_light_id: ceiling_fan
    brightness_up:
      name: "Brightness +"
    brightness_down:
      name: "Brightness -"
    cct_warm:
      name: "CCT Warmer"
    cct_cold:
      name: "CCT Colder"
    rgb_mode:
      name: "RGB Mode"
    rgb_speed_up:
      name: "RGB Speed +"
    rgb_speed_down:
      name: "RGB Speed -"
    timer:
      name: "Timer"
    off_60s:
      name: "Off 60s"
```

## How it Works

The component sends standard NEC IR codes via ESPHome's `remote_transmitter`. Fan speed is tracked internally (optimistic state) — when you set speed 4, the component calculates how many speed+/speed- commands to send from the current position.

Since IR is one-directional, the component cannot know the actual device state if the physical remote is used. The assumed state will re-sync on the next command.

## Protocol

See the [protocol documentation](https://github.com/andredp/home-network/blob/main/estepa-fan-ir/estepa_protocol.md) for the full NEC code table.

## License

MIT
