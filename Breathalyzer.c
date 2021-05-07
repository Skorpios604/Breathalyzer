#include <LiquidCrystal.h>

// LiquidCrystal I2C - Version: Latest 
#include <LiquidCrystal_I2C.h>

//library
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//warmup
int TIME_UNTIL_WARMUP = 20;
unsigned long time;

//measurement
int TIME_UNTIL_MEASURE = 5;
unsigned long measurement_start;

//sensor readings
int analogPin = A0;
int val = 0;

//buzzer
const int buzzerPin = 7;

//button
const int buttonPin = 2; 

//modes
bool measurement_mode;
bool measurement_done;

//LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); //set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  //serial
  Serial.begin(9600);
  
  //Init LCD
  lcd.init();
  lcd.backlight();
  
}

void loop()
{
  //read button state
  int button_state = digitalRead(buttonPin);
  
  if(button_state && !measurement_mode)
  {
    lcd.clear();
    measurement_mode = true;
    measurement_start = millis()/1000;
    measurement_done = false;
  }
  //wait
  delay(100);
  
  //get time
  time = millis()/1000;
  
  //warmup
  if(time<=TIME_UNTIL_WARMUP)
  {
    int progress_time = map(time, 0, TIME_UNTIL_WARMUP, 0, 100);
    printWarming(progress_time);
  }
  
  else
  {
    if (measurement_mode == false && !measurement_done)
    {
      //instruction
      printPress();
    }
    if (measurement_mode && !measurement_done)
    {
      //instruction
      printMeasure();
      
      //sound
      tone(buzzerPin, 1000);
      
      //read alcohol level
      val = readAlcohol();
    
    }
    
    if (measurement_mode && !measurement_done && ((time - measurement_start) > TIME_UNTIL_MEASURE))
    {
      noTone(buzzerPin);
      measurement_mode = false;
      measurement_done = true;
      lcd.clear();
    }
    
    if (measurement_done)
    {
      printAlcohol(val);
      printAlcoholLevel(val);
    } 
  }
  
  }
  
  void printWarming (int progress)
  {
    lcd.setCursor(0,0);
    lcd.print("Warming up: ");
    lcd.print(progress);
    lcd.print("%");
  }
  
  void printPress()
  {
    lcd.setCursor(0,0);
    lcd.print ("Press to start...");
  }
  
  void printMeasure()
  {
    lcd.setCursor(0,0);
    lcd.print("Breathe until the ");
    lcd.setCursor(0,1);
    lcd.print("sound stops...");
  }
  
  void printAlcohol(int value)
  {
    lcd.setCursor(0,0);
    lcd.print("Sensor reading: ");
    lcd.print(val);
  }
  
  void printAlcoholLevel (int value)
  {
    lcd.setCursor(0,1);
    if(value<200)
    {
      lcd.print("You are sober.");
    }
    if(value>=200 && value<280)
    {
      lcd.print("You had a beer.");
    }
    if(value>=280 && value<350)
    {
      lcd.print("Two or more beers.");
    }
    if(value>=350 && value <450)
    {
      lcd.print("Too much...");
    }
    if(value>450)
    {
      lcd.print("You are drunk!");
    }
  }
  
  int readAlcohol()
  {
    //number measurements
    int nb_measurements = 5;
    int measurements;
    
    //measure
    for(int i=0; i<nb_measurements; i++)
    {
      measurements = measurements + analogRead(analogPin);
    }
    measurements = measurements/nb_measurements;
  
    return measurements;
  }
  