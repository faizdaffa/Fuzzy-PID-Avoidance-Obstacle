#include <WiFi.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <MPU6050_light.h>

AsyncWebServer server(80);
MPU6050 mpu(Wire);

const char* ssid = "BaseEngineering";
const char* password = "123456780";

const char* KP = "inputKP";
const char* KI = "inputKI";
const char* KD = "inputKD";

String read_dataZ()
{
  return String(baca_yaw);
}

void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}

String readFile(fs::FS& fs, const char* path) {
  //Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if (!file || file.isDirectory()) {
    //Serial.println("- empty file or failed to open file");
    return String();
  }
  //Serial.println("- read from file:");
  String fileContent;
  while (file.available()) {
    fileContent += String((char)file.read());
  }
  file.close();
  //Serial.println(fileContent);
  return fileContent;
}

void writeFile(fs::FS& fs, const char* path, const char* message) {
  //Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if (!file) {
    //Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    //Serial.println("- file written");
  } else {
    //Serial.println("- write failed");
  }
  file.close();
}

// Replaces placeholder with stored values
String processor(const String& var) {
  //Serial.println(var);
  if (var == "inputKP") {
    return readFile(SPIFFS, "/inputKP.txt");
  } else if (var == "inputKI") {
    return readFile(SPIFFS, "/inputKI.txt");
  } else if (var == "inputKD") {
    return readFile(SPIFFS, "/inputKD.txt");
  }
  return String();
}

void webserver_innit() {  
  //SPIFFS=====================================================================
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  //WIFI=====================================================================
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  //HTML=====================================================================
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  
  server.on("/yaw", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", read_dataZ().c_str());
  });

  // Send a GET request to <ESP_IP>/get?inputString=<inputMessage>
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest * request) {
    String inputMessage;
    // GET inputString value on <ESP_IP>/get?inputString=<inputMessage>
    if (request->hasParam(KP)) {
      inputMessage = request->getParam(KP)->value();
      writeFile(SPIFFS, "/inputKP.txt", inputMessage.c_str());
    }
    // GET inputInt value on <ESP_IP>/get?inputInt=<inputMessage>
    else if (request->hasParam(KI)) {
      inputMessage = request->getParam(KI)->value();
      writeFile(SPIFFS, "/inputKI.txt", inputMessage.c_str());
    }
    // GET inputFloat value on <ESP_IP>/get?inputFloat=<inputMessage>
    else if (request->hasParam(KD)) {
      inputMessage = request->getParam(KD)->value();
      writeFile(SPIFFS, "/inputKD.txt", inputMessage.c_str());
    } else {
      inputMessage = "No message sent";
    }
    //Serial.println(inputMessage);
    request->send(200, "text/text", inputMessage);
  });

  server.onNotFound(notFound);
  server.begin();
  //MPU6050==========================================================================
  Wire.begin();
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(true, true); // gyro and accelero
  Serial.println("Done!\n");
}

void read_input() {
  yourInputKP = readFile(SPIFFS, "/inputKP.txt").toFloat();
  // Serial.print("KP:");
  // Serial.print(yourInputKP);

  yourInputKI = readFile(SPIFFS, "/inputKI.txt").toFloat();
  // Serial.print("  KI:");
  // Serial.print(yourInputKI);

  yourInputKD = readFile(SPIFFS, "/inputKD.txt").toFloat();
  // Serial.print("  KD:");
  // Serial.print(yourInputKD);
}
