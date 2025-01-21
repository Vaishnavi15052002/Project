#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3rdWtMKQy"
#define BLYNK_TEMPLATE_NAME "bus tracking"
#define BLYNK_AUTH_TOKEN "ZdJlbZ4BDuPz9ACcwqX7V8HCArbh89Q0"

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Pixel 6a";
char pass[] = "raghavendra";

String data;
String data1;
String data2;
String data3;
String data4;
String data5;
String data6;
String data7;
String data8;

int t;
int vibration_r;
int x_r; 
int y_r;
int Smoke_Sensor_result;
int psi;
double latitude;
double longitude;

void setup()
{
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
}

void loop()
{
  if (Serial.available() > 0)
  {
    data = Serial.readStringUntil('\n');
    Serial.println(data);
    if (data != "\n")
    {
      data1 = getValue(data, ',', 0);
      data2 = getValue(data, ',', 1);
      data3 = getValue(data, ',', 2);
      data4 = getValue(data, ',', 3);
      data5 = getValue(data, ',', 4);
      data6 = getValue(data, ',', 5);
      data7 = getValue(data, ',', 6);
      data8 = getValue(data, ',', 7);

      t=data1.toInt();
      vibration_r=data2.toInt();
      x_r=data3.toInt();
      Smoke_Sensor_result=data4.toInt();
      y_r=data5.toInt();
      psi=data6.toInt();
      latitude=data7.toInt();
      longitude=data8.toInt();

      Blynk.virtualWrite(V0,t);
      Blynk.virtualWrite(V1,psi);
      Blynk.virtualWrite(V2,Smoke_Sensor_result);
      Serial.print(t);
      Serial.print(",");
      Serial.print(vibration_r);
      Serial.print(",");
      Serial.print(x_r);
      Serial.print(",");
      Serial.print(Smoke_Sensor_result);
      Serial.print(",");
      Serial.print(x_r);
      Serial.print(",");
      Serial.print(psi);
      Serial.print(",");
      Serial.print(latitude);
      Serial.print(",");
      Serial.println(longitude);
      
      if (psi!=0)//psi<30
      {
        Blynk.logEvent("tyer_pressure", "LOW Tyer pressure detected");
        delay(100);
        Serial.println("tyer");
        Blynk.logEvent("google_map", "http://maps.google.com/maps?q=" + String(latitude) + "," + String(longitude));
        delay(100);
      }

      if (Smoke_Sensor_result>200)
      {
        Blynk.logEvent("smoke_detection" , "Smoke detected");
        delay(100);
        Blynk.logEvent("google_map", "http://maps.google.com/maps?q=" + String(latitude) + "," + String(longitude));
        delay(100);
      }

      if ((vibration_r==0)||(x_r>500)||(y_r>400))
      {
        Blynk.logEvent("accident_detected", "Accident detected");
        delay(100);
        Blynk.logEvent("google_map", "http://maps.google.com/maps?q=" + String(latitude) + "," + String(longitude));
        delay(100);
      }

    }
  }

  Blynk.run();
  data = " ";
}

String getValue(String data, char seperator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == seperator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";

}
