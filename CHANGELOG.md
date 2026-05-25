# CHANGELOG

All notable changes to this project will be documented in this file.

The format is based on:
- Keep a Changelog
- Semantic Versioning (SemVer)

---

# [0.1.0-alpha] - 2026-05-24

## Added

### Project
- Initial GitHub repository
- Initial project structure
- MIT License
- README.md
- ROADMAP.md
- CHANGELOG.md

### Hardware
- Arduino Mega 2560 selected as main platform
- MCP2515 CAN controller support
- TJA1050 CAN transceiver support

### CAN / OBD2
- ISO15765-4 CAN configuration
- 500kbps CAN support
- 11-bit CAN identifiers
- OBD2 request ID: 0x7DF
- OBD2 response ID: 0x7E8

### Speeduino Integration
- Secondary Serial communication support
- Serial1 communication at 115200 baud
- Initial runtime data parser

### OBD2 Features
- PID 00 (Supported PIDs)
- PID 0C (Engine RPM)

### Scanner Compatibility
- Initial compatibility target:
  - Launch Creader 3001

---

# [Planned]

## 0.2.0
- PID 05 (ECT)
- PID 0B (MAP)
- PID 0F (IAT)
- PID 11 (TPS)

## 0.3.0
- PID 0D (VSS)
- PID 0E (Spark Advance)
- PID 42 (Battery Voltage)
- PID 14 (O2 Sensor)

## 0.4.0
- DTC support
- MIL support
- Freeze frame
- Clear DTC

## 0.5.0
- ISO-TP multi-frame support
- Improved scanner compatibility

## Future
- ESP8266 WiFi bridge
- Bluetooth support
- SD datalogging
- Web dashboard
- Custom PCB
- Automotive enclosure
