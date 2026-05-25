# Speeduino Secondary Serial

# Overview

This project uses the Speeduino Secondary Serial interface to retrieve runtime ECU data and convert it into standard OBD2 CAN messages.

This approach avoids parsing the full TunerStudio protocol and greatly simplifies implementation on Arduino Mega 2560.

---

# Configuration

## Recommended Protocol

```text
Generic Fixed
```

or

```text
RealDash
```

---

# Recommended Baudrate

```text
115200
```

---

# Connection

| Speeduino | Mega 2560 |
|---|---|
| TX | RX1 (D19) |
| RX | TX1 (D18) *(optional)* |
| GND | GND |

---

# Runtime Data

The following runtime values are planned:

| Variable | Description |
|---|---|
| RPM | Engine RPM |
| TPS | Throttle Position |
| MAP | Manifold Pressure |
| ECT | Coolant Temperature |
| IAT | Intake Air Temperature |
| VSS | Vehicle Speed |
| Spark | Ignition Advance |
| Battery | System Voltage |
| O2 | O2 Sensor |
| Lambda | AFR/Lambda |

---

# Example Packet

```text
BYTE 0   RPM_H
BYTE 1   RPM_L
BYTE 2   TPS
BYTE 3   MAP
BYTE 4   ECT
BYTE 5   IAT
BYTE 6   VSS
BYTE 7   SPARK
BYTE 8   BAT_H
BYTE 9   BAT_L
BYTE 10  O2
BYTE 11  LAMBDA_H
BYTE 12  LAMBDA_L
```

---

# Advantages

- Low CPU usage
- Low RAM usage
- Simple parsing
- Fast response time
- Ideal for Arduino Mega 2560

---

# Future Improvements

- Automatic packet synchronization
- CRC validation
- RealDash compatibility
- TunerStudio passthrough mode
- CAN forwarding
