# OBD2 Simulator

Functional OBD2 CAN simulator compatible with generic automotive scanners.

Validated with:
- Launch Creader 3001

This project simulates a basic OBD2 ECU using:
- Arduino UNO
- MCP2515 CAN controller
- SAE J1979 standard PIDs

---

# Features

## Supported PIDs

| PID | Description |
|---|---|
| 00 | Supported PIDs |
| 01 | Monitor Status |
| 04 | Engine Load |
| 05 | Engine Coolant Temperature |
| 0B | MAP Sensor |
| 0C | Engine RPM |
| 0D | Vehicle Speed |
| 0F | Intake Air Temperature |
| 10 | MAF Air Flow |
| 11 | Throttle Position |
| 14 | O2 Sensor |
| 1C | OBD Standard |

---

# Hardware

- Arduino UNO
- MCP2515 + TJA1050
- OBD2 Scanner

Validated with:
- Launch Creader 3001

---

# CAN Configuration

| Parameter | Value |
|---|---|
| Protocol | ISO15765-4 |
| Speed | 500kbps |
| CAN ID Type | 11-bit |

---

# CAN IDs

| Function | ID |
|---|---|
| Functional Request | 0x7DF |
| ECU Response | 0x7E8 |

---

# Wiring

## MCP2515 ↔ Arduino UNO

| MCP2515 | Arduino UNO |
|---|---|
| VCC | 5V |
| GND | GND |
| CS | D10 |
| SO (MISO) | D12 |
| SI (MOSI) | D11 |
| SCK | D13 |
| INT | D2 |

---

# OBD2 Connector

| OBD2 Pin | Function |
|---|---|
| 6 | CAN-H |
| 14 | CAN-L |
| 4 | GND |
| 5 | GND |

---

# Dynamic Simulation

The simulator generates dynamic values for:
- RPM
- Vehicle Speed
- Engine Temperature
- TPS
- MAP
- Engine Load
- MAF
- O2 Sensor

This allows realistic scanner testing without a real ECU.

---

# Serial Debug

USB Serial:
```text
115200 baud
```

Example output:

```text
REQ -> 0x7DF | Mode: 0x01 | PID: 0x0C
TX -> 0x7E8 | 04 41 0C 2E E0
```

---

# Notes

This simulator became the foundation for the future:

- Speeduino OBD2 Gateway
- Real-time ECU live data bridge
- CAN development platform
- OBD2 compatibility testing

---

# Project Goals

- Lightweight implementation
- Maximum scanner compatibility
- Simple CAN architecture
- Easy integration with standalone ECUs

---

# Current Status

Validated:
- Launch Creader 3001 communication
- Single-frame SAE J1979 live data
- Stable CAN communication at 500kbps

---

# Future

Planned future integration:
- Speeduino Secondary Serial
- Real ECU runtime data
- WiFi bridge
- Bluetooth bridge
- Custom automotive PCB

---

# License

MIT License
