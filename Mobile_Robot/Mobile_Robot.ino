//TEST GITHUB
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "webserver.h"
#include "ultrasonik.h"
// #include "motor.h"
#include "fuzzy.h"
#include "pid.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

unsigned long currentMillis;
const unsigned long interval1 = 0;
const unsigned long interval2 = 10;
const unsigned long interval3 = 1000;
unsigned long prevMillis1 = 0;
unsigned long prevMillis2 = 0;
unsigned long prevMillis3 = 0;
unsigned long prevMillis4 = 0;

int setpoint = 0, resultModus = 0, previousModus = 0;
const int N_SAMPLES = 100;
int modus[N_SAMPLES];
float z, prevZ = 0;
bool takeData = true;
// int collectDataLeft = 0, collectDataStraight = 0, collectDataRight = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() 
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);

  Serial.begin(9600);
  motor_innit();
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.clearDisplay();
  display.println(F("Calculating gyro offset, do not move MPU6050"));
  webserver_innit();
  delay(2000);
  display.display();
}

void loop() 
{
  currentMillis = millis();
  if (currentMillis - prevMillis1 >= interval1) 
  {
    prevMillis1 = currentMillis;
    mpu.update();
    baca_yaw = mpu.getAngleZ();
    Serial.print(String(baca_yaw) + "   ");
  }

  if (currentMillis - prevMillis2 >= interval2) 
  {
    prevMillis2 = currentMillis;
    display.clearDisplay();

    //========FUZZY========
    read_ultrasonik();
    fuzzification(Kiri, Depan, Kanan);
    
    for (int i = 0; i < N_SAMPLES; i++)
    {
      String defuzzification = ruleBase(left, front, right);
      modus[i] = inferFuzzy(defuzzification);
    }
    int dataSize = sizeof(modus) / sizeof(modus[0]); // Calculate the array size
  
    // Serial.print("Modus: ");
    // Serial.println(resultModus);    
    
    previousModus = resultModus;
    resultModus = findMode(modus, dataSize);

    if (previousModus != resultModus) 
    {
      // Mengganti keputusan
      // Serial.print("BEDA\t");
      if (takeData)
      {
        // Serial.print("UPDATE");
        if (resultModus == 90)
        {
          setpoint += 90;
          rotasi(setpoint, baca_yaw);
          // collectDataLeft += 1;
        }
        else if (resultModus == -90)
        {
          setpoint -= 90;
          rotasi(setpoint, baca_yaw);
          // collectDataRight += 1;
        }
        // else
        // {
        //   collectDataStraight += 1;
        // }
        // Serial.println(takeData);
        // gerak pid
        takeData = false;
      }
    }
    else
    {
      takeData = true;
    }

    //=========PID=========
    read_input();  //input KP, KI, KD webserver
    // z = mpu.getAngleZ();
    pid(baca_yaw, setpoint, yourInputKP, yourInputKI, yourInputKD);
    
    // Serial.print("  ||  Z = ");
    // Serial.println(mpu.getAngleZ());

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    //OLED Ultrasonik
    display.setCursor(0, 0);
    display.print("L:" + String(Kiri));
    display.setCursor(0, 20);
    display.print("F:" + String(Depan));
    display.setCursor(0, 40);
    display.print("R:" + String(Kanan));
    //OLED MPU6050
    display.setTextSize(1);
    display.setCursor(70, 0);
    display.print(baca_yaw, 1);
    //PID Setting
    display.setCursor(70, 20);
    display.print("KP:" + String(yourInputKP));
    display.setCursor(70, 36);
    display.print("KI:" + String(yourInputKI));
    display.setCursor(70, 56);
    display.print("KD:" + String(yourInputKD));
    // display.print("SP:" + String(setpoint));
    display.display();
    Serial.println(left + " " + front + " " + right + " = " + String(setpoint));

    // Serial.println(String(mpu.getAngleZ()) + "\t" + String(kalman_mpu6050));
  }
  // Serial.println(String(i) + ". " + left + " " + front + " " + right + " = " + String(resultModus) + " ==> " + String(collectDataLeft) + "\t" + String(collectDataStraight) + "\t" + String(collectDataRight));
}

int findMode(int arr[], int size) 
{
  int mode = arr[0]; // Initialize mode with the first element
  int maxCount = 1; // Initialize the count of the mode

  for (int i = 0; i < size; i++) {
    int currentNumber = arr[i];
    int currentCount = 0;

    for (int j = 0; j < size; j++) {
      if (arr[j] == currentNumber) {
        currentCount++;
      }
    }

    if (currentCount > maxCount) {
      mode = currentNumber;
      maxCount = currentCount;
    }
  }

  return mode;
}

void rotasi(float setSudut, float bacaZ)
{
  float errorRotasi = setSudut - bacaZ;
  // Serial.print(String(errorRotasi) + "\t");
  if (pwm > 0) 
  {
    // Serial.println("slow down!");
    for (int i = pwm; i >= 20; i--)
    {
      if (currentMillis - prevMillis3 >= interval3)
      {
        prevMillis3 = currentMillis;
        Serial.println("pwm decrease");
        digitalWrite(M1A, HIGH);
        digitalWrite(M1B, LOW);
        digitalWrite(M2A, HIGH);
        digitalWrite(M2B, LOW);
        analogWrite(M1E, i);
        analogWrite(M2E, i);
      }
    }
  }

  if ((millis() - prevMillis4) >= 1000) 
  {
    prevMillis4 = millis();
    stop();
  }
  // delay(1000);
  if (error > 0) 
  {
    putar_kiri();
  } 
  if (error < 0) 
  {
    putar_kanan();
  }
}
