#define PWM_MAX_VAL             255
#define SPEED_V_MIN_VAL         1
#define SPEED_V_MAX_VAL         5
#define PIN_PWM_R_MOTOR         5
#define PIN_PWM_L_MOTOR         6

#define MAX_SERIAL_STR_LEN      2

char userInput[MAX_SERIAL_STR_LEN] = "";

void setup() {
  Serial.begin(9600);
  Serial.println("Here We Go!");
  Serial.println("Choose Motor by R / L");
  Serial.println("Choose speed level between  0 -> 5");
  Serial.println("input i.e. - R1");
  Serial.println("input i.e. - L4");

  setPwmFrequency(PIN_PWM_R_MOTOR, 1);
  setPwmFrequency(PIN_PWM_L_MOTOR, 1);

  setPWM(PIN_PWM_R_MOTOR, 0);
  setPWM(PIN_PWM_L_MOTOR, 0);
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

void loop() {
  if (! getInput()) {
    return;
  }
  char motorChoice = userInput[0];
  Serial.print("motorChoice is "); Serial.println(motorChoice);
  
  if (! getInput()) {
    return;
  }
  int valuePWM = atoi((char*)&userInput[1]);
  Serial.print("valuePWM is "); Serial.println(valuePWM);
  
  int pinPWM = 3;
  switch (motorChoice) {
    case 'R':
      pinPWM = PIN_PWM_R_MOTOR;
      break;
    case 'L':
      pinPWM = PIN_PWM_L_MOTOR;
      break;
    default:
      Serial.println("ERR- illigal motor choice");
      break;
  }
  
  setPWM(pinPWM, valuePWM);
}

bool getInput() {
  int index = 0;
  bool hasInput = false;

  if (index == MAX_SERIAL_STR_LEN) {
    Serial.println("ERR: input too long");
  }

  while (Serial.available() && index < MAX_SERIAL_STR_LEN) {
    hasInput = true;
    char ch = Serial.read();
    if (ch != 10) {
      userInput[index++] = ch;
    }
    else {
      userInput[index] = '\0';
      Serial.print("input is ");
      Serial.println(userInput);
    }

    delay(100);
  }

  return hasInput;
}

void setPWM(int ledPin, int speedV) {
  int pwmVal = map(speedV, SPEED_V_MIN_VAL, SPEED_V_MAX_VAL, 0, PWM_MAX_VAL);
  if (speedV <= 0) {
    pwmVal = 0;
  }
  if (pwmVal >= PWM_MAX_VAL) {
    pwmVal = PWM_MAX_VAL;
  }
  
  Serial.println(pwmVal);
  analogWrite(ledPin, pwmVal);
  delay(2000);
}
