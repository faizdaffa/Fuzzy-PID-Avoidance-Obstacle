String sudut;
String jauh, sedang, dekat;
float fuzzy_kiri_jauh, fuzzy_kiri_sedang, fuzzy_kiri_dekat;
float fuzzy_depan_jauh, fuzzy_depan_sedang, fuzzy_depan_dekat;
float fuzzy_kanan_jauh, fuzzy_kanan_sedang, fuzzy_kanan_dekat;
String left, front, right;
String previousDefuzzification;

//----------------------kiri----------------------
void kiri_jauh(int ultrasonikKiri)
{
  if (ultrasonikKiri < 20) 
  {
    fuzzy_kiri_jauh = 0;
  }
  if (ultrasonikKiri >= 20 && ultrasonikKiri <= 30) 
  {
    fuzzy_kiri_jauh = ((ultrasonikKiri - 20.0) / (30 - 20.0));
  }
  if (ultrasonikKiri > 30) 
  {
    fuzzy_kiri_jauh = 1;
  }
}

void kiri_sedang(int ultrasonikKiri)
{
  if (ultrasonikKiri <= 10 || ultrasonikKiri > 30) 
  {
    fuzzy_kiri_sedang = 0;
  }
  if (ultrasonikKiri >= 10 && ultrasonikKiri <= 20) 
  {
    fuzzy_kiri_sedang = ((ultrasonikKiri - 10.0) / (20.0 - 10.0));
  }
  if (ultrasonikKiri >= 20 && ultrasonikKiri <= 30)
  {
    fuzzy_kiri_sedang = ((30.0 - ultrasonikKiri) / (30.0 - 20.0));
  }
}

void kiri_dekat(int ultrasonikKiri)
{
  if (ultrasonikKiri < 10) 
  {
    fuzzy_kiri_dekat = 1;
  }
  if (ultrasonikKiri >= 10 && ultrasonikKiri <= 20) 
  {
    fuzzy_kiri_dekat = ((20.0 - ultrasonikKiri) / (20.0 - 10.0));
  }
  if (ultrasonikKiri > 20) 
  {
    fuzzy_kiri_dekat = 0;
  }
}

//----------------------depan----------------------
void depan_jauh(int ultrasonikDepan)
{
  if (ultrasonikDepan < 20) 
  {
    fuzzy_depan_jauh = 0;
  }
  if (ultrasonikDepan >= 20 && ultrasonikDepan <= 30) 
  {
    fuzzy_depan_jauh = ((ultrasonikDepan - 20.0) / (30 - 20.0));
  }
  if (ultrasonikDepan > 30) 
  {
    fuzzy_depan_jauh = 1;
  }
}

void depan_sedang(int ultrasonikDepan)
{
  if (ultrasonikDepan <= 10 || ultrasonikDepan > 30) 
  {
    fuzzy_depan_sedang = 0;
  }
  if (ultrasonikDepan >= 10 && ultrasonikDepan <= 20) 
  {
    fuzzy_depan_sedang = ((ultrasonikDepan - 10.0) / (20.0 - 10.0));
  }
  if (ultrasonikDepan >= 20 && ultrasonikDepan <= 30)
  {
    fuzzy_depan_sedang = ((30.0 - ultrasonikDepan) / (30.0 - 20.0));
  }
}

void depan_dekat(int ultrasonikDepan)
{
  if (ultrasonikDepan < 10) 
  {
    fuzzy_depan_dekat = 1;
  }
  if (ultrasonikDepan >= 10 && ultrasonikDepan <= 20) 
  {
    // fuzzy_depan_dekat = ((20.0 - ultrasonikDepan) / (20.0 - 10.0));
    fuzzy_depan_dekat = 0;
  }
  if (ultrasonikDepan > 20) 
  {
    fuzzy_depan_dekat = 0;
  }
}

//----------------------kanan----------------------
void kanan_jauh(int ultrasonikKanan)
{
  //jauh
  if (ultrasonikKanan < 20) 
  {
    fuzzy_kanan_jauh = 0;
  }
  if (ultrasonikKanan >= 20 && ultrasonikKanan <= 30) 
  {
    fuzzy_kanan_jauh = ((ultrasonikKanan - 20.0) / (30.0 - 20.0));
  }
  if (ultrasonikKanan > 30) 
  {
    fuzzy_kanan_jauh = 1;
  }
}

void kanan_sedang(int ultrasonikKanan)
{
  //sedang
  if (ultrasonikKanan <= 10 || ultrasonikKanan > 30) 
  {
    fuzzy_kanan_sedang = 0;
  }
  if (ultrasonikKanan >= 10 && ultrasonikKanan <= 20) 
  {
    fuzzy_kanan_sedang = ((ultrasonikKanan - 10.0) / (20.0 - 10.0));
  }
  if (ultrasonikKanan >= 20 && ultrasonikKanan <= 30)
  {
    fuzzy_kanan_sedang = ((30.0 - ultrasonikKanan) / (30.0 - 20.0));
  }
}

void kanan_dekat(int ultrasonikKanan)
{
  //dekat
  if (ultrasonikKanan < 10) 
  {
    fuzzy_kanan_dekat = 1;
  }
  if (ultrasonikKanan >= 10 && ultrasonikKanan <= 20) 
  {
    fuzzy_kanan_dekat = ((20.0 - ultrasonikKanan) / (20.0 - 10.0));
  }
  if (ultrasonikKanan > 20) 
  {
    fuzzy_kanan_dekat = 0;
  }
}

