// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// ESP32 Remote WIFI Car

#include "esp_camera.h"
#include <WiFi.h>
#include <FS.h>
#include <SPIFFS.h>

extern "C" {
  #include "mbedtls/base64.h"
}

#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "camera_pins.h"

extern String WiFiAddr;
extern String readFile(fs::FS &fs, const char * path);
void startCameraServer();

const char *ap_ssid = "vanhoa";
const char *ap_password = "11111111";
String hostname = "Remote WIFI Car";

extern int gpLb =  2;
extern int gpLf = 14;
extern int gpRb = 15;
extern int gpRf = 13;
extern int gpLed =  4;
extern int gpClaxon =  12;

unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  pinMode(gpLf, OUTPUT);
  pinMode(gpLb, OUTPUT);
  pinMode(gpRf, OUTPUT);
  pinMode(gpRb, OUTPUT);
  pinMode(gpLed, OUTPUT);
  pinMode(gpClaxon, OUTPUT);

  digitalWrite(gpLf, LOW);
  digitalWrite(gpLb, LOW);
  digitalWrite(gpRf, LOW);
  digitalWrite(gpRb, LOW);
  digitalWrite(gpLed, LOW);
  digitalWrite(gpClaxon, LOW);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_CIF);

  Serial.print("Configuring access point...");

  String sta_ssid = readFile(SPIFFS, "/wifi_ssid.txt");
  String sta_password = readFile(SPIFFS, "/wifi_pass.txt");

  const char* ssidchar = sta_ssid.c_str();
  const char* passchar = sta_password.c_str();

  size_t outputLength1, outputLength2;
  unsigned char decodedssid[64];
  unsigned char decodedpass[64];

  mbedtls_base64_decode(decodedssid, sizeof(decodedssid), &outputLength1, (const unsigned char *)ssidchar, strlen(ssidchar));
  mbedtls_base64_decode(decodedpass, sizeof(decodedpass), &outputLength2, (const unsigned char *)passchar, strlen(passchar));

  String ssidstring = String((char*)decodedssid);
  String passstring = String((char*)decodedpass);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssidstring.c_str(), passstring.c_str());
  Serial.println("\nConnecting to: " + sta_ssid);

  unsigned long currentMillis = millis();
  previousMillis = currentMillis;
  while (WiFi.status() != WL_CONNECTED && currentMillis - previousMillis <= 10000) {
    delay(500);
    Serial.print(".");
    currentMillis = millis();
  }

  IPAddress myIP;
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n*WiFi-STA-Mode*");
    Serial.print("IP: ");
    myIP = WiFi.localIP();
    Serial.println(myIP);
  } else {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(hostname.c_str());
    myIP = WiFi.softAPIP();
    Serial.println("\nWiFi failed connected to " + sta_ssid);
    Serial.println("\n*WiFi-AP-Mode*");
    Serial.print("AP IP address: ");
    Serial.println(myIP);
  }

  startCameraServer();
  Serial.print("Camera Ready! Use 'http://");
  Serial.print(myIP);
  WiFiAddr = myIP.toString();
  Serial.println("' to connect");
}
  
void loop() {
}
