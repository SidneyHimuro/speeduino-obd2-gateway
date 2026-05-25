# OBD2 PIDs

# Supported PIDs

| PID | Description | Status |
|---|---|---|
| 00 | Supported PIDs | Implemented |
| 0C | Engine RPM | Implemented |
| 05 | Engine Coolant Temp | Planned |
| 0B | MAP Sensor | Planned |
| 0D | Vehicle Speed | Planned |
| 0E | Spark Advance | Planned |
| 0F | Intake Air Temp | Planned |
| 11 | Throttle Position | Planned |
| 14 | O2 Sensor Bank1 Sensor1 | Planned |
| 42 | Battery Voltage | Planned |

---

# PID 00

## Description
Returns bitmap of supported PIDs.

## Request
```text
01 00
```

## Response
```text
41 00
```

---

# PID 0C — Engine RPM

## Formula

```text
RPM = ((A * 256) + B) / 4
```

## Example

3000 RPM:

```text
A = 0x2E
B = 0xE0
```

---

# PID 05 — Engine Coolant Temperature

## Formula

```text
Temperature = A - 40
```

---

# PID 0F — Intake Air Temperature

## Formula

```text
Temperature = A - 40
```

---

# PID 11 — Throttle Position

## Formula

```text
TPS % = (A * 100) / 255
```

---

# PID 0B — MAP Sensor

## Formula

```text
MAP (kPa) = A
```

---

# PID 0D — Vehicle Speed

## Formula

```text
Speed (km/h) = A
```

---

# PID 0E — Spark Advance

## Formula

```text
Advance = (A / 2) - 64
```

---

# PID 42 — Battery Voltage

## Formula

```text
Voltage = ((A * 256) + B) / 1000
```
