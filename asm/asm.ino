#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiManager.h>
#include "DHTesp.h"
//#include "MQ135.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>          
#include <SimpleTimer.h>
#define dht_dpin 16
#define anInput     A0                        //analog feed from MQ135
#define co2Zero     380                        //calibrated CO2 0 level
//#define PIN_MQ135 A0


//MQ135 mq135_sensor = MQ135(PIN_MQ135);
DHTesp dht;
LiquidCrystal_I2C lcd(0x27, 16, 2);
SimpleTimer timer;
char auth[] = "ExF_2AceFQXtpun8BfBRcRsQv0vGn0Ro";
char ssid[] = "";
char pass[] = "";
// 192.168.4.1
//char ssid[] = "TOTOLINK_A800R";
//char pass[] = "31082001";
//char ssid[] = "Greenwich-Student";
//char pass[] = "12345678";
float t;
float h;

void sendUptime()
{
  int co2now[10];                               //int array for co2 readings
  int co2raw = 0;                               //int for raw value of co2
  int co2ppm = 0;                               //int for calculated ppm
  int zzz = 0;                                  //int for averaging
  for (int x = 0;x<10;x++)  //samplpe co2 10x over 2 seconds
  {                   
    co2now[x]=analogRead(A0)*10;
    delay(200);
  }

  for (int x = 0;x<10;x++)  //add samples together
  {                     
    zzz=zzz + co2now[x];  
  }
  
  co2raw = zzz/10;                            //divide samples by 10
  co2ppm = co2raw - co2Zero;                 //get calculated ppm
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
  //float ppm = mq135_sensor.getPPM();
  //float correctedPPM = mq135_sensor.getCorrectedPPM(t, h);
  Serial.print("ppm = ");
  Serial.print(co2ppm);
  Blynk.virtualWrite(V0, co2ppm);
}

void setup()
{
    Serial.begin(9600);
    pinMode(anInput,INPUT);
    WiFi.begin(ssid, pass);
    WiFiManager wifiManager;
    //wifiManager.resetSettings();
    //delay(500);
    wifiManager.autoConnect("BLYNK_Air_quality"); 
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
    lcd.print("AirQuality:");
    timer.setInterval(1000, sendUptime);
}

void loop()
{
  int co2now[10];                               //int array for co2 readings
  int co2raw = 0;                               //int for raw value of co2
  int co2ppm = 0;                               //int for calculated ppm
  int zzz = 0;                                  //int for averaging
  for (int x = 0;x<10;x++)  //samplpe co2 10x over 2 seconds
  {                   
    co2now[x]=analogRead(A0)*10;
    delay(200);
  }

  for (int x = 0;x<10;x++)  //add samples together
  {                     
    zzz=zzz + co2now[x];  
  }
  
  co2raw = zzz/10;                            //divide samples by 10
  co2ppm = co2raw - co2Zero;
  float h = dht.getHumidity();
  float t = dht.getTemperature(); 
  lcd.setCursor (3, 0);
  lcd.print(t,0);
  lcd.setCursor (9, 0); 
  lcd.print(h,0);
  lcd.setCursor (11, 1); 
  lcd.print(co2ppm);
  Blynk.run();
  timer.run();
  
}
