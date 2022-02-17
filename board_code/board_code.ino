#include <Adafruit_BMP085.h>
#include <DHT.h>;
#include <SoftwareSerial.h>
#define DHTPIN 11
#define LED_PIN 10
#define M0 9
#define M1 8       
#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE); 



Adafruit_BMP085 bmp;
SoftwareSerial loraSerial(3, 2); // TX, RX
int hour = 3600*1000;
int h_counter = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  sleep_lora();
  Serial.begin(9600);
  
  loraSerial.begin(9600);
}

void loop() {
  if(h_counter == 15 || h_counter == 0) digitalWrite(LED_PIN, HIGH);   
  delay(500);              
  if(h_counter == 15 || h_counter == 0) digitalWrite(LED_PIN, LOW);    
  delay(500);
  
  
  if(h_counter == 3600 || h_counter == 0){ 

    bmp.begin();
    dht.begin();
   
    String lora_msg_a = float_to_string(dht.readHumidity());
    lora_msg_a.concat(";");
    lora_msg_a.concat(float_to_string(dht.readTemperature()));
    lora_msg_a.concat(";");
    lora_msg_a.concat(bmp.readTemperature());
    lora_msg_a.concat(";");
    lora_msg_a.concat(bmp.readPressure());

    wakeup_lora();
    delay(500);
    loraSerial.print(lora_msg_a);
    transmit_led_status();
    Serial.println(lora_msg_a);
    delay(500);
    sleep_lora();
    if(h_counter != 0) h_counter = 1;
  }              
  h_counter++;
}

void sleep_lora(){
  digitalWrite(M0, HIGH);
  digitalWrite(M1, HIGH);
}

void wakeup_lora(){
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
}

void transmit_led_status(){

  for(int i=0; i<10; i++){
    digitalWrite(LED_PIN, HIGH);   
    delay(20);
    digitalWrite(LED_PIN, LOW);   
    delay(20);                  
  }
  
}

String float_to_string(float source){
  char buffer[5];
  return dtostrf(source, 1, 4, buffer);
}
