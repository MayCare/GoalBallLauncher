#define BRAKE 0
#define CW    1
#define CCW   2

#define PWM_MAX_VAL             255
#define SPEED_V_MIN_VAL         1
#define SPEED_V_MAX_VAL         5

//MOTOR R
#define PIN_PWM_R_MOTOR                    5
#define DEFAULT_SPEED_VALUE_R_MOTOR_volt   4

//MOTOR L
#define PIN_PWM_L_MOTOR                    6
#define DEFAULT_SPEED_VALUE_R_MOTOR_volt   4

//LAUNCH BALL ACTUATOR
#define LOAD_BALL_ACTUATOR_CW_PIN     7
#define LOAD_BALL_ACTUATOR_CCW_PIN    8
#define LOAD_BALL_ACTUATOR_SPEED_PIN  4
#define LOAD_BALL_ACTUATOR_EN_PIN     4


//Note: in order to shoot, hold in pin button 4 pressed,
//Then, while 4 is pressed, press button in pin 2
#define PIN_SHOOT_ENABLE      13
#define PIN_SHOOT_INTERRUPT   2

//TODO - replace this boolean flag with multiple states (more like a state machine).
bool g_shootPressed = false;

void setup() {
  Serial.begin(9600);

  // NOTE! setting the frequency changes the clock and affects delay().
  //USE ONLY IF MUST! Then TEST and ADJUST delay() usages in the code accordingly
  //setPwmFrequency(PIN_PWM_R_MOTOR, 1);
  //setPwmFrequency(PIN_PWM_L_MOTOR, 1);

  setupUserInterface();
  setupLoadBallActuator();
  
  attachInterrupt(digitalPinToInterrupt(PIN_SHOOT_INTERRUPT), shootButtonISR, FALLING); // trigger when button pressed, but not when released.
}


void loop() {
  if (g_shootPressed && isShootingEnabled()) {
    Serial.println("If you have to shoot - SHOOT!"); // don't talk :)
    shoot();
  }
}

bool isShootingEnabled() {
  //<DEBUG CODE!!>
  return true;
  //</DEBUG CODE!!>  
  //COMMENT IN the line below when done debugging. 
  //This is a saftey mechanism:
  //return (digitalRead(PIN_SHOOT_ENABLE) == LOW);
}

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if (pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch (divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if (pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if (pin == 3 || pin == 11) {
    switch (divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}

void setupUserInterface() {
  pinMode(PIN_SHOOT_ENABLE, INPUT_PULLUP);
  pinMode(PIN_SHOOT_INTERRUPT, INPUT_PULLUP);
}

void setupLoadBallActuator() {
  pinMode(LOAD_BALL_ACTUATOR_CW_PIN, OUTPUT);
  pinMode(LOAD_BALL_ACTUATOR_CCW_PIN, OUTPUT);
  pinMode(LOAD_BALL_ACTUATOR_SPEED_PIN, OUTPUT);
  pinMode(LOAD_BALL_ACTUATOR_EN_PIN, OUTPUT);
}

void setPWM(int ledPin, int speedV) {
  int pwmVal = map(speedV, SPEED_V_MIN_VAL, SPEED_V_MAX_VAL, 0, PWM_MAX_VAL);
  Serial.println(pwmVal);
  delay(2000);
}


void shootButtonISR() {
  g_shootPressed = true;
  //TODO Add timeout mechanism to elapse flags.
}


void moveLoadBallActuator(int dir) {
  digitalWrite(LOAD_BALL_ACTUATOR_EN_PIN, HIGH);
  if (dir == CW)
    {
      digitalWrite(LOAD_BALL_ACTUATOR_CW_PIN, LOW);
      digitalWrite(LOAD_BALL_ACTUATOR_CCW_PIN, HIGH);
    }
    else if (dir == CCW)
    {
      digitalWrite(LOAD_BALL_ACTUATOR_CW_PIN, HIGH);
      digitalWrite(LOAD_BALL_ACTUATOR_CCW_PIN, LOW);
    }
    else
    {
      digitalWrite(LOAD_BALL_ACTUATOR_CW_PIN, LOW);
      digitalWrite(LOAD_BALL_ACTUATOR_CCW_PIN, LOW);
      digitalWrite(LOAD_BALL_ACTUATOR_SPEED_PIN, LOW);
    }
}


void shoot() {
  g_shootPressed = false;
  
  moveLoadBallActuator(CW);
  delay(2000);
  moveLoadBallActuator(CCW);
  delay(2000);
  moveLoadBallActuator(BRAKE);

  //move motors by:
  //setPWM(PIN_PWM_R_MOTOR, valuePWM); // to set speed of motors
}
