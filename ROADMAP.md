# ROADMAP

## Stage 1 — Basic OBD2 Communication
- [x] MCP2515 initialization
- [x] OBD2 PID 00
- [x] OBD2 PID 0C (RPM)
- [x] Launch Creader connection test

---

## Stage 2 — Basic Sensors
- [x] PID 05 (ECT)
- [x] PID 0B (MAP)
- [x] PID 0F (IAT)
- [x] PID 11 (TPS)

---

## Stage 3 — Full Live Data
- [x] PID 0D (VSS)
- [x] PID 0E (Spark Advance)
- [x] PID 42 (Battery Voltage)
- [x] PID 14 (O2 Sensor)

---

## ~~Stage 4 — Advanced OBD2~~ *(Discontinued)*

- [ ] ~~DTC support~~
- [ ] ~~MIL support~~
- [ ] ~~Freeze Frame~~
- [ ] ~~Read/Clear DTC~~

### Status
Advanced diagnostic functionality was evaluated during development but later discontinued after real-world validation with the Launch Creader 3001.

The project focus shifted toward lightweight real-time live data compatibility for standalone ECUs, where full OEM-style diagnostic emulation provided limited practical value for the intended application.

Maintaining a simple and highly reliable implementation proved more beneficial than adding complex diagnostic layers.

---

## ~~Stage 5 — ISO-TP~~ *(Discontinued)*

- [ ] ~~Multi-frame CAN~~
- [ ] ~~Long packet support~~
- [ ] ~~Better scanner compatibility~~

### Status
ISO-TP support was initially considered during project planning but later deemed unnecessary after successful validation of all required live data functionality using standard single-frame CAN responses.

Scanner compatibility goals were achieved without requiring multi-frame transport protocols, significantly reducing implementation complexity and resource usage.

---

## Stage 6 — Optional Features
- [ ] ESP8266 WiFi bridge
- [ ] Bluetooth support
- [ ] SD datalogging
- [ ] Web dashboard
- [ ] CAN sniffing mode

---

## Stage 7 — Hardware
- [ ] Custom PCB
- [ ] OBD2 enclosure
- [ ] Automotive power supply
- [ ] Reverse polarity protection
