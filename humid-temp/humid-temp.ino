#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#define DHTPIN 0

#define DHTTYPE DHT11 // DHT 11

// Set the LCD address to 0x27 for a 16 chars and 2 line display


DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); // 0x27 is the I2C bus address for an unmodified backpack
void setup()
{
  Serial.begin(115200);
  // activate LCD module
  lcd.begin (); // for 16 x 2 LCD module
  lcd.home (); // set cursor to 0,0
  lcd.display();
  lcd.blink();
}

void loop()
{

  delay(1000);
  float h = dht.readHumidity();

  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  Serial.print(h);

  Serial.print(t);
  lcd.print("temp:");
  lcd.setCursor (0, 1); // go to start of 2nd line
  lcd.print("humid:");
  lcd.setCursor (11, 0);
  lcd.print(t);
  lcd.setCursor (9, 1); // go to start of 2nd line
  lcd.print(h);

}
