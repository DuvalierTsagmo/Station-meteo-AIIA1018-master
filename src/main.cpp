/*
  Titre      : prediction de pluie
  Auteur     : Duvalier Tsagmo
  Date       : 22/03/2023
  Description: predire la pluie et la temperature de l'humiditer
  Version    : 0.0.1
*/

// bibiotheque utiliser
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_NeoPixel.h>
#include "DFRobot_RGBLCD1602.h"
#include <Wire.h>

char dtaUart[15];
char dtaLen = 0;
DFRobot_RGBLCD1602 lcd(/*lcdCols*/ 16, /*lcdRows*/ 2); // 16 characters and 2 lines of show

Adafruit_AHTX0 aht;

int RainSensorPin = A0;
int light = A2;
int k_pin = A1;
int Rain;

#define PIN_LED 4 // Control signal, connect to DI of the LED
#define NUM_LED 1 // Number of LEDs in a strip

// FGonction pour la LED RGB
void colorWipe(uint32_t c, uint16_t wait);

// Declaration d'objet
Adafruit_NeoPixel RGB_Strip = Adafruit_NeoPixel(NUM_LED, PIN_LED, NEO_GRB + NEO_KHZ800);
int k;
double h, t;

void setup()
{

    Serial.begin(9600);
    delay(10);

    if (aht.begin())
    {
        Serial.println("Found AHT20");
    }
    else
    {
        Serial.println("Didn't find AHT20");
    }

    lcd.init();
    lcd.setCursor(2, 0);
    lcd.print("Duvalier Station ");
    lcd.setCursor(6, 1);
    lcd.print("IDO");
    delay(3000);
    lcd.clear();
    lcd.print("Debut");
    delay(2000);
    // Start RGB
    RGB_Strip.begin();
    RGB_Strip.show();
    RGB_Strip.setBrightness(255); // Set brightness, 0-255 (darkest - brightest)
}

void loop()
{

    lcd.setRGB(255, 255, 255);

    sensors_event_t humidity, temp; // declaration d'objets
    aht.getEvent(&humidity, &temp);
    h = humidity.relative_humidity;   // capter l'humidite
    t = temp.temperature;             // capter la temperature
    k = analogRead(k_pin);            // capter la lumiere
    Rain = analogRead(RainSensorPin); // capter la pluie

    // Affichage des donnees
    Serial.println(temp.temperature);
    Serial.println(humidity.relative_humidity);
    Serial.println(k, DEC);
    Serial.println(Rain);

    // Formule trouvée par modélisation
    double ProbablityRain = 0.03743225 * h - 2.21389335 * t - 0.09773871 * k + 148.85713381876218;

    Serial.println(ProbablityRain);
    // If-clauses - Alertes sur les LEDs Blue, Orange, Red
    if (ProbablityRain > 60)
    {
        colorWipe(RGB_Strip.Color(255, 0, 0), 1000); // Red
        Serial.println("color red");
    }
    else if (ProbablityRain > 50)
    {
        colorWipe(RGB_Strip.Color(0, 0, 255), 1000); // Red
        Serial.println("color blue");
    }
    else
    {
        colorWipe(RGB_Strip.Color(0, 255, 0), 1000); // Red
        Serial.println("color green");
    }

    lcd.scrollDisplayLeft();
    lcd.setCursor(0, 0);
    lcd.print("Tmp: ");
    lcd.print(temp.temperature);
    lcd.print(" C Hum: ");
    lcd.print(humidity.relative_humidity);
    lcd.print(" % sol: ");
    lcd.print(k);
    lcd.print(" LUX  Rain: ");
    lcd.print(Rain);
    lcd.print(" mm");
    lcd.setCursor(0, 1);
    lcd.print("The Probablity of Rain is : ");
    lcd.print(ProbablityRain);
    lcd.print("%");
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint16_t wait)
{
    for (uint16_t i = 0; i < RGB_Strip.numPixels(); i++)
    {
        RGB_Strip.setPixelColor(i, c);
        RGB_Strip.show();
        delay(wait);
    }
}
