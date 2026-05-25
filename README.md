# Speeduino OBD2 Gateway

Unofficial community project for Speeduino ECU integration.

OBD2 CAN gateway using:
- Arduino Mega 2560
- MCP2515
- Speeduino Secondary Serial

Allows generic OBD2 scanners such as:
- Launch Creader 3001

to read live engine data from Speeduino ECUs.

---

# Features

## Current
- OBD2 PID 00
- OBD2 PID 0C (RPM)

## Planned
- TPS
- MAP
- ECT
- IAT
- VSS
- Battery Voltage
- Spark Advance
- O2 Sensor
- DTC support

---

# CAN Configuration

- ISO15765-4
- 500kbps
- 11-bit

---

# Hardware

- Arduino Mega 2560
- MCP2515 + TJA1050
- Speeduino ECU

---

# License

MIT License
