
// Declare the libraries that are necessary for the LCD screen

#include 
#include 

// Declare a warmup time of 20 seconds for the MQ-3 sensor, which is necessary for good functioning of the sensor

int TIME_UNTIL_WARMUP = 20;
unsigned long time;

// Declare a time for each measurement; the time for which the user will have to blow on the sensor for the measurement

int TIME_UNTIL_MEASURE = 5;
unsigned long measurement_start;

// Declare the pin of the MQ-3 sensor

int analogPin = A0;
int val = 0;

// Declare the pin of the piezo buzzer

const int buzzerPin = 7;

// Pin of the push button:

const int buttonPin = 2;

// Initialise the I2C LCD instance:

LiquidCrystal_I2C lcd(0x27,20,4);

// In the setup(), initialise the LCD screen, and activate the backlight

lcd.init(); 
lcd.backlight();

// In the loop() part of the sketch, we check if the button was pressed. If so, reset the whole measurement process

int button_state = digitalRead(buttonPin);

if (button_state && !measurement_mode) {
  lcd.clear();
  measurement_mode = true;
  measurement_start = millis()/1000; 
  measurement_done = false;
}

// Keep track of the time with a counter:

time = millis()/1000;

// If still in the warmup process, we wait, and print this status on the LCD screen

if(time<=time_until_warmup) {=""  ="" int="" progress_time="map(time," 0,="" time_until_warmup,="" 100);="" printwarming(progress_time);="" }<="" pre="">

// Once warmup is done, wait until user presses the button to start the measurement process

else
{
  if (measurement_mode == false && !measurement_done) {
    
    // Instruction 
    printPress(); 
  }

  if (measurement_mode && !measurement_done) {

    // Instruction 
    printMeasure();

    // Sound
    tone(buzzerPin, 1000);

    // Read alcohol level
    val = readAlcohol();
  }
  
  if (measurement_mode && !measurement_done && ((time - measurement_start)> TIME_UNTIL_MEASURE)){

    noTone(buzzerPin);
    measurement_mode = false;
    measurement_done = true;
    lcd.clear();
  }

  if(measurement_done) {

    printAlcohol(val);
    printAlcoholLevel(val); 
  }
}