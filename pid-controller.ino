#include <Wire.h>
#include <Servo.h>

int Analog_in = A0;
Servo myservo;
float distance = 0;
float distance_previous_error, distance_error;
int period = 40;
float kp = 5.5;
float ki = 5;
float kd = 2500;
float distance_setpoint = 1.5;
float PID_p, PID_i, PID_d, PID_total;
unsigned long time;
int changer = 0;

void setup() {

  myservo.attach(9);
  myservo.write(60); // Write here the angle at which the platform lies flat
  pinMode(Analog_in, INPUT);

  time = millis();
}

void loop() {

  if (millis() > time + period) {

    time = millis();
    distance = get_dist(100);
    distance_error = distance_setpoint - distance;

    PID_p = kp * distance_error;
    float dist_difference = distance_error - distance_previous_error;
    PID_d = kd * (dist_difference / period);

    if (-3 < distance_error && distance_error < 3) {
      PID_i = PID_i + (ki * distance_error);
    } else {
      PID_i = 0;
    }

    PID_total = PID_p + PID_i + PID_d;
    PID_total = map(PID_total, -150, 150, 0, 150);
    
    // Precautions to prevent the servo from going to extreme angles
    if (PID_total < 20) { 
      PID_total = 20;
    }
    if (PID_total > 160) {
      PID_total = 160;
    }

    myservo.write(PID_total + 15);
    distance_previous_error = distance_error;
  }
}

float get_dist(int n) {
  long sum = 0;
  for (int i = 0; i < n; i++) {
    sum += analogRead(Analog_in);
  }
  float adc = sum / n;
  float distance_cm = 17569.7 * pow(adc, -1.2062);
  return(distance_cm);
}
