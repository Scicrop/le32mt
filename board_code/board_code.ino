#include <Adafruit_BMP085.h>
#include <DHT.h>;
#define DHTPIN 11     
#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE); 

int LED_PIN = 10;
float hum;  
float temp;
Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  bmp.begin();
  dht.begin();

}

void loop() {
  digitalWrite(LED_PIN, HIGH);   
  delay(1000);              
  digitalWrite(LED_PIN, LOW);    
  delay(1000);
  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");
  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");
  hum = dht.readHumidity();
  temp= dht.readTemperature();
  Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");              

}
