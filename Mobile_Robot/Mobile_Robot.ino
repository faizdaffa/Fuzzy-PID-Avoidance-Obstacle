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

unsigned long timer = 0;
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
  mpu.update();
  if (millis() - timer > 10)  // print data every 10ms
  {
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
          rotasi(setpoint, z);
          // collectDataLeft += 1;
        }
        else if (resultModus == -90)
        {
          setpoint -= 90;
          rotasi(setpoint, z);
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
    z = mpu.getAngleZ();
    pid(z, setpoint, yourInputKP, yourInputKI, yourInputKD);
    
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
    display.print(z, 1);
    //PID Setting
    display.setCursor(70, 20);
    display.print("KP:" + String(yourInputKP));
    display.setCursor(70, 36);
    display.print("KI:" + String(yourInputKI));
    display.setCursor(70, 56);
    display.print("KD:" + String(yourInputKD));
    // display.print("SP:" + String(setpoint));
    display.display();
    timer = millis();

    // Serial.println(String(mpu.getAngleZ()) + "\t" + String(kalman_mpu6050));
  }
//   Serial.println(String(i) + ". " + left + " " + front + " " + right + " = " + String(resultModus) + " ==> " + String(collectDataLeft) + "\t" + String(collectDataStraight) + "\t" + String(collectDataRight));
   Serial.println(left + " " + front + " " + right + " = " + String(resultModus));
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
  Serial.println("11111111111");
  while (pwm > 0) 
  {
  Serial.println("slow down!");
  for (int i = pwm; i >= 0; i--) {
    Serial.println("pwm decrease");
    digitalWrite(M1A, HIGH);
    digitalWrite(M1B, LOW);
    digitalWrite(M2A, HIGH);
    digitalWrite(M2B, LOW);
    analogWrite(M1E, i);
    analogWrite(M2E, i);
    delay(100); // Adjust this delay to control the speed reduction rate
  }
}

  Serial.println("444444444444");
  stop();
  delay(1000);
  if (error > 0) 
  {
    putar_kiri();
    if (bacaZ == setSudut) {stop(); delay(1000);}
  } 
  if (error < 0) 
  {
    putar_kanan();
    if (bacaZ == setSudut) {stop(); delay(1000);}
  }
}
