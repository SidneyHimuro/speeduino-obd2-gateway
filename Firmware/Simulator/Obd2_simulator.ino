#include <mcp_can.h>
#include <SPI.h>

// ======================================================
// SIMULADOR OBD2 CAN PARA LAUNCH CREDER 3001
// Arduino UNO + MCP2515
// CAN 500kbps (Cristal 8MHz)
// ======================================================

const int SPI_CS_PIN = 10;
MCP_CAN CAN(SPI_CS_PIN);

// ======================================================
// Variáveis simuladas
// ======================================================

int rpm = 1500;
int velocidade = 60;
int temperatura = 90;
int throttle = 25;

// Novos sensores
int cargaMotor = 30; // %
int mapPressure = 40; // kPa
int iatTemp = 35; // Celsius
float mafRate = 10.0; // g/s
int o2Voltage_mV = 400; // mV (Varia de 100 a 900mV)

int direcaoRPM = 1;

// Controle anti-eco
unsigned long lastSentTime = 0;
unsigned long lastSentId = 0;

// ======================================================

void setup() {
Serial.begin(115200);
delay(1000);

Serial.println();
Serial.println(F("===================================="));
Serial.println(F(" SIMULADOR OBD2 + SNIFFER CAN "));
Serial.println(F(" Launch Creader 3001 Compatible "));
Serial.println(F("===================================="));

// Inicializa MCP2515 com cristal de 8MHz
if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
Serial.println(F("MCP2515 inicializado OK"));
} else {
Serial.println(F("ERRO MCP2515"));
while (1);
}

CAN.setMode(MCP_NORMAL);
Serial.println(F("Modo NORMAL ativado\n"));
}

// ======================================================

void loop() {
atualizarSensores();
receberOBD();
}

// ======================================================
// GERA DADOS OSCILANTES
// ======================================================

void atualizarSensores() {
static unsigned long lastUpdate = 0;

if (millis() - lastUpdate > 100) {
lastUpdate = millis();

// Dinâmica de RPM e Velocidade    
rpm += (direcaoRPM * 100);    
if (rpm > 4500) direcaoRPM = -1;    
if (rpm < 900)  direcaoRPM = 1;    

velocidade = map(rpm, 900, 4500, 20, 140);    
temperatura = random(85, 98);    
throttle = map(rpm, 900, 4500, 10, 80);    

// Dinâmica dos novos sensores baseada no RPM    
cargaMotor = map(rpm, 900, 4500, 25, 95); // Carga de 25% a 95%    
mapPressure = map(rpm, 900, 4500, 30, 95); // Pressão sobe com a aceleração    
iatTemp = random(35, 40); // Ar de admissão varia pouco    
mafRate = map(rpm, 900, 4500, 40, 1200) / 10.0; // MAF varia de 4.0 a 120.0 g/s    
o2Voltage_mV = random(100, 900); // Sonda lambda "pula" entre mistura rica e pobre

}
}

// ======================================================
// RECEBE REQUISIÇÕES OBD2
// ======================================================

void receberOBD() {
if (CAN_MSGAVAIL == CAN.checkReceive()) {
unsigned long canId;
unsigned char len = 0;
unsigned char buf[8];

CAN.readMsgBuf(&canId, &len, buf);    

// Ignora eco    
if (canId == lastSentId && (millis() - lastSentTime) < 50) {    
  return;    
}    

// Requisição broadcast OBD2    
if (canId == 0x7DF && len >= 3) {    
  byte tamanho = buf[0];    
  byte modo    = buf[1];    
  byte pid     = buf[2];    

  Serial.print(F("REQ -> 0x"));    
  Serial.print(canId, HEX);    
  Serial.print(F(" | Modo: 0x"));    
  print2hex(modo);    
  Serial.print(F(" | PID: 0x"));    
  print2hex(pid);    
  Serial.println();    

  responderPID(modo, pid);    
}

}
}

// ======================================================
// RESPONDE PID
// ======================================================

