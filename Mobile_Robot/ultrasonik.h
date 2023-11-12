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

void read_ultrasonik()
{
  delay(50);

  KiriBelakang  = sonar[0].ping_cm(); // Send ping, get ping time in microseconds (uS).
  KiriDepan     = sonar[1].ping_cm(); // Send ping, get ping time in microseconds (uS).
  Depan         = sonar[2].ping_cm(); // Send ping, get ping time in microseconds (uS).
  KananDepan    = sonar[3].ping_cm(); // Send ping, get ping time in microseconds (uS).
  KananBelakang = sonar[4].ping_cm(); // Send ping, get ping time in microseconds (uS).

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

  Kiri  = (KiriDepan  +  KiriBelakang) / 2;
  Kanan = (KananDepan + KananBelakang) / 2;
}