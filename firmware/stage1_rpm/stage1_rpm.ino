#include <SPI.h>
#include <mcp_can.h>

// ======================================================
// MCP2515 CONFIG
// ======================================================

#define CAN_CS 10

MCP_CAN CAN(CAN_CS);

// ======================================================
// VARIABLES
// ======================================================

uint16_t rpm = 0;

// ======================================================
// SERIAL BUFFER
// ======================================================

uint8_t serialBuffer[2];
uint8_t serialIndex = 0;

// ======================================================
// SETUP
// ======================================================

void setup()
{
    // USB DEBUG
    Serial.begin(115200);

    // SPEEDUINO SERIAL
    Serial1.begin(115200);

    Serial.println();
    Serial.println("=================================");
    Serial.println("Speeduino OBD2 Gateway");
    Serial.println("Stage 1 - RPM");
    Serial.println("=================================");

    // MCP2515 INIT
    while (CAN_OK != CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ))
    {
        Serial.println("MCP2515 init FAIL");
        delay(500);
    }

    CAN.setMode(MCP_NORMAL);

    Serial.println("MCP2515 init OK");
    Serial.println("Waiting OBD2 requests...");
}

// ======================================================
// MAIN LOOP
// ======================================================

void loop()
{
    readSpeeduino();

    processOBD();
}

// ======================================================
// READ SPEEDUINO SERIAL
//
// CURRENT PACKET:
// BYTE0 = RPM_H
// BYTE1 = RPM_L
// ======================================================

void readSpeeduino()
{
    while (Serial1.available())
    {
        uint8_t b = Serial1.read();

        serialBuffer[serialIndex++] = b;

        if (serialIndex >= 2)
        {
            rpm = ((uint16_t)serialBuffer[0] << 8) |
                   serialBuffer[1];

            serialIndex = 0;

            Serial.print("RPM: ");
            Serial.println(rpm);
        }
    }
}

// ======================================================
// PROCESS OBD2 REQUESTS
// ======================================================

void processOBD()
{
    unsigned long rxId;
    unsigned char len;
    unsigned char rxBuf[8];

    // CHECK CAN MESSAGE
    if (CAN_MSGAVAIL == CAN.checkReceive())
    {
        CAN.readMsgBuf(&rxId, &len, rxBuf);

        // DEBUG
        Serial.print("CAN ID: 0x");
        Serial.println(rxId, HEX);

        // ONLY OBD2 FUNCTIONAL REQUEST
        if (rxId != 0x7DF)
            return;

        uint8_t mode = rxBuf[1];
        uint8_t pid  = rxBuf[2];

        Serial.print("MODE: 0x");
        Serial.print(mode, HEX);

        Serial.print(" PID: 0x");
        Serial.println(pid, HEX);

        // ONLY MODE 01
        if (mode != 0x01)
            return;

        switch (pid)
        {
            // SUPPORTED PIDS
            case 0x00:
                sendSupportedPIDs();
                break;

            // ENGINE RPM
            case 0x0C:
                sendRPM();
                break;
        }
    }
}

// ======================================================
// PID 00
// SUPPORTED PIDS
//
// CURRENT:
// 0C = RPM
// ======================================================

void sendSupportedPIDs()
{
    uint8_t tx[8];

    tx[0] = 0x06;
    tx[1] = 0x41;
    tx[2] = 0x00;

    /*
       PID 0C bitmap
    */

    tx[3] = 0x00;
    tx[4] = 0x10;
    tx[5] = 0x00;
    tx[6] = 0x00;
    tx[7] = 0x00;

    CAN.sendMsgBuf(0x7E8, 0, 8, tx);

    Serial.println("PID 00 response sent");
}

// ======================================================
// PID 0C
// ENGINE RPM
//
// RPM = ((A * 256) + B) / 4
// ======================================================

void sendRPM()
{
    uint16_t obdRpm = rpm * 4;

    uint8_t A = obdRpm >> 8;
    uint8_t B = obdRpm & 0xFF;

    uint8_t tx[8] =
    {
        0x04,
        0x41,
        0x0C,
        A,
        B,
        0x00,
        0x00,
        0x00
    };

    CAN.sendMsgBuf(0x7E8, 0, 8, tx);

    Serial.print("RPM response sent: ");
    Serial.println(rpm);
}