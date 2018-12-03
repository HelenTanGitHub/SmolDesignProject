/* defining preoprocessor directive
 includes flash rate (Hz), button pin, PWM output pin, PWM max
 */

#define FLASH_RATE_HZ 2 
#define BUTTON_IN 2 
#define PWM_LED_OUT 3
#define PWM_MAX 255

/*Operating modes include
 * 0 - off
 * 1 - bright
 * 2 - mid-bright
 * 3 - dim
 * 4 - flash
 */

 int operating_mode = 0;
 boolean button_pushed = false;
 boolean previous_button_state = false;
 boolean flash_light = false;
 int PWM_OUT = 0;
 const long interval = int(1000/FLASH_RATE_HZ/2); 
 int ledState = LOW;
 unsigned long previousMillis = 0;
 
void setup() {
  pinMode(BUTTON_IN, INPUT);
  pinMode(PWM_LED_OUT, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_IN), button_pushed_interrupt, FALLING);
  Serial.begin(9600);
  analogWrite(PWM_LED_OUT, PWM_OUT);
}

void loop() {
  check_button_press();

  switch (operating_mode) {
    case 0:
      PWM_OUT = 0;
      flash_light = false;
      break;
    case 1:
      PWM_OUT = PWM_MAX;
      break;
    case 2:
      PWM_OUT = int(PWM_MAX/2);
      break;
    case 3:
      PWM_OUT = int(PWM_MAX/4);
      break;
    case 4:
      flash_light = true;
      break;
  }
  output();
  //Serial.println(operating_mode); - this works
}

void button_pushed_interrupt(){
  if(!previous_button_state){
    button_pushed = true;
    previous_button_state = true;
    //Serial.print("pushed"); - this works
  }
}

void flash_the_light(){
   unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(PWM_LED_OUT, ledState);
  }
}

/* if the button is pushed, change the operating mode of the light,
 *  reset the button_pressed variables
 */
void check_button_press(){
  if (button_pushed) {
    previous_button_state = false;
    button_pushed = false;
    if(operating_mode < 4){
      operating_mode += 1;
    }
    else {
      operating_mode = 0;
    }
  }
}

void output(){
  if(flash_light){
    flash_the_light();
  }
  else{
    analogWrite(PWM_LED_OUT, PWM_OUT);
  }
}

