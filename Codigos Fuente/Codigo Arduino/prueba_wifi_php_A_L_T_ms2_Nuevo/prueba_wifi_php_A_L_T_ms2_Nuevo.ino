#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>

#define MPU 0x68
#define Rojo 2 // led de error
#define Verde 1 // led de funcionamiento
#define Azul 3
//#define Temp_pin 0 // Pin de temperatura


const char* ssid = "tesis-ap";
const char* password =  "Rodo377+385";

int16_t AcX, AcY, AcZ;
String mac;
int Temp_pin= 0;
int Temp_volt =0;
float Tmp;
float Tmp2;
unsigned long tiempo1, tiempo2;
float ArrayX[100];
float ArrayY[100];
float ArrayZ[100];
// Variables de seteo del filtro
float Y=0.0;
float alfa=0.5;
float Sx, Sy,Sz;


void setup() {
  // Pines de Status
  pinMode(Rojo,OUTPUT);
  pinMode(Verde,OUTPUT);
  pinMode(Azul,OUTPUT);
  delay(10);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  Serial.print("Conectando...");
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(500);
  }
 
  mac = WiFi.macAddress();
// Configuracion del MPU6050 y protocolo wire
  Wire.begin(4,5); // D2(GPIO4)=SDA / D1(GPIO5)=SCL
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
//

} // ************FIN SETUP

void loop() {
  digitalWrite(Verde,LOW);
  if(WiFi.status()== WL_CONNECTED){   //WiFi  status
     digitalWrite(Azul,LOW); // Indico que hay conexion wifi
     HTTPClient http;
    // Tomar datos del acelerometro******************************
    //  Serial.print(millis());     
        Wire.beginTransmission(MPU);
        Wire.write(0x3B); //Pedir el registro 0x3B - corresponde al AcX
        Wire.endTransmission(false);  
        Wire.requestFrom(MPU,6,true);   //A partir del 0x3B, se piden 6 registros
        AcX=Wire.read()<<8|Wire.read(); //Cada valor ocupa 2 registros
        AcY=Wire.read()<<8|Wire.read();
        AcZ=Wire.read()<<8|Wire.read();
        //Filtro Pasa bajo y 
        Sx=(((alfa*AcX)+((1-alfa)*Sx))*9.81/16384.0);
        Sy=(((alfa*AcY)+((1-alfa)*Sy))*9.81/16384.0);
        Sz=(((alfa*AcZ)+((1-alfa)*Sz))*9.81/16384.0);
          
       Temp_volt= analogRead(Temp_pin);
     //  Tmp = map(Temp_volt, 0, 1023,2, 150); //segun datasheet
     Serial.print(Temp_volt);
      Tmp = (Temp_volt/1023)*5000;
      Tmp = Tmp/10;
    //Concatenar para enviar al Servidor 
    String datos_a_enviar =  "mac="+mac+"&AcX="+Sx+"&AcY="+Sy+"&AcZ="+Sz+"&Tmp="+Tmp;
    Serial.println(datos_a_enviar);
    
    
    http.begin("http://10.1.1.1/sensores/entrada_datos.php");        //Indicamos el destino
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int codigo_respuesta = http.POST(datos_a_enviar);  //eNVIAR datos post

   if(codigo_respuesta>0){

     if(codigo_respuesta == 200){
          digitalWrite(Rojo,LOW);
    }else{
          digitalWrite(Rojo,HIGH);
    }
   
    }else{


       digitalWrite(Rojo,LOW); // Indico que hay error en el envio de http
       delay(500);

    }

    http.end();  //libero recursos

  }else{
       digitalWrite(Azul,HIGH); // Indico que NO hay conexion wifi
       delay(500);
    

  }

   delay(100);
}
