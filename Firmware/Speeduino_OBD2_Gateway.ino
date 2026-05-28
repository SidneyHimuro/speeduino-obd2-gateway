#include <mcp_can.h>
#include <SPI.h>
#include <SoftwareSerial.h>

// ======================================================
// SPEEDUINO -> OBD2 CAN 29BIT GATEWAY
// ESTÁVEL PARA LAUNCH / ELM327 / CAR SCANNER
// ======================================================

// ======================================================
// MCP2515
// ======================================================
const int SPI_CS_PIN = 10;

// ======================================================
// SPEEDUINO SERIAL
// ======================================================
const int SPEEDUINO_RX = 4;
const int SPEEDUINO_TX = 5;

SoftwareSerial speeduinoSerial(SPEEDUINO_RX, SPEEDUINO_TX);

MCP_CAN CAN(SPI_CS_PIN);

// ======================================================
// OBD2 29-BIT IDS
// ======================================================

// Functional request
const unsigned long OBD_REQ_ID  = 0x18DB33F1;

// ECU response
const unsigned long OBD_RESP_ID = 0x18DAF110;

// ======================================================
// SPEEDUINO VARIABLES
// ======================================================
uint16_t rpm = 0;
uint16_t map_pressure = 100;
uint8_t temperatura_iat = 40;
uint8_t temperatura_ect = 40;
uint8_t throttle = 0;
uint8_t velocidade = 0;
uint8_t raw_afr = 147;
uint8_t battery10 = 120;
uint8_t engine_load = 0;
uint8_t ve = 0;
uint8_t afr_target = 147;
int8_t adv_ignition = 0;

// ======================================================
// TIMERS
// ======================================================
unsigned long lastSpeeduinoRequest = 0;
const unsigned long speeduinoInterval = 80;

// ======================================================
// CAN ANTI ECHO
// ======================================================
unsigned long lastSentTime = 0;
unsigned long lastSentId = 0;

// ======================================================
// SETUP
// ======================================================
void setup() {
  Serial.begin(115200);
  speeduinoSerial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println(F("===================================="));
  Serial.println(F(" SPEEDUINO -> OBD2 29BIT "));
  Serial.println(F(" Stable CAN Gateway "));
  Serial.println(F("===================================="));

  if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println(F("MCP2515 OK"));
  } else {
    Serial.println(F("MCP2515 ERROR"));
    while (1);
  }

  CAN.setMode(MCP_NORMAL);
  Serial.println(F("CAN NORMAL MODE"));
}

// ======================================================
// LOOP
// ======================================================
void loop() {
  lerSpeeduino();
  receberOBD();
}

// ======================================================
// LEITURA SPEEDUINO
// ======================================================
void lerSpeeduino() {
  if ((millis() - lastSpeeduinoRequest) < speeduinoInterval) {
    return;
  }
  lastSpeeduinoRequest = millis();

  while (speeduinoSerial.available()) {
    speeduinoSerial.read();
  }

  speeduinoSerial.write('A');

  uint8_t sBuf[80];
  uint8_t bytesRead = 0;
  unsigned long timeout = millis();

  while ((millis() - timeout) < 40) {
    while (speeduinoSerial.available()) {
      if (bytesRead < sizeof(sBuf)) {
        sBuf[bytesRead++] = speeduinoSerial.read();
      }
    }
  }

  if (bytesRead < 25) {
    return;
  }

  map_pressure = (sBuf[5] << 8) | sBuf[4];
  temperatura_iat = sBuf[6];
  temperatura_ect = sBuf[7];
  battery10 = sBuf[9];
  raw_afr = sBuf[10];
  rpm = (sBuf[15] << 8) | sBuf[14];
  engine_load = sBuf[17];
  ve = sBuf[18];
  afr_target = sBuf[19];
  adv_ignition = (int8_t)sBuf[23];
  throttle = sBuf[24];
  velocidade = 0;
}

// ======================================================
// RECEIVE OBD REQUESTS
// ======================================================
void receberOBD() {
  if (CAN_MSGAVAIL != CAN.checkReceive()) {
    return;
  }

  unsigned long canId;
  unsigned char len = 0;
  unsigned char buf[8];

  CAN.readMsgBuf(&canId, &len, buf);

  if (!(canId & 0x80000000)) {
    return;
  }
  canId &= 0x1FFFFFFF;

  if ((canId == lastSentId) && ((millis() - lastSentTime) < 50)) {
    return;
  }

  if (canId != OBD_REQ_ID) {
    return;
  }
  if (len < 3) {
    return;
  }

  byte modo = buf[1];
  byte pid = buf[2];

  responderPID(modo, pid);
}

