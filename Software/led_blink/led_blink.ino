#define FLASH_RATE_HZ 2
#define BUTTON_IN 2
#define PWM_LED_OUT 3
#define PWM_MAX 255

// operating mode: 0 - off, 1 - bright, 2 - mid-bright, 3 - dim, 4 - flash
int operatingMode = 0;
volatile boolean buttonIsPushed = false;
int previousButtonState = 0;
int outputPWM = 0;

unsigned long timeNewKeyPress = 0;
unsigned long timeLastKeyPress = 0;
unsigned int timeDebounce = 10;

void setup() 
{
   attachInterrupt(digitalPinToInterrupt(BUTTON_IN), ButtonPushed, RISING);
   pinMode(BUTTON_IN, INPUT);
   pinMode(PWM_LED_OUT,OUTPUT);
}

void loop()
{
    Serial.begin(9600);
    CheckForButtonPress();
}


void ButtonPushed() 
{
      buttonIsPushed = true;
}

void CheckForButtonPress() 
{
  if (buttonIsPushed == true) 
  {
        timeLastKeyPress = millis();
    Serial.print(timeLastKeyPress);
    Serial.print('\n');

    delay(1000);
    Serial.print(timeLastKeyPress);
    Serial.print('\n');

    delay(1000);
    if( timeLastKeyPress -timeLastKeyPress >= timeDebounce)
    {
      operatingMode = operatingMode + 1;
      if (operatingMode == 5) 
      {
        operatingMode = 0;
      }
      timeLastKeyPress = timeNewKeyPress;
      SetPWMBasedOnOperatingMode();
          buttonIsPushed = false;

    }
 }
}

void   SetPWMBasedOnOperatingMode()
 {

    switch (operatingMode) {
        case 0:
            outputPWM = 0;
            ShineLed();

        case 1:
            outputPWM = PWM_MAX;
            ShineLed();

        case 2:
            outputPWM = int(PWM_MAX/2);
            ShineLed();

        case 3:
            outputPWM = int(PWM_MAX/4);
            ShineLed();

        case 4:
            FlashTheLight();
    }

}


void FlashTheLight() 
{
  outputPWM = PWM_MAX;
  delay(1000);
  
  outputPWM = PWM_MAX;
  delay(1000);
}

void ShineLed() 
{
  Serial.print(operatingMode);
  Serial.print('\n');

  delay(100);
  analogWrite(PWM_LED_OUT, outputPWM);
}
