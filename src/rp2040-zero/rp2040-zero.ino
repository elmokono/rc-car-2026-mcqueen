#include <Servo.h>
#include <Adafruit_NeoPixel.h>

#define USE_ACCEL_RAMP 0
// ---------------- Pines ----------------

// Entradas receptor
const int THR_FWD = 2;
const int THR_REV = 3;
const int DIR_LEFT = 5;
const int DIR_RIGHT = 4;

// Salidas motor
const int IN1 = 9;
const int IN2 = 10;

// Servo
const int SERVO_PIN = 15;
Servo steering;

// LED RGB integrado
const int LED_PIN = 16;
Adafruit_NeoPixel pixel(1, LED_PIN, NEO_GRB + NEO_KHZ800);

// ---------------- Servo ----------------
const int SERVO_LEFT = 15;
const int SERVO_CENTER = 90;
const int SERVO_RIGHT = 165;

// ---------------- Control motor ----------------
int targetSpeed = 0;
int currentSpeed = 0;

const int ACCEL_STEP = 15;
const int BRAKE_STEP = 15;

// ---------------- Funciones ----------------
void setMotor(int speed) {
  speed = constrain(speed, -255, 255);

  if (speed > 0) {
    analogWrite(IN1, speed);
    analogWrite(IN2, 0);
  } else if (speed < 0) {
    analogWrite(IN1, 0);
    analogWrite(IN2, -speed);
  } else {
    analogWrite(IN1, 0);
    analogWrite(IN2, 0);
  }
}

void setLED(uint8_t r, uint8_t g, uint8_t b) {
  pixel.setPixelColor(0, pixel.Color(r, g, b));
  pixel.show();
}

// ---------------- Setup ----------------
void setup() {
  pinMode(THR_FWD, INPUT);
  pinMode(THR_REV, INPUT);
  pinMode(DIR_LEFT, INPUT);
  pinMode(DIR_RIGHT, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  analogWriteFreq(20000);  // 20 kHz, silencioso y eficiente
  analogWriteRange(255);

  steering.attach(SERVO_PIN);
  steering.write(SERVO_CENTER);

  pixel.begin();
  pixel.clear();
  pixel.show();
}

// ---------------- Loop ----------------
void loop() {

  // -------- Dirección --------
  bool left = digitalRead(DIR_LEFT);
  bool right = digitalRead(DIR_RIGHT);

  if (left && !right) {
    steering.write(SERVO_LEFT);
  } else if (!left && right) {
    steering.write(SERVO_RIGHT);
  } else {
    steering.write(SERVO_CENTER);
  }

  // -------- Tracción --------
  bool fwd = digitalRead(THR_FWD);
  bool rev = digitalRead(THR_REV);

  if (fwd && !rev) {
    targetSpeed = 255;
  } else if (!fwd && rev) {
    targetSpeed = -255;
  } else {
    targetSpeed = 0;
  }

  // -------- Rampa de velocidad --------
#if USE_ACCEL_RAMP

  // Rampa progresiva
  if (currentSpeed < targetSpeed) {
    currentSpeed += ACCEL_STEP;
    if (currentSpeed > targetSpeed)
      currentSpeed = targetSpeed;
  } else if (currentSpeed > targetSpeed) {
    currentSpeed -= BRAKE_STEP;
    if (currentSpeed < targetSpeed)
      currentSpeed = targetSpeed;
  }

#else

  // Respuesta instantánea
  currentSpeed = targetSpeed;

#endif

  setMotor(currentSpeed);

  // -------- LED de estado --------
  if (currentSpeed == 0) {
    setLED(0, 0, 50);  // azul
  } else if (currentSpeed > 0) {
    setLED(0, 50, 0);  // verde
  } else {
    setLED(50, 0, 0);  // rojo
  }

  delay(20);
}
