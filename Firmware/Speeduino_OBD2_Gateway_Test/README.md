# Speeduino to OBD2 CAN 29-Bit Gateway

A stable CAN gateway that bridges Speeduino ECU data to OBD2 protocols over 29-bit CAN bus. Compatible with ELM327 interfaces, launch control systems, and standard car scanners.

## Features

- Converts Speeduino serial data to OBD2 standard PIDs
- Supports 29-bit CAN identifiers (OBD2 standard)
- Responds to diagnostic requests from ELM327 or any OBD2 scanner
- Anti-echo filtering to prevent CAN bus collisions
- Real-time engine data conversion

## Supported OBD2 PIDs

The gateway responds to the following diagnostic parameters:

| PID | Description | Source (Speeduino) |
|-----|-------------|---------------------|
| 0x00 | Supported PIDs 00-20 | Static bitmap |
| 0x01 | Monitor status | Static values |
| 0x04 | Engine load | `engine_load` |
| 0x05 | Coolant temperature | `temperatura_ect` |
| 0x0B | Manifold absolute pressure | `map_pressure` |
| 0x0C | Engine RPM | `rpm` (scaled ×4) |
| 0x0D | Vehicle speed | `velocidade` |
| 0x0E | Ignition timing advance | `adv_ignition` |
| 0x0F | Intake air temperature | `temperatura_iat` |
| 0x11 | Throttle position | `throttle` |
| 0x14 | Oxygen sensor / Lambda | `raw_afr` (converted to lambda) |
| 0x1C | OBD2 standards compliance | Fixed value |
| 0x1F | Engine run time | System uptime |
| 0x20 | Supported PIDs 21-40 | Bitmap |
| 0x34 | Oxygen sensor (alternate) | Lambda value |
| 0x40 | Supported PIDs 41-60 | Bitmap |
| 0x42 | Battery voltage | `battery10` (scaled) |
| 0x43 | Absolute load value | `ve` (Volumetric Efficiency) |
| 0x5D | Fuel injection timing | Fixed zero value |

## Hardware Requirements

- Arduino board (Uno/Nano/Mega compatible)
- MCP2515 CAN controller module
- Speeduino ECU (with serial output enabled)

## Pin Connections

| Component | Arduino Pin |
|-----------|-------------|
| MCP2515 CS | 10 |
| Speeduino RX | 4 |
| Speeduino TX | 5 |
| SPI | Standard (11,12,13) |

## Communication Protocols

- **Serial (Speeduino ↔ Arduino)**: 115200 baud
- **Serial (Debug)**: 115200 baud (USB)
- **CAN bus**: 500 kbps (OBD2 standard)

## CAN Identifiers

- **Request ID (OBD scanner → Gateway)**: `0x18DB33F1` (functional)
- **Response ID (Gateway → ECU)**: `0x18DAF110`

## How It Works

1. **Data Acquisition**: Requests data from Speeduino every 80ms using a simple `'A'` command
2. **Data Parsing**: Extracts engine parameters from the 25+ byte response packet
3. **CAN Listening**: Monitors CAN bus for OBD2 diagnostic requests
4. **PID Response**: Converts Speeduino values to OBD2 standard formats and responds

## Speeduino Data Packet Format

The code expects a 25+ byte response packet from Speeduino:

| Byte Offset | Parameter |
|-------------|-----------|
| 4-5 | MAP pressure (16-bit) |
| 6 | Intake air temperature |
| 7 | Coolant temperature |
| 9 | Battery voltage (×10) |
| 10 | Raw AFR (×10) |
| 14-15 | RPM (16-bit) |
| 17 | Engine load (%) |
| 18 | Volumetric efficiency (%) |
| 19 | AFR target |
| 23 | Ignition advance (signed) |
| 24 | Throttle position (%) |

## Installation

1. Install required libraries:
   - `mcp_can` by Cory J. Fowler
   - `SPI` (built-in)
   - `SoftwareSerial` (built-in)

2. Connect hardware according to pin configuration above

3. Upload code to Arduino

4. Monitor serial debug output at 115200 baud

## Usage

Once running:
- Connect an ELM327 or OBD2 scanner to the CAN bus (pins: CAN High, CAN Low)
- The gateway will automatically respond to diagnostic requests
- Use any OBD2 software (Torque, OBD Auto Doctor, etc.) to view engine data

## Notes

- The gateway is designed for 29-bit CAN identifiers only
- Anti-echo filtering prevents responding to its own messages
- Vehicle speed is currently set to 0 (can be enabled in the Speeduino configuration)
- Lambda values are calculated assuming stoichiometric AFR of 14.7:1

## Troubleshooting

**MCP2515 not detected**: Check SPI connections and CS pin (Pin 10)

**No OBD2 responses**: Verify CAN bus wiring and termination (120Ω resistor at each end)

**Incorrect data**: Check Speeduino serial communication and packet format

## License

Open source - feel free to modify and distribute.

## Credits

Designed for Speeduino ECU integration with standard OBD2 diagnostic tools.