void responderPID(byte modo, byte pid) {
byte resp[8] = {0};
int dataLen = 0;

if (modo == 0x01) {
switch (pid) {

// PIDs SUPORTADOS (Nova máscara ativando os novos PIDs)    
  case 0x00:    
    resp[0] = 0x06;    
    resp[1] = 0x41;    
    resp[2] = 0x00;    
    // PIDs ativos: 01, 04, 05, 0B, 0C, 0D, 0F, 10, 11, 14, 1C    
    resp[3] = 0x98;     
    resp[4] = 0x3F;     
    resp[5] = 0x90;     
    resp[6] = 0x08;     
    dataLen = 7;    
    break;    

  // STATUS    
  case 0x01:    
    resp[0] = 0x06;    
    resp[1] = 0x41;    
    resp[2] = 0x01;    
    resp[3] = 0x00;    
    resp[4] = 0x07;    
    resp[5] = 0x65;    
    resp[6] = 0x04;    
    dataLen = 7;    
    break;    

  // CARGA DO MOTOR    
  case 0x04:    
    resp[0] = 0x03;    
    resp[1] = 0x41;    
    resp[2] = 0x04;    
    resp[3] = map(cargaMotor, 0, 100, 0, 255);    
    dataLen = 4;    
    break;    

  // TEMPERATURA MOTOR (ECT)    
  case 0x05:    
    resp[0] = 0x03;    
    resp[1] = 0x41;    
    resp[2] = 0x05;    
    resp[3] = temperatura + 40;    
    dataLen = 4;    
    break;    

  // PRESSÃO DO COLETOR (MAP)    
  case 0x0B:    
    resp[0] = 0x03;    
    resp[1] = 0x41;    
    resp[2] = 0x0B;    
    resp[3] = mapPressure;    
    dataLen = 4;    
    break;    

  // RPM    
  case 0x0C: {    
    int valorRPM = rpm * 4;    
    resp[0] = 0x04;    
    resp[1] = 0x41;    
    resp[2] = 0x0C;    
    resp[3] = highByte(valorRPM);    
    resp[4] = lowByte(valorRPM);    
    dataLen = 5;    
    break;    
  }    

  // VELOCIDADE    
  case 0x0D:    
    resp[0] = 0x03;    
    resp[1] = 0x41;    
    resp[2] = 0x0D;    
    resp[3] = velocidade;    
    dataLen = 4;    
    break;    

  // TEMPERATURA DO AR (IAT)    
  case 0x0F:    
    resp[0] = 0x03;    
    resp[1] = 0x41;    
    resp[2] = 0x0F;    
    resp[3] = iatTemp + 40;    
    dataLen = 4;    
    break;    

  // FLUXO DE AR (MAF)    
  case 0x10: {    
    int valorMAF = mafRate * 100;    
    resp[0] = 0x04;    
    resp[1] = 0x41;    
    resp[2] = 0x10;    
    resp[3] = highByte(valorMAF);    
    resp[4] = lowByte(valorMAF);    
    dataLen = 5;    
    break;    
  }    

  // TPS    
  case 0x11:    
    resp[0] = 0x03;    
    resp[1] = 0x41;    
    resp[2] = 0x11;    
    resp[3] = map(throttle, 0, 100, 0, 255);    
    dataLen = 4;    
    break;    

  // SENSOR DE O2 (Voltagem + Fuel Trim)    
  case 0x14: {    
    int valorO2 = (o2Voltage_mV * 200) / 1000;    
    resp[0] = 0x04;    
    resp[1] = 0x41;    
    resp[2] = 0x14;    
    resp[3] = valorO2;     // Voltagem A    
    resp[4] = 128;         // Trim B (128 = 0%)    
    dataLen = 5;    
    break;    
  }    

  // PADRÃO OBD    
  case 0x1C:    
    resp[0] = 0x03;    
    resp[1] = 0x41;    
    resp[2] = 0x1C;    
    resp[3] = 0x01; // 0x01 = OBD-II (CARB)    
    dataLen = 4;    
    break;    

  // PID NÃO SUPORTADO    
  default:    
    resp[0] = 0x03;    
    resp[1] = 0x7F;    
    resp[2] = modo;    
    resp[3] = 0x12;    
    dataLen = 4;    
    break;    
}    

// ==========================================    
// ENVIA RESPOSTA    
// ==========================================    

lastSentTime = millis();    
lastSentId   = 0x7E8;    

if (CAN.sendMsgBuf(0x7E8, 0, dataLen, resp) == CAN_OK) {    
  Serial.print(F("TX -> 0x7E8 | "));    
  for (int i = 0; i < dataLen; i++) {    
    print2hex(resp[i]);    
    Serial.print(" ");    
  }    
  Serial.println();    
} else {    
  Serial.println(F("ERRO ENVIO CAN"));    
}    
Serial.println();

}
}

// ======================================================
// FUNÇÕES AUXILIARES
// ======================================================

void print2hex(byte valor) {
if (valor < 0x10) Serial.print("0");
Serial.print(valor, HEX);
}
