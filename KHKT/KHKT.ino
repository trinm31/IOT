#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiManager.h>
#include "DHTesp.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>          
#include <SimpleTimer.h>
#define dht_dpin 16
#define TRIG_PIN 13
#define ECHO_PIN 12
#define TIME_OUT 5000
#define sensor A0
int val;          
WidgetLED ledrain(V3);
WidgetLED leddry(V4);

DHTesp dht;
LiquidCrystal_I2C lcd(0x27, 16, 2);
WidgetMap myMap(V1);
SimpleTimer timer;
char auth[] = "DST0SnRHQFMW6_j8hVSgH6lzMpfWA7_Q";
char ssid[] = "";
char pass[] = "";
// 192.168.4.1
float t;
float h;

float GetDistance()
{
  long duration, distanceCm;
   
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH, TIME_OUT);
 
  // convert to distance
  distanceCm = duration / 29.1 / 2;
  
  return distanceCm;
}

void sendUptime()
{
  float h = dht.getHumidity();
  float t = dht.getTemperature();
  Serial.println("Humidity and temperature\n\n");
  Serial.print("Current humidity = ");
  Serial.print(h);
  Serial.print("%  ");
  Serial.print("temperature = ");
  Serial.print(t);
  Blynk.virtualWrite(V6, t);
  Blynk.virtualWrite(V5, h);
}

void setup()
{
    Serial.begin(9600);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(sensor,INPUT);
    WiFi.begin(ssid, pass);
    WiFiManager wifiManager;
    //wifiManager.resetSettings();
    //delay(500);
    wifiManager.autoConnect("BLYNK_Flood_Monitoring"); 
    Serial.println("connected...WI-FI)");
    Blynk.begin(auth, ssid, pass);
    dht.setup(dht_dpin, DHTesp::DHT11);
    Wire.begin(2, 0);
    lcd.begin();
    lcd.backlight(); 
    lcd.home (); 
    lcd.blink();
    lcd.home ();
    lcd.print("T: "); 
    lcd.setCursor (6,0); 
    lcd.print("H: ");
    lcd.setCursor (0,1); 
    lcd.print("IsFlood:");
    timer.setInterval(1000, sendUptime);
}

void loop()
{
  float h = dht.getHumidity();
  float t = dht.getTemperature();

  // rain
  int val = analogRead(sensor); //  Đọc giá trị từ cảm biến mưa và lưu vào biến val.
    /*Khi có mưa (biến val < 600) đông cơ servo ở vị trí 90 độ (cửa đóng)
      Khi không có mưa (val>600) động cơ servo ở vị trí 0 độ (cửa mở.*/
   if ( val < 700 )
   {
        
       Serial.println(" - It's wet");
       Blynk.virtualWrite(V8, 1);
       ledrain.on();
       leddry.off();
   } else {
       Serial.println(" - It's dry");
       Blynk.virtualWrite(V8, 0);
       leddry.on();
       ledrain.off();
   }
  delay(200);
  Serial.println();
  delay(100);

  // end rain 

  // distance
  long distance = GetDistance();
 
  if (distance <= 0)
  {
    Serial.println("Echo time out !!");
  }
  else
  {   
    Serial.print("Distance to nearest obstacle (cm): ");
    Serial.println(distance);
    if (distance >= 6){
      lcd.setCursor (10, 1); 
      lcd.print("true");
      Blynk.notify("Phan Chau Trinh Stress is flooding");
    }
    else{
      lcd.setCursor (10, 1); 
      lcd.print("fault");
    }
    Blynk.virtualWrite(V7, distance);
  }
  int index = 0;
  float lat = 15.57356;
  float lon = 108.48251;
  String value = String(distance);
  value += " cm";
  myMap.location(index, lat, lon, value);
  Blynk.virtualWrite(V9, distance);
  delay(1000);
  // End distance
  lcd.setCursor (3, 0);
  lcd.print(t,0);
  lcd.setCursor (9, 0); 
  lcd.print(h,0);
  Blynk.run();
  timer.run();
  
}
