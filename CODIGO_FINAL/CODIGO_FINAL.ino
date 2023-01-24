#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DFRobot_MAX30102.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h>

#include <WiFiClient.h>


Adafruit_SSD1306 oled(128, 64, &Wire);
DFRobot_MAX30102 pulseox;

// defino credenciales red
const char* ssid ="TAMA";
const char* password ="92010471653819";

WiFiClient client; 

void setup() {
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.clearDisplay();
  oled.display();
  oled.setTextSize(2);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0,0);
  oled.print("Inicializando...");
  oled.display();
  delay(2000);
  
  Serial.begin(115200);
  Serial.println(F("Prueba de conexión con el servidor"));

  pulseox.begin();
  pulseox.sensorConfiguration(/*ledBrightness=*/50, /*sampleAverage=*/SAMPLEAVG_4, \
                        /*ledMode=*/MODE_MULTILED, /*sampleRate=*/SAMPLERATE_100, \
                        /*pulseWidth=*/PULSEWIDTH_411, /*adcRange=*/ADCRANGE_16384);

                        
  WiFi.begin(ssid, password);
  Serial.print("Conectando...");
  while (WiFi.status()!= WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conexión OK!");
  Serial.print("IP Local: ");
  Serial.println(WiFi.localIP());




}


int32_t SPO2; //SPO2
int8_t SPO2Valid; //Flag to display if SPO2 calculation is valid
int32_t heartRate; //Heart-rate
int8_t heartRateValid; //Flag to display if heart-rate calculation is valid

void loop() {

  pulseox.heartrateAndOxygenSaturation(/**SPO2=*/&SPO2, /**SPO2Valid=*/&SPO2Valid, /**heartRate=*/&heartRate, /**heartRateValid=*/&heartRateValid);
  oled.clearDisplay();
  oled.setCursor(0,10);
  oled.print("BPM: ");
  oled.print(heartRate, DEC);
  oled.setCursor(0,40);
  oled.print("SpO2: ");
  oled.print(SPO2, DEC);
  oled.display();
  
  delay(2000);

  
  // funcion  de envio por POST
  EnvioDatos();

 
}



// rutina de envio de datos por POST
void EnvioDatos(){
  Serial.print("entro1");
  if (WiFi.status() == WL_CONNECTED){
    Serial.print("entro2");
    HTTPClient http;  // creo el objeto http
    String datos_a_enviar = "pulso= " + String(heartRate, DEC) + " &oxigenacion= " + String(SPO2, DEC);  

    Serial.print("entro3");
    http.begin(client,"http://proyectopulsiox.000webhostapp.com/EspPost.php");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // defino texto plano..
    Serial.print("entro4");

    int codigo_respuesta = http.POST(datos_a_enviar);
    Serial.println(codigo_respuesta);

    Serial.print("entro5");
    if (codigo_respuesta > 0){
      Serial.print("entro6");
      Serial.println("Código HTTP: "+ String(codigo_respuesta));
        if (codigo_respuesta == 200){
          String cuerpo_respuesta = http.getString();
          Serial.println("El servidor respondió: ");
          Serial.println(cuerpo_respuesta);
        }
     } else {
        Serial.print("Error enviado POST, código: ");
        Serial.println(codigo_respuesta);
     }

       http.end();  // libero recursos
       
  } else {
     Serial.println("Error en la conexion WIFI");
  }
  delay(2000); //espera 4s
}
