#include <SoftwareSerial.h>

#include <BearSSLHelpers.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiType.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <ESP8266WiFi.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266HTTPClient.h>

SoftwareSerial loraSerial(4, 5); // TX, RX
WiFiClient client;
const char* ssid = "******";
const char* password = "******";
int LED_LORA = 15;
int LED_WIFI = 13;
int LED_INET = 12;
int result = -1;

int time_check = 270000;

int counter_lora = time_check;
int counter_wifi = time_check;
int counter_inet = time_check;

const char* host = "http://scicrop.com/trihum";
char urlout[255];
HTTPClient http;
int httpCode = 0; 

void setup() {
 
  pinMode(LED_LORA, OUTPUT);
  pinMode(LED_WIFI, OUTPUT);
  pinMode(LED_INET, OUTPUT);
  Serial.begin(9600);
  loraSerial.begin(9600);
  Serial.println("booting...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting wifi...");
  }
  Serial.println("Wifi Connected.");
  counter_wifi = 0;  
  result = loraSerial.available();
  Serial.println(result);


}

void loop() {
 
 if(result == 0){
    String input = loraSerial.readString();
    int input_len = input.length();
    int valid_input = -1;
    if(input_len > 3) valid_input = 0; 
    if(valid_input == 0){
      Serial.println(input); 
      counter_lora = 0;
      strcpy(urlout, host);
      strcat(urlout, "/?data=");
      char buffered_char[30];
      input.toCharArray(buffered_char, 50);
      
      strcat(urlout, buffered_char);
      if (WiFi.status() == WL_CONNECTED) { 
        counter_wifi = 0;
        http.begin(client, urlout); 
        httpCode = http.GET();
        Serial.println(urlout);
        Serial.println(httpCode);
        if (httpCode > 0) { 
          String payload = http.getString();  
          Serial.println(payload);
          counter_inet = 0;            
        }
        http.end();   
      }        
    }
    if(counter_lora <= time_check) digitalWrite(LED_LORA, HIGH);
    if(counter_wifi <= time_check) digitalWrite(LED_WIFI, HIGH);
    if(counter_inet <= time_check) digitalWrite(LED_INET, HIGH); 
    delay(10);
    if(counter_lora <= time_check) digitalWrite(LED_LORA, LOW);
    if(counter_wifi <= time_check) digitalWrite(LED_WIFI, LOW);
    if(counter_inet <= time_check) digitalWrite(LED_INET, LOW);
    delay(10);
    if(counter_lora > time_check){ digitalWrite(LED_LORA, HIGH); Serial.println("failed @ lora connection!"); }
    if(counter_wifi > time_check){ digitalWrite(LED_WIFI, HIGH); Serial.println("failed @ wifi connection!"); }
    if(counter_inet > time_check){ digitalWrite(LED_INET, HIGH); Serial.println("failed @ internet connection");} 
    counter_lora++;
    counter_wifi++;
    counter_inet++;
  }else{
    digitalWrite(LED_LORA, HIGH);
    Serial.println("failed @ loraSerial.available()");
    delay(2000); 
    digitalWrite(LED_LORA, LOW);
    delay(2000);
  }
}