// ======================================================
// RESPONDE PID
// ======================================================
void responderPID(byte modo, byte pid) {
  byte resp[8] = {0};
  byte dataLen = 0;

  if (modo != 0x01) {
    return;
  }

  switch (pid) {

    // ==================================================
    // SUPPORTED PIDS 00-20
    // ==================================================
    case 0x00:
      resp[0] = 0x06;
      resp[1] = 0x41;
      resp[2] = 0x00;
      // 01 04 05 0B 0C 0D 0E 0F 11 14 1C 1F 20
      resp[3] = 0x98;
      resp[4] = 0x3E;
      resp[5] = 0x90;
      resp[6] = 0x13; 
      dataLen = 7;
      break;

    case 0x01:
      resp[0] = 0x06; resp[1] = 0x41; resp[2] = 0x01;
      resp[3] = 0x00; resp[4] = 0x07; resp[5] = 0x65; resp[6] = 0x04;
      dataLen = 7;
      break;

    // ENGINE LOAD (Byte 17 da Speeduino)
    case 0x04:
      resp[0] = 0x03; resp[1] = 0x41; resp[2] = 0x04;
      resp[3] = (uint8_t)((engine_load * 255) / 100);
      dataLen = 4;
      break;

    case 0x05:
      resp[0] = 0x03; resp[1] = 0x41; resp[2] = 0x05;
      resp[3] = temperatura_ect;
      dataLen = 4;
      break;

    case 0x0B:
      resp[0] = 0x03; resp[1] = 0x41; resp[2] = 0x0B;
      resp[3] = constrain(map_pressure, 0, 255);
      dataLen = 4;
      break;

    case 0x0C: {
        uint16_t obdRPM = rpm * 4;
        resp[0] = 0x04; resp[1] = 0x41; resp[2] = 0x0C;
        resp[3] = highByte(obdRPM); resp[4] = lowByte(obdRPM);
        dataLen = 5;
      }
      break;

    case 0x0D:
      resp[0] = 0x03; resp[1] = 0x41; resp[2] = 0x0D;
      resp[3] = velocidade;
      dataLen = 4;
      break;

    case 0x0E: {
        int8_t adv = constrain(adv_ignition, -64, 63);
        resp[0] = 0x03; resp[1] = 0x41; resp[2] = 0x0E;
        resp[3] = (adv * 2) + 128;
        dataLen = 4;
      }
      break;

    case 0x0F:
      resp[0] = 0x03; resp[1] = 0x41; resp[2] = 0x0F;
      resp[3] = temperatura_iat;
      dataLen = 4;
      break;

    case 0x11:
      resp[0] = 0x03; resp[1] = 0x41; resp[2] = 0x11;
      resp[3] = (uint8_t)((throttle * 255) / 100);
      dataLen = 4;
      break;

    case 0x14: {
        float afr = raw_afr / 10.0;
        float lambda = constrain((afr / 14.7), 0.5, 1.5);
        uint16_t obdLambda = (uint16_t)(lambda * 32768.0);
        uint8_t voltage = 128;
        resp[0] = 0x06; resp[1] = 0x41; resp[2] = 0x14;
        resp[3] = highByte(obdLambda); resp[4] = lowByte(obdLambda);
        resp[5] = voltage; resp[6] = 0x00;
        dataLen = 7;
      }
      break;

    case 0x1C:
      resp[0] = 0x03; resp[1] = 0x41; resp[2] = 0x1C;
      resp[3] = 0x01;
      dataLen = 4;
      break;

    // ENGINE RUN TIME
    case 0x1F: {
        uint16_t runTime = millis() / 1000;
        resp[0] = 0x04; resp[1] = 0x41; resp[2] = 0x1F;
        resp[3] = highByte(runTime); resp[4] = lowByte(runTime);
        dataLen = 5;
      }
      break;

    // ==================================================
    // SUPPORTED PIDS 21-40
    // ==================================================
    case 0x20:
      resp[0] = 0x06; resp[1] = 0x41; resp[2] = 0x20;
      resp[3] = 0x00; resp[4] = 0x00;
      resp[5] = 0x10; // Anuncia 0x34
      resp[6] = 0x01; // Anuncia bloco 0x40
      dataLen = 7;
      break;

    case 0x34: {
        float afr = raw_afr / 10.0;
        float lambda = constrain((afr / 14.7), 0.5, 1.5);
        uint16_t obdLambda = (uint16_t)(lambda * 32768.0);
        
        resp[0] = 0x06; resp[1] = 0x41; resp[2] = 0x34;
        resp[3] = highByte(obdLambda);
        resp[4] = lowByte(obdLambda);
        resp[5] = 0x80;
        resp[6] = 0x00;
        dataLen = 7;
      }
      break;

    // ==================================================
    // SUPPORTED PIDS 41-60
    // ==================================================
    case 0x40:
      resp[0] = 0x06; resp[1] = 0x41; resp[2] = 0x40;
      resp[3] = 0x60; // Anuncia 0x42 (Bateria) e 0x43 (Absolute Load/VE)
      resp[4] = 0x00; resp[5] = 0x00;
      resp[6] = 0x08; // Anuncia 0x5D
      dataLen = 7;
      break;

    // BATTERY VOLTAGE
    case 0x42: {
        uint16_t obdBat = battery10 * 100;
        resp[0] = 0x04; resp[1] = 0x41; resp[2] = 0x42;
        resp[3] = highByte(obdBat); resp[4] = lowByte(obdBat);
        dataLen = 5;
      }
      break;

    // ==================================================
    // ABSOLUTE LOAD VALUE (EXIBE A VE DA SPEEDUINO EM %)
    // ==================================================
    case 0x43: {
        // A fórmula do scanner é: (A*256 + B) * 100 / 255
        // Revertendo, transformamos a variável VE em A e B
        uint16_t absLoad = (ve * 255) / 100;
        resp[0] = 0x04; resp[1] = 0x41; resp[2] = 0x43;
        resp[3] = highByte(absLoad);
        resp[4] = lowByte(absLoad);
        dataLen = 5;
      }
      break;

    // FUEL INJECTION TIMING
    case 0x5D: {
        resp[0] = 0x04; resp[1] = 0x41; resp[2] = 0x5D;
        resp[3] = 0x69; // Resulta em 0.0 graus 
        resp[4] = 0x00;
        dataLen = 5;
      }
      break;

    // ==================================================
    // UNSUPPORTED PID
    // ==================================================
    default:
      resp[0] = 0x03; resp[1] = 0x7F; resp[2] = modo; resp[3] = 0x12;
      dataLen = 4;
      break;
  }

  // ====================================================
  // SEND RESPONSE
  // ====================================================
  lastSentTime = millis();
  lastSentId = OBD_RESP_ID;

  CAN.sendMsgBuf(OBD_RESP_ID, 1, dataLen, resp);
}
