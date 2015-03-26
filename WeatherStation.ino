// Initialize global variables.
// Set digital pin 13 as the LED output (use on board LED).

#define ANEMOMETER_PIN 2
#define WIND_DIRECTION_PIN 0
#define CALCULATE_WINDSPEED_INTERVAL 5000
#define CALCULATE_WINDDIR_INTERVAL 5000
#define LED_PIN 13
//int ledPin = 13;
volatile int state = LOW;
// Create global variable to store the number of revolutions
// of the anemometer.
volatile int numRevsAnemometer = 0;
unsigned long nextWindSpeedCalc;
unsigned long nextWindDirCalc;
unsigned long time;

// Setup pins and variables.
void setup()
{
  // Set up serial port at 9600 baud for reporting status.
  Serial.begin(9600);
  // Set the LED pin as an output.
  pinMode(LED_PIN, OUTPUT);
  // Set anemometer pin as an input.
  // I don't know if this is necessary with attachInterrupt.
  pinMode(ANEMOMETER_PIN, INPUT);
  // Set internal pull-up resistor.
  digitalWrite(ANEMOMETER_PIN, HIGH);
  // Set up digital pin 2 to respond to hardware interrupts on
  // falling side of a signal. the 'blink' function is set as
  // the callback and runs on each trigger.
  attachInterrupt(0, countAnemometer, FALLING);
}

// Main loop for program.
void loop()
{
   time = millis();

   if (time >= nextWindSpeedCalc) {
      calculateWindSpeed();
      nextWindSpeedCalc = time + CALCULATE_WINDSPEED_INTERVAL;
   }
   if (time >= nextWindDirCalc) {
      calculateWindDir();
      nextWindDirCalc = time + CALCULATE_WINDDIR_INTERVAL;
   }
}

/**
 * Calculate wind speed.
 *
 * Turn on anemometer interrupt to count revolutions for a given period of time.
 * Use this count to calculate the wind speed.
 */
void calculateWindSpeed()
{
   long speed = 1492;
   int mph, kts;
   
//   Serial.println(numRevsAnemometer);
   speed *= numRevsAnemometer;
   mph = speed / CALCULATE_WINDSPEED_INTERVAL;
   kts = mph * .869;
   Serial.print("Wind speed: ");
   Serial.print(mph);
   Serial.print("mph / ");
   Serial.print(kts);
   Serial.println("kts");
 

   numRevsAnemometer = 0;        // Reset counter
}

/**
 * Determine wind direction.
 */
void calculateWindDir()
{
  int dirVal;
  dirVal = analogRead(WIND_DIRECTION_PIN);
  dirVal >>=2;
  
  Serial.print("WIND DIRECTION: ");
  Serial.println(dirVal);
}

// Callback for anemometer interrupt
void countAnemometer()
{
  // Increment the revolution counter.
  numRevsAnemometer++;
  // Set the global state variable to the opposite of what it was.
  state = !state;
  // Turn on or off LED based on what the global value of state is.
  digitalWrite(LED_PIN, state);
}
