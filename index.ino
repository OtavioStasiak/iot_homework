#include Arduino.h
#include WiFi.h
#include HTTPClient.h


#define entradaDigital 52
#define entradaAnalogica 36
#define redOutput 14
#define greenOutput 27

const char ssid = HOME-2G;
const char password = 5152550house;
const char serverName = ""

bool isEmergency = false;
unsigned long startTime = 0; 
const unsigned long timeout = 120000;
bool dSensor;
int aSensor;

void setup() {
  Serial.begin(9600);
  
   Conectar ao Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print(Conectando ao Wi-Fi);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(.);
  }

  Serial.println();
  Serial.println(Conectado ao Wi-Fi);
  pinMode(entradaDigital, INPUT);
  pinMode(greenOutput, OUTPUT);
  pinMode(redOutput, OUTPUT);
}

void loop() {
  dSensor = digitalRead(entradaDigital);
  aSensor = analogRead(entradaAnalogica);
  
  if (aSensor  400 && !isEmergency) {
    digitalWrite(redOutput, HIGH);
    digitalWrite(greenOutput, LOW);
    
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      http.begin(serverName);

      int httpResponseCode = http.GET();

      if (httpResponseCode  0) {
        String response = http.getString();
        Serial.println(httpResponseCode); 
        startTime = millis();
        isEmergency = true;
      } else {
        Serial.print(Erro na requisição HTTP );
        Serial.println(httpResponseCode);  
      }

      http.end();
    } else {
      Serial.println(Wi-Fi desconectado);
    }

  } else {
    digitalWrite(greenOutput, HIGH);
    digitalWrite(redOutput, LOW);
  }

   if (millis() - startTime = timeout) {
      isEmergency = false; 
      Serial.println(Timeout atingido. Emergência desativada.);
   }

  
  delay(1000);
}
