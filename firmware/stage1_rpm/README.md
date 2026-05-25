# Stage 1 — RPM OBD2 Communication

# Objective

Initial implementation of OBD2 communication between:

- Speeduino ECU
- Arduino Mega 2560
- MCP2515 CAN module
- Launch Creader 3001

This stage validates:
- CAN communication
- OBD2 protocol
- Scanner compatibility
- RPM PID response

---

# Features

## Implemented

| PID | Function |
|---|---|
| 00 | Supported PIDs |
| 0C | Engine RPM |

---

# Hardware

- Arduino Mega 2560
- MCP2515 + TJA1050
- Speeduino ECU
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
| Request | 0x7DF |
| Response | 0x7E8 |

---

# Serial Configuration

| Parameter | Value |
|---|---|
| Port | Serial1 |
| Baudrate | 115200 |

---

# Wiring

## Speeduino → Mega

| Speeduino | Mega |
|---|---|
| TX | RX1 (D19) |
| GND | GND |

---

## MCP2515 → Mega

| MCP2515 | Mega |
|---|---|
| CS | D10 |
| SO | D50 |
| SI | D51 |
| SCK | D52 |
| INT | D2 |

---

# Serial Packet

Current packet format:

```text
BYTE0 = RPM_H
BYTE1 = RPM_L
```

---

# RPM Formula

```text
RPM = ((A * 256) + B) / 4
```

---

# Example

3000 RPM:

```text
3000 * 4 = 12000
12000 = 0x2EE0

A = 0x2E
B = 0xE0
```

CAN response:

```text
04 41 0C 2E E0 00 00 00
```

---

# OBD2 Requests

## PID 00

Request:

```text
02 01 00 00 00 00 00 00
```

Response:

```text
06 41 00 00 10 00 00 00
```

---

## PID 0C

Request:

```text
02 01 0C 00 00 00 00 00
```

Response example:

```text
04 41 0C 2E E0 00 00 00
```

---

# Validation Goals

- [ ] Scanner powers on
- [ ] Scanner detects CAN protocol
- [ ] Scanner connects successfully
- [ ] Scanner displays RPM
- [ ] Stable CAN communication

---

# Known Limitations

Current implementation:
- Single-frame CAN only
- No ISO-TP
- No DTC support
- RPM only

---

# Next Stage

## Stage 2 — Basic Sensors

Planned:
- ECT
- MAP
- TPS
- IAT