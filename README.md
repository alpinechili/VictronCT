# VictronCT

**An ESP32 firmware for monitoring and extending a Victron Cerbo GX using Modbus TCP and MQTT.**

---

## Features

- Battery monitoring
- MQTT publishing
- Home Assistant integration
- Automatic Modbus register scanner
- Modular architecture
- CT clamp expansion (coming soon)
- OTA updates (planned)

---

## Hardware

- ESP32
- Victron Cerbo GX
- Ethernet/WiFi network
- MottramLabs ESP32 4 Channel Current Sensor (future)

---

## Current Data

| Parameter | Status |
|-----------|--------|
| Battery Voltage | ✅ |
| Battery Current | ✅ |
| Battery Power | ✅ |
| Battery SOC | ✅ |
| Grid Power | 🚧 |
| PV Power | 🚧 |
| AC Loads | 🚧 |
| CT Sensors | 🚧 |

---

## MQTT Topics

VictronCT publishes:

```
VictronCT/battery/soc
VictronCT/battery/voltage
VictronCT/battery/current
VictronCT/battery/power
```

---

## Architecture

```
                Victron Cerbo GX
                        │
                Modbus TCP Manager
                        │
                  VictronData
                 ▲            ▲
                 │            │
           CT Manager    MQTT Publisher
                 │            │
                 └──────┬─────┘
                        │
                 Home Assistant
```

---

## Building

```
git clone https://github.com/alpinechili/VictronCT.git
```

Copy

```
include/secrets_template.h
```

to

```
include/secrets.h
```

Edit your WiFi and MQTT settings.

Compile using PlatformIO.

---

## Roadmap

- [x] Battery monitoring
- [x] MQTT
- [x] Register scanner
- [ ] Grid monitoring
- [ ] PV monitoring
- [ ] AC Load monitoring
- [ ] MQTT Discovery
- [ ] OTA Updates
- [ ] CT Clamp support

---

## Licence

MIT