String DekatSedangJauh(float a, float b, float c)
{
  float max = a;
  String data = "dekat";
  if (a > b && a > c)
  {
    data = "dekat";
  }
  else if (b > max) 
  {
    data = "sedang";
  }
  else if (c > b) 
  {
    data = "jauh";
  }
  else
  {
    data = "jauh";
  } 
  return data;
}

void fuzzification(int ultrasonikKiri, int ultrasonikDepan, int ultrasonikKanan)
{
  kiri_jauh   (ultrasonikKiri);
  kiri_sedang (ultrasonikKiri);
  kiri_dekat  (ultrasonikKiri);
  depan_dekat (ultrasonikDepan);
  depan_sedang(ultrasonikDepan);
  depan_jauh  (ultrasonikDepan);
  kanan_dekat (ultrasonikKanan);
  kanan_sedang(ultrasonikKanan);
  kanan_jauh  (ultrasonikKanan);

  left  = DekatSedangJauh(fuzzy_kiri_dekat,  fuzzy_kiri_sedang,  fuzzy_kiri_jauh);
  front = DekatSedangJauh(fuzzy_depan_dekat, fuzzy_depan_sedang, fuzzy_depan_jauh);
  right = DekatSedangJauh(fuzzy_kanan_dekat, fuzzy_kanan_sedang, fuzzy_kanan_jauh);
  // Serial.println("Left : " + left + "  front : " + front + "  right : " + right);
  // Serial.print(String(fuzzy_depan_dekat) + "  " + String(fuzzy_depan_sedang) + "  " + String(fuzzy_depan_jauh) + " " + String(front));
  Serial.print(String(fuzzy_kanan_jauh) + "  " + String(fuzzy_kanan_sedang) + "  " + String(fuzzy_kanan_dekat) + " " + String(right));
}

String ruleBase(String left, String front, String right)
{
  if (left == "dekat"  && front == "dekat"  && right == "dekat")  {sudut = "turnRight";  return sudut;}
  if (left == "dekat"  && front == "dekat"  && right == "sedang") {sudut = "turnRight";  return sudut;}
  if (left == "dekat"  && front == "dekat"  && right == "jauh")   {sudut = "turnRight";  return sudut;}
  if (left == "dekat"  && front == "sedang" && right == "dekat")  {sudut = "straight";   return sudut;}
  if (left == "dekat"  && front == "sedang" && right == "sedang") {sudut = "straight";   return sudut;}
  if (left == "dekat"  && front == "sedang" && right == "jauh")   {sudut = "straight";   return sudut;}
  if (left == "dekat"  && front == "jauh"   && right == "dekat")  {sudut = "straight";   return sudut;}
  if (left == "dekat"  && front == "jauh"   && right == "sedang") {sudut = "straight";   return sudut;}
  if (left == "dekat"  && front == "jauh"   && right == "jauh")   {sudut = "straight";   return sudut;}
  
  if (left == "sedang" && front == "dekat"  && right == "dekat")  {sudut = "turnLeft";   return sudut;}
  if (left == "sedang" && front == "dekat"  && right == "sedang") {sudut = "turnRight";  return sudut;}
  if (left == "sedang" && front == "dekat"  && right == "jauh")   {sudut = "turnRight";  return sudut;}
  if (left == "sedang" && front == "sedang" && right == "dekat")  {sudut = "straight";   return sudut;}
  if (left == "sedang" && front == "sedang" && right == "sedang") {sudut = "straight";   return sudut;}
  if (left == "sedang" && front == "sedang" && right == "jauh")   {sudut = "straight";   return sudut;}
  if (left == "sedang" && front == "jauh"   && right == "dekat")  {sudut = "straight";   return sudut;}
  if (left == "sedang" && front == "jauh"   && right == "sedang") {sudut = "straight";   return sudut;}
  if (left == "sedang" && front == "jauh"   && right == "jauh")   {sudut = "straight";   return sudut;}
  
  if (left == "jauh" && front == "dekat"  && right == "dekat")    {sudut = "turnLeft";   return sudut;}
  if (left == "jauh" && front == "dekat"  && right == "sedang")   {sudut = "turnLeft";   return sudut;}
  if (left == "jauh" && front == "dekat"  && right == "jauh")     {sudut = "turnRight";  return sudut;}
  if (left == "jauh" && front == "sedang" && right == "dekat")    {sudut = "straight";   return sudut;}
  if (left == "jauh" && front == "sedang" && right == "sedang")   {sudut = "straight";   return sudut;}
  if (left == "jauh" && front == "sedang" && right == "jauh")     {sudut = "straight";   return sudut;}
  if (left == "jauh" && front == "jauh"   && right == "dekat")    {sudut = "straight";   return sudut;}
  if (left == "jauh" && front == "jauh"   && right == "sedang")   {sudut = "straight";   return sudut;}
  if (left == "jauh" && front == "jauh"   && right == "jauh")     {sudut = "straight";   return sudut;}
}

int inferFuzzy(String defuzzification){
  if (defuzzification == "turnLeft"){
    return 90;
  }
  if (defuzzification == "turnRight"){
    return -90;
  }
  if (defuzzification == "straight"){
    return 0;
  }
}