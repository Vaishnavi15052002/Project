#include <SoftwareSerial.h>
#include <Q2HX711.h>
#include <TinyGPS++.h>
#include <LiquidCrystal_I2C.h> 
#include "DHT.h"

#define DHTPIN 2   
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);
TinyGPSPlus gps;
LiquidCrystal_I2C lcd(0x27, 16, 2);

int Smoke_Sensor = A0;
int Smoke_Sensor_result;

int x = A1;
int y = A2;
int x_r; 
int y_r;

int vibration = A3;
int vibration_r;

int t;

double latitude;
double longitude;
String latitude_data;
String longitude_data;

const byte MPS_OUT_pin = 3; // OUT data pin
const byte MPS_SCK_pin = 4; // clock data pin
int avg_size = 10; // #pts to average over

int psi;
Q2HX711 MPS20N0040D(MPS_OUT_pin, MPS_SCK_pin); // start comm with the HX710B

void setup() 
{
  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.clear();         
  lcd.backlight();
  pinMode(vibration,INPUT);
}

void loop() 
{
  t = dht.readTemperature();
  
  vibration_r=digitalRead(vibration);

  Smoke_Sensor_result = analogRead(Smoke_Sensor); 
  x_r = analogRead(x);
  y_r = analogRead(y);

  while (Serial.available() > 0)
    if (gps.encode(Serial.read()))
    {
      if (gps.location.isValid())
      {
        double latitude = (gps.location.lat());
        double longitude = (gps.location.lng());
        latitude_data= (String(latitude, 6));
        longitude_data= (String(longitude, 6));
        Serial.println(latitude_data);
        Serial.println(longitude_data);
      }
    }
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("GPS Connection Error!!"));
    while (true);
  }

  float avg_val = 0.0; 
  for (int ii=0;ii<avg_size;ii++)
  {
    avg_val += MPS20N0040D.read(); 
    delay(50); 
  }
  avg_val /= avg_size;
  psi= (avg_val*5.0/1024/1000)-45;
  delay(100);
  if(psi<0)
  {
    psi=0;
  }

  Serial.print(t);
  Serial.print(",");
  Serial.print(vibration_r);
  Serial.print(",");
  Serial.print(x_r);
  Serial.print(",");
  Serial.print(y_r);
  Serial.print(",");
  Serial.print(Smoke_Sensor_result);
  Serial.print(",");
  Serial.print(psi);
  Serial.print(",");
  Serial.print(latitude_data);
  Serial.print(",");
  Serial.println(longitude_data);
  
  lcd.setCursor(0,0);
  lcd.print("Tmp:"+String(t));
  lcd.setCursor(0,1);
  lcd.print("PSI:"+String(psi));
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("LT:"+String(latitude_data));
  lcd.setCursor(0,1);
  lcd.print("LN:"+String(longitude_data));
  delay(1000);
  lcd.clear();
}
