# CAN Frames

# OBD2 CAN Configuration

| Parameter | Value |
|---|---|
| Protocol | ISO15765-4 |
| Speed | 500kbps |
| CAN ID Type | 11-bit |

---

# OBD2 IDs

| Function | CAN ID |
|---|---|
| Functional Request | 0x7DF |
| ECU Response | 0x7E8 |

---

# PID 00 — Supported PIDs

## Request

ID:
```text
0x7DF
```

Data:
```text
02 01 00 00 00 00 00 00
```

---

## Response

ID:
```text
0x7E8
```

Data:
```text
06 41 00 00 10 00 00 00
```

---

# PID 0C — Engine RPM

## Request

ID:
```text
0x7DF
```

Data:
```text
02 01 0C 00 00 00 00 00
```

---

## Response Example

RPM = 3000

ID:
```text
0x7E8
```

Data:
```text
04 41 0C 2E E0 00 00 00
```

---

# RPM Formula

```text
RPM = ((A * 256) + B) / 4
```

Example:

```text
0x2E = 46
0xE0 = 224

RPM = ((46 * 256) + 224) / 4
RPM = 3000
```

---

# Future PIDs

| PID | Function |
|---|---|
| 05 | ECT |
| 0B | MAP |
| 0D | VSS |
| 0E | Spark Advance |
| 0F | IAT |
| 11 | TPS |
| 14 | O2 Sensor |
| 42 | Battery Voltage |
