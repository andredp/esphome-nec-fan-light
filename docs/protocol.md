# Inspire Estepa Ceiling Fan+Light IR Protocol

## Overview

- **Model:** Inspire Estepa (Leroy Merlin, ref 88405654)
- **Type:** Ceiling fan with LED light (CCT + RGB), 58cm, 7650 lm
- **Remote model:** 1CN-12V100345PO (sticker says "RC-type: Infrered(IR)")
- **Protocol:** NEC (extended address)
- **Address:** 0x7F80 (NEC extended)
- **Carrier frequency:** 38 kHz
- **Location:** Controlled via upper corridor Shelly switch (power on/off)

## NEC Codes

| Button | NEC Address | NEC Command | LG Format (hex) | Notes |
|--------|-------------|-------------|-----------------|-------|
| Fan Off | 0x7F80 | 0xE01F | 0x01FEF807 | Stops fan, remembers last speed |
| Fan On / Speed + | 0x7F80 | 0xE41B | 0x01FED827 | Turns on or cycles speed 1→6 |
| Fan Speed - | 0x7F80 | 0xEE11 | 0x01FE8877 | |
| Fan Reverse | 0x7F80 | 0xF807 | 0x01FEE01F | Toggle direction |
| Fan Natural Wind | 0x7F80 | 0xB34C | 0x01FE32CD | Breeze/pulse mode |
| Light On | 0x7F80 | 0xE21D | 0x01FEB847 | |
| Light Off | 0x7F80 | 0xE31C | 0x01FE38C7 | |
| Brightness + | 0x7F80 | 0xED12 | 0x01FE48B7 | |
| Brightness - | 0x7F80 | 0xFA05 | 0x01FEA05F | |
| CCT Warm (+) | 0x7F80 | 0xE11E | 0x01FE7887 | |
| CCT Cold (-) | 0x7F80 | 0xE51A | 0x01FE58A7 | |
| RGB Mode | 0x7F80 | 0xF50A | 0x01FE50AF | Cycles RGB colors |
| RGB Speed + | 0x7F80 | 0xF40B | 0x01FED02F | Faster RGB cycling |
| RGB Speed - | 0x7F80 | 0xEF10 | 0x01FE08F7 | Slower RGB cycling |
| Timer | 0x7F80 | 0xFC03 | 0x01FEC03F | Cycles 1h/2h/4h |
| Off 60s | 0x7F80 | 0xF30C | 0x01FE30CF | Delayed off (60 seconds) |

### Fan Off
```
NEC: address=0x7F80, command=0xE01F
LG: data=0x01FEF807
```

## Protocol Characteristics

- Standard NEC with extended 16-bit address (0x7F80)
- **Fan: 6 speed levels** — speed+ cycles 1→2→3→4→5→6, speed- reverses
- Fan on (speed+) resumes last speed; fan off remembers previous speed
- Light brightness is relative (increment/decrement)
- CCT is relative (warmer/colder steps)
- RGB mode cycles through preset colors
- Timer cycles through 1h → 2h → 4h → off
- Fan on and fan speed+ share the same code (0xE41B)
- Repeat frame: 9000µs mark + 2250µs space (standard NEC repeat)

## Comparison with Brilliant Mazzaro/Salerno

The codes are **identical** to the Brilliant Mazzaro/Salerno ceiling fan+light:
- Same NEC address structure (0x01FE prefix in LG format)
- Same command codes for matching functions
- Same Chinese OEM (common for Leroy Merlin Inspire range)

Reference: https://gist.github.com/pedzed/a1a8106dede193274ff89ba54b725232

## Raw Captures

### Fan Off (from dump: raw, pre-NEC decode)
```
[13:11:05.429] Received Raw: 9011, -4485, 613, -515, 580, -549, 581, -548, 580, -549, 581, -547,
582, -548, 581, -548, 580, -1680, 610, -1672, 585, -1643, 614, -1643, 614, -1672, 584, -1645,
612, -1644, 612, -1645, 610, -517, 579, -549, 581, -547, 581, -1677, 611, -1647, 610, -1646,
610, -516, 580, -549, 580, -548, 581, -1678, 611, -1644, 612, -516, 580, -549, 580, -548, 581,
-1677, 612, -1646, 611, -1647, 610
```

### Fan On / Speed 1 (NEC decoded)
```
NEC: address=0x7F80, command=0xE41B
LG: data=0x01FED827, nbits=32
```

### Fan Speed + (repeated presses, all same code)
```
NEC: address=0x7F80, command=0xE41B (×5)
```

### Fan Reverse
```
NEC: address=0x7F80, command=0xF807
LG: data=0x01FEE01F
```

### Light Off
```
NEC: address=0x7F80, command=0xE31C
LG: data=0x01FE38C7
```

### Light On
```
NEC: address=0x7F80, command=0xE21D
LG: data=0x01FEB847
```

### Brightness +
```
NEC: address=0x7F80, command=0xED12
LG: data=0x01FE48B7
```

### Brightness -
```
NEC: address=0x7F80, command=0xFA05
LG: data=0x01FEA05F
```

### CCT Warm
```
NEC: address=0x7F80, command=0xE11E
LG: data=0x01FE7887
```

### CCT Cold
```
NEC: address=0x7F80, command=0xE51A
LG: data=0x01FE58A7
```

### RGB Mode
```
NEC: address=0x7F80, command=0xF50A
LG: data=0x01FE50AF
```

### Timer (cycles 1h/2h/4h)
```
NEC: address=0x7F80, command=0xFC03
LG: data=0x01FEC03F
```

### Fan Speed -
```
NEC: address=0x7F80, command=0xEE11
LG: data=0x01FE8877
```

## TODO

- [x] Re-capture fan off with `dump: all` — confirmed: 0xE01F
- [x] Determine total number of fan speed steps — 6 speeds
- [x] Build ESPHome component repo (esphome-nec-fan-light)
- [ ] Determine total number of brightness steps
- [ ] Determine total number of CCT steps
- [ ] Add `ir_rf_proxy` to the Athom config for receiver-based state sync

## Potentially Missing Buttons (from Brilliant Mazzaro reference)

Based on the Brilliant Mazzaro/Salerno which shares the same OEM codes:
- **Light SOS / Night mode** — Mazzaro code: `01FE30CF` (NEC cmd 0x0CF3?)

Check your remote for any buttons not yet captured.
