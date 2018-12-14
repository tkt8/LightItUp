#define FLASH_RATE_HZ 2.0
#define BUTTON_IN 2
#define PWM_LED_OUT 11
#define PWM_MAX 255

// operating mode: 0 - off, 1 - bright, 2 - mid-bright, 3 - dim, 4 - flash
int operatingMode = 0;
volatile boolean buttonIsPushed = false;
int previousButtonState = 0;
int outputPWM = 0;

unsigned long timeNewKeyPress = 0;
unsigned long timeLastKeyPress = 0;
unsigned int timeDebounce = 500 ;

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
        timeNewKeyPress = millis();
         
        if( timeNewKeyPress-timeLastKeyPress >= timeDebounce)
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

    switch (operatingMode) 
    {
        case 0:
            outputPWM = 0;
            ShineLed();
            break;

        case 1:
            outputPWM = PWM_MAX;
            ShineLed();
            break;

        case 2:
            outputPWM = int(PWM_MAX/2);
            ShineLed();
            break;

        case 3:
            outputPWM = int(PWM_MAX/4);
            ShineLed();
            break;
            
        case 4:
            FlashTheLight();
            break;
    }

}


void FlashTheLight() 
{
  float Ton=(1/FLASH_RATE_HZ)*1000;
  outputPWM = PWM_MAX;
  ShineLed();
  delay(Ton);
  
  outputPWM = 0;
  ShineLed();
  delay(Ton);
}

void ShineLed() 
{
  
     analogWrite(PWM_LED_OUT, outputPWM);
}
