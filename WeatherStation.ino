int pin = 13;
volatile int state = LOW;

void setup()
{
  Serial.begin(9600);
  pinMode(pin, OUTPUT);
  attachInterrupt(0, blink, FALLING);
}

void loop()
{
  digitalWrite(pin, state);
}

void blink()
{
  state = !state;
  Serial.println("Click");
}

