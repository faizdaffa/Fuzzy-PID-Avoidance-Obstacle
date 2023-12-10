#define M1E 33
#define M1A 32
#define M1B 17
#define M2E 18
#define M2A 19
#define M2B 5

int pwmA;
int pwmB;
int pwmKa;
int pwmKi;
int Upper = 50;
int Lower = 0;
int basePWM = 40;
int pwm = 20;

float error, integralE, derivativeE, lastError;
float PID, dt;
unsigned long now, prevTime;

void motor_innit() 
{
  pinMode(M1A, OUTPUT);
  pinMode(M1B, OUTPUT);
  pinMode(M1E, OUTPUT);
  pinMode(M2A, OUTPUT);
  pinMode(M2B, OUTPUT);
  pinMode(M2E, OUTPUT);
}

void pid(float yaw, float sp, int kp, int ki, int kd) 
{
  digitalWrite(M1A, HIGH);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, HIGH);
  digitalWrite(M2B, LOW);

  now = millis();
  dt  = (now - prevTime) / 1000;
  error = sp - yaw;
  integralE = (integralE + error) * dt;
  derivativeE = (error - lastError);
  prevTime  = now;
  lastError = error;
  
  float P = kp * error;
  float I = ki * integralE;
  float D = kd * derivativeE;

  PID = P + I + D;

  pwmA = basePWM - PID;
  if (pwmA > Upper) {pwmA = Upper;}
  if (pwmA < Lower) {pwmA = Lower;}
  pwmKa = pwmA;

  pwmB = basePWM + PID;
  if (pwmB > Upper) {pwmB = Upper;}
  if (pwmB < Lower) {pwmB = Lower;}
  pwmKi = pwmB;

  analogWrite(M1E, pwmKa);
  analogWrite(M2E, pwmKi);
  // Serial.println(String(error) + "\t" + String(P) + "\t" + String(I) + "\t" + String(D) + "\t" + String(dt) + "\t" + String(PID) + "\t" + String(pwmKi) + "\t" + String(pwmKa));
}

void maju()
{
    digitalWrite(M1A, HIGH);
    digitalWrite(M1B, LOW);
    analogWrite(M1E, pwm);
    digitalWrite(M2A, HIGH);
    digitalWrite(M2B, LOW);
    analogWrite(M2E, pwm);
}

void mundur()
{
    digitalWrite(M1A, LOW);
    digitalWrite(M1B, HIGH);
    analogWrite(M1E, pwm);
    digitalWrite(M2A, LOW);
    digitalWrite(M2B, HIGH);
    analogWrite(M2E, pwm);
}

void putar_kanan()
{
    digitalWrite(M1A, HIGH);
    digitalWrite(M1B, LOW);
    analogWrite(M1E, 25);
    digitalWrite(M2A, LOW);
    digitalWrite(M2B, HIGH);
    analogWrite(M2E, 20);
}

void putar_kiri()
{
    digitalWrite(M1A, LOW);
    digitalWrite(M1B, HIGH);
    analogWrite(M1E, 25);
    digitalWrite(M2A, HIGH);
    digitalWrite(M2B, LOW);
    analogWrite(M2E, 20);
}

void stop()
{
    digitalWrite(M1A, LOW);
    digitalWrite(M1B, LOW);
    analogWrite(M1E, 0);
    digitalWrite(M2A, LOW);
    digitalWrite(M2B, LOW);
    analogWrite(M2E, 0);
}