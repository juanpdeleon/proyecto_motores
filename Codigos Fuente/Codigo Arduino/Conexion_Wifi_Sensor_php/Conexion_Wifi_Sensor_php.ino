#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureAxTLS.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureAxTLS.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>

#include <Wire.h>
 
//Direccion I2C de la IMU
#define MPU 0x68
 
//Ratios de conversion
#define A_R 16384.0 // 32768/2
#define G_R 131.0 // 32768/250
 
//Conversion de radianes a grados 180/PI
#define RAD_A_DEG = 57.295779
 
//MPU-6050 da los valores en enteros de 16 bits
//Valores RAW
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
int outputpin= A0; 
//Angulos
float Acc[2];
float Gy[3];
float Angle[3];

String valores;

long tiempo_prev;
float dt;
const char* ssid = "Montepaz_Visitas";
const char* password = "montepazabc*+";
int ModbusTCP_port = 502;
 
//////// Required for Modbus TCP / IP /// Requerido para Modbus TCP/IP /////////
#define maxInputRegister 20
#define maxHoldingRegister 20
 
#define MB_FC_NONE 0
#define MB_FC_READ_REGISTERS 3 //implemented
#define MB_FC_WRITE_REGISTER 6 //implemented
#define MB_FC_WRITE_MULTIPLE_REGISTERS 16 //implemented
//
// MODBUS Error Codes
//
#define MB_EC_NONE 0
#define MB_EC_ILLEGAL_FUNCTION 1
#define MB_EC_ILLEGAL_DATA_ADDRESS 2
#define MB_EC_ILLEGAL_DATA_VALUE 3
#define MB_EC_SLAVE_DEVICE_FAILURE 4
//
// MODBUS MBAP offsets
//
#define MB_TCP_TID 0
#define MB_TCP_PID 2
#define MB_TCP_LEN 4
#define MB_TCP_UID 6
#define MB_TCP_FUNC 7
#define MB_TCP_REGISTER_START 8
#define MB_TCP_REGISTER_NUMBER 10
 
byte ByteArray[260];
unsigned int MBHoldingRegister[maxHoldingRegister];
 
//////////////////////////////////////////////////////////////////////////
 
WiFiServer MBServer(ModbusTCP_port);
 
void setup() {
 
 pinMode(14, OUTPUT);
 
 Serial.begin(9600);
 delay(100) ;
 WiFi.begin(ssid, password);
 IPAddress ip(192,168,11,133);   
 IPAddress gateway(192,168,11,1);   
 IPAddress subnet(255,255,255,0);   
 WiFi.config(ip, gateway, subnet);
 delay(100) ;
 Serial.println(".");
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }
 MBServer.begin();
 Serial.println("Connected ");
 Serial.print("ESP8266 Slave Modbus TCP/IP ");
 Serial.print(WiFi.localIP());
 Serial.print(":");
 Serial.println(String(ModbusTCP_port));
 Serial.println("Modbus TCP/IP Online");
 Wire.begin(4,5); // D2(GPIO4)=SDA / D1(GPIO5)=SCL
 Wire.beginTransmission(MPU);
 Wire.write(0x6B);
 Wire.write(0);
 Wire.endTransmission(true);
}
 
 
void loop() {
 
 
 // Check if a client has connected // Modbus TCP/IP
 WiFiClient client = MBServer.available();
 if (!client) {
 return;
 }

   Wire.beginTransmission(MPU);
   Wire.write(0x3B); //Pedir el registro 0x3B - corresponde al AcX
   Wire.endTransmission(false);
   Wire.requestFrom(MPU,6,true);   //A partir del 0x3B, se piden 6 registros
   AcX=Wire.read()<<8|Wire.read(); //Cada valor ocupa 2 registros
   AcY=Wire.read()<<8|Wire.read();
   AcZ=Wire.read()<<8|Wire.read();
 
   int analogValue = analogRead(outputpin);
   float t1=((analogValue/1024.0) *3300)/10;
   
 boolean flagClientConnected = 0;
 byte byteFN = MB_FC_NONE;
 int Start;
 int WordDataLength;
 int ByteDataLength;
 int MessageLength;
 
 // Modbus TCP/IP
 while (client.connected()) {
 
 if(client.available())
 {
 flagClientConnected = 1;
 int i = 0;
 while(client.available())
 {
 ByteArray[i] = client.read();
 i++;
 }
 
 client.flush();

 MBHoldingRegister[0] = AcX;
 MBHoldingRegister[1] = AcY;
 MBHoldingRegister[2] = AcZ;
 MBHoldingRegister[3] = t1;
 
 //// rutine Modbus TCP
 byteFN = ByteArray[MB_TCP_FUNC];
 Start = word(ByteArray[MB_TCP_REGISTER_START],ByteArray[MB_TCP_REGISTER_START+1]);
 WordDataLength = word(ByteArray[MB_TCP_REGISTER_NUMBER],ByteArray[MB_TCP_REGISTER_NUMBER+1]);
 }
 
 // Handle request
 
 switch(byteFN) {
 case MB_FC_NONE:
 break;
 
 case MB_FC_READ_REGISTERS: // 03 Read Holding Registers
 ByteDataLength = WordDataLength * 2;
 ByteArray[5] = ByteDataLength + 3; //Number of bytes after this one.
 ByteArray[8] = ByteDataLength; //Number of bytes after this one (or number of bytes of data).
 for(int i = 0; i < WordDataLength; i++)
 {
 ByteArray[ 9 + i * 2] = highByte(MBHoldingRegister[Start + i]);
 ByteArray[10 + i * 2] = lowByte(MBHoldingRegister[Start + i]);
 }
 MessageLength = ByteDataLength + 9;
 client.write((const uint8_t *)ByteArray,MessageLength);
 
 byteFN = MB_FC_NONE;
 
 break;
 
 
 case MB_FC_WRITE_REGISTER: // 06 Write Holding Register
 MBHoldingRegister[Start] = word(ByteArray[MB_TCP_REGISTER_NUMBER],ByteArray[MB_TCP_REGISTER_NUMBER+1]);
 ByteArray[5] = 6; //Number of bytes after this one.
 MessageLength = 12;
 client.write((const uint8_t *)ByteArray,MessageLength);
 byteFN = MB_FC_NONE;
 break;
 
 case MB_FC_WRITE_MULTIPLE_REGISTERS: //16 Write Holding Registers
 ByteDataLength = WordDataLength * 2;
 ByteArray[5] = ByteDataLength + 3; //Number of bytes after this one.
 for(int i = 0; i < WordDataLength; i++)
 {
 MBHoldingRegister[Start + i] = word(ByteArray[ 13 + i * 2],ByteArray[14 + i * 2]);
 }
 MessageLength = 12;
 client.write((const uint8_t *)ByteArray,MessageLength); 
 byteFN = MB_FC_NONE;
 
 break;
 }
 }
 
 
 
 
}
