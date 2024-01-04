//TEST GITHUB
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// #include "webserver.h"
#include <MPU6050_light.h>
#include "ultrasonik.h"
// #include "motor.h"
#include "fuzzy.h"
#include "pid.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

MPU6050 mpu(Wire);
float yourInputKP = 4.8;
float yourInputKI = 10.6;
float yourInputKD = 2.2;
float baca_yaw;

unsigned long currentMillis;
const unsigned long interval1 = 10;
unsigned long prevMillis1 = 0;
int motorState = 0;

int resultModus = 0, previousModus = 0;
const int N_SAMPLES = 100;
int modus[N_SAMPLES];
float z, CAL, setpoint;
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
  // manual_innit();
  motor_innit();
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.clearDisplay();

  Wire.begin();
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  display.println(F("Calculating gyro offset, do not move MPU6050"));
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(2000);
  // mpu.upsideDownMounting = true;  // uncomment this line if the MPU6050 is mounted upside-down
  mpu.calcOffsets(true, true); // gyro and accelero
  //CAL OFFSET2
  z   = mpu.getAngleZ();
  CAL = 0 - z;
  setpoint = z;
  Serial.println("sp = " + String(setpoint) + "   z = " + String(z));
  Serial.println("Done!\n");

  // webserver_innit();
  // delay(2000);
  display.display();
  // Serial.println(String(i) + ". " + left + " " + front + " " + right + " = " + String(resultModus) + " ==> " + String(collectDataLeft) + "\t" + String(collectDataStraight) + "\t" + String(collectDataRight));
}

void loop() 
{
  mpu.update();
  currentMillis = millis();
  if (currentMillis - prevMillis1 >= 1) 
  {
    prevMillis1 = currentMillis;
    baca_yaw = mpu.getAngleZ();;
    // Serial.print(String(baca_yaw) + "   ");

    display.clearDisplay();
    //read distance ultrasonic
    // manual_ultrasonic();
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
        else
        {
          setpoint = setpoint;
          // collectDataStraight += 1;
        }
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
    // read_input();  //input KP, KI, KD webserver
    // z = mpu.getAngleZ();
    pid(baca_yaw, setpoint, yourInputKP, yourInputKI, yourInputKD);
    
    
    // Serial.println(String(setpoint) + ";" + String(baca_yaw));
    // Serial.println(d_left);
    Serial.print(F("  "));
    Serial.print(Kanan);
    Serial.print(F("  "));
    Serial.println(setpoint);
    // Serial.print("  ||  Z = ");
    // Serial.println(mpu.getAngleZ());

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Z:" + String(baca_yaw));

    //OLED Ultrasonik
    display.setCursor(0, 20);
    display.print("L:" + String(Kiri));
    display.setCursor(0, 36);
    display.print("F:" + String(Depan));
    display.setCursor(0, 56);
    display.print("R:" + String(Kanan));
    //OLED MPU6050
    display.setCursor(70, 0);
    display.print("SP:" + String(setpoint));
    //PID Setting
    display.setCursor(70, 20);
    display.print("KP:" + String(yourInputKP));
    display.setCursor(70, 36);
    display.print("KI:" + String(yourInputKI));
    display.setCursor(70, 56);
    display.print("KD:" + String(yourInputKD));
    // display.print("SP:" + String(setpoint));
    display.display();
    // Serial.println(left + " " + front + " " + right + " = " + String(setpoint));

    // Serial.println(String(mpu.getAngleZ()) + "\t" + String(kalman_mpu6050));
  }
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
  // if (pwm > 0) 
  // {
  //   Serial.println("slow down!");
  //   for (int i = pwm; i >= 0; i--)
  //   {
  //     if (currentMillis - prevMillis3 >= interval3)
  //     {
  //       prevMillis3 = currentMillis;
  //       Serial.println("pwm decrease");
  //       digitalWrite(M1A, HIGH);
  //       digitalWrite(M1B, LOW);
  //       digitalWrite(M2A, HIGH);
  //       digitalWrite(M2B, LOW);
  //       analogWrite(M1E, i);
  //       analogWrite(M2E, i);
  //     }
  //   }
  // }
  stop();
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