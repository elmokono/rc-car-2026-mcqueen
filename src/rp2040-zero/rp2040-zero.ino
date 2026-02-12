#include <Servo.h>
#include <Adafruit_NeoPixel.h>

// ---------------- Pines ----------------

// Entradas receptor
const int DIR_A = 2;
const int DIR_B = 3;
const int THR_A = 4;
const int THR_B = 5;

// Salidas motor
const int IN1 = 10;
const int IN2 = 11;

// Servo
const int SERVO_PIN = 15;
Servo steering;

// LED RGB integrado (WS2812)
const int LED_PIN = 16;
Adafruit_NeoPixel pixel(1, LED_PIN, NEO_GRB + NEO_KHZ800);

// ---------------- Servo ----------------
const int SERVO_LEFT = 60;
const int SERVO_CENTER = 90;
const int SERVO_RIGHT = 120;

// ---------------- Control motor ----------------
int targetSpeed = 0;
int currentSpeed = 0;

const int ACCEL_STEP = 5;
const int BRAKE_STEP = 10;

// ---------------- Funciones ----------------
void setMotor(int speed) {
  speed = constrain(speed, -255, 255);

  if (speed > 0) {
    analogWrite(IN1, speed);
    analogWrite(IN2, 0);
  } 
  else if (speed < 0) {
    analogWrite(IN1, 0);
    analogWrite(IN2, -speed);
  } 
  else {
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
  pinMode(DIR_A, INPUT);
  pinMode(DIR_B, INPUT);
  pinMode(THR_A, INPUT);
  pinMode(THR_B, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  steering.attach(SERVO_PIN);
  steering.write(SERVO_CENTER);

  pixel.begin();
  pixel.clear();
  pixel.show();
}

// ---------------- Loop ----------------
void loop() {
  // -------- Dirección --------
  bool dirA = digitalRead(DIR_A);
  bool dirB = digitalRead(DIR_B);

  if (dirA && !dirB) {
    steering.write(SERVO_LEFT);
  } 
  else if (!dirA && dirB) {
    steering.write(SERVO_RIGHT);
  } 
  else {
    steering.write(SERVO_CENTER);
  }

  // -------- Tracción --------
  bool thrA = digitalRead(THR_A);
  bool thrB = digitalRead(THR_B);

  if (thrA && !thrB) {
    targetSpeed = 200;   // adelante
  } 
  else if (!thrA && thrB) {
    targetSpeed = -150;  // reversa
  } 
  else {
    targetSpeed = 0;
  }

  // -------- Rampa de velocidad --------
  if (currentSpeed < targetSpeed) {
    currentSpeed += ACCEL_STEP;
    if (currentSpeed > targetSpeed)
      currentSpeed = targetSpeed;
  }
  else if (currentSpeed > targetSpeed) {
    currentSpeed -= BRAKE_STEP;
    if (currentSpeed < targetSpeed)
      currentSpeed = targetSpeed;
  }

  setMotor(currentSpeed);

  // -------- Estado LED --------
  if (currentSpeed == 0) {
    // Idle
    setLED(0, 0, 50); // azul suave
  }
  else if (currentSpeed > 0) {
    // Acelerando
    setLED(0, 50, 0); // verde
  }
  else {
    // Reversa / freno
    setLED(50, 0, 0); // rojo
  }

  delay(20);
}
