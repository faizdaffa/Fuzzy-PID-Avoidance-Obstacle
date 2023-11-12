#define M1E 33
#define M1A 32
#define M1B 17
#define M2E 18
#define M2A 19
#define M2B 5

int pwm = 255;

void motor_innit()
{
    pinMode(M1A, OUTPUT);
    pinMode(M1B, OUTPUT);
    pinMode(M1E, OUTPUT);
    pinMode(M2A, OUTPUT);
    pinMode(M2B, OUTPUT);
    pinMode(M2E, OUTPUT);
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
    analogWrite(M1E, pwm);
    digitalWrite(M2A, LOW);
    digitalWrite(M2B, HIGH);
    analogWrite(M2E, pwm);
}

void putar_kiri()
{
    digitalWrite(M1A, LOW);
    digitalWrite(M1B, HIGH);
    analogWrite(M1E, pwm);
    digitalWrite(M2A, HIGH);
    digitalWrite(M2B, LOW);
    analogWrite(M2E, pwm);
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

void tes_motor()
{
    maju();   delay(1000);
    stop();   delay(1000);
    mundur(); delay(1000);
    stop();   delay(1000);
}