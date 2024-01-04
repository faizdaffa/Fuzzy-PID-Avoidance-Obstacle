#include <NewPing.h>

#define TRIGGER_1  12 // KiriBelakang
#define ECHO_1     13
#define TRIGGER_2  27 // KiriDepan
#define ECHO_2     14
#define TRIGGER_3  25 // Depan
#define ECHO_3     26
#define TRIGGER_4   4 // KananDepan
#define ECHO_4     16
#define TRIGGER_5  15 // KananBelakang
#define ECHO_5      2
#define MAX       400
#define SONAR_NUM   5

NewPing sonar[SONAR_NUM] = 
{
  // Sensor object array.
  // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(TRIGGER_1, ECHO_1, MAX),
  NewPing(TRIGGER_2, ECHO_2, MAX),
  NewPing(TRIGGER_3, ECHO_3, MAX),
  NewPing(TRIGGER_4, ECHO_4, MAX),
  NewPing(TRIGGER_5, ECHO_5, MAX)
};

int KiriBelakang  = 0;
int KiriDepan  = 0;
int Kiri  = 0;
int Depan = 0;
int Kanan = 0;
int KananDepan = 0;
int KananBelakang = 0;
float distance1, distance2, distance3, distance4, distance5, d_left, d_right;

void read_ultrasonik()
{
  delay(50);

  // KiriBelakang  = sonar[0].ping_cm(); // Send ping, get ping time in microseconds (uS).
  Kiri     = sonar[1].ping_cm(); // Send ping, get ping time in microseconds (uS).
  Depan         = sonar[2].ping_cm(); // Send ping, get ping time in microseconds (uS).
  Kanan    = sonar[3].ping_cm(); // Send ping, get ping time in microseconds (uS).
  // KananBelakang = sonar[4].ping_cm(); // Send ping, get ping time in microseconds (uS).

  // Serial.print("KiriD: ");
  // Serial.print(KiriDepan);
  // Serial.print("  KiriB: ");
  // Serial.print(KiriBelakang);
  // Serial.print("  Depan: ");
  // Serial.print(Depan);
  // Serial.print("  KananD: ");
  // Serial.print(KananDepan);
  // Serial.print("  KananB: ");
  // Serial.println(KananBelakang);

  // Kiri  = (KiriDepan  +  KiriBelakang) / 2;
  // Kanan = (KananDepan + KananBelakang) / 2;
}
void manual_innit()
{ 
  pinMode(TRIGGER_1, OUTPUT);
  pinMode(ECHO_1, INPUT);
  pinMode(TRIGGER_2, OUTPUT);
  pinMode(ECHO_2, INPUT);
  pinMode(TRIGGER_3, OUTPUT);
  pinMode(ECHO_3, INPUT);
  pinMode(TRIGGER_4, OUTPUT);
  pinMode(ECHO_4, INPUT);
  pinMode(TRIGGER_5, OUTPUT);
  pinMode(ECHO_5, INPUT);
}

float measureDistance(int triggerPin, int echoPin) 
{
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;  // Convert to centimeters

  return distance;
}

void manual_ultrasonic()
{
  distance1 = measureDistance(TRIGGER_1, ECHO_1);
  distance2 = measureDistance(TRIGGER_2, ECHO_2);
  distance3 = measureDistance(TRIGGER_3, ECHO_3);
  distance4 = measureDistance(TRIGGER_4, ECHO_4);
  distance5 = measureDistance(TRIGGER_5, ECHO_5);
  d_left    = abs(distance1 - distance2);
  d_right   = abs(distance4 - distance5);
  // Serial.print(f"Distance 1: {} cm", distance1);
  // Serial.println(f"Distance 2: {} cm", distance2);
}