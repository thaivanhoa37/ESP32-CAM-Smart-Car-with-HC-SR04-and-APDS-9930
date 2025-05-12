#include "esp_camera.h"
#include <WiFi.h>
#include <WebServer.h>
#include "esp_http_server.h"

// Camera pin configuration (AI Thinker ESP32-CAM)
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// Wi-Fi credentials
const char* ssid = "vanhoa";
const char* password = "11111111";

// Web server for motor control
WebServer server(80);

// Movement command handler
void handleMove() {
  if (server.hasArg("dir")) {
    String dir = server.arg("dir");
    if (dir == "F" || dir == "L" || dir == "R" || dir == "S" || dir == "B") {
      Serial.write(dir.charAt(0)); // Chỉ gửi một ký tự lệnh
      Serial.print("Sent command: "); // Debug qua Serial Monitor
      Serial.println(dir);
      server.send(200, "text/plain", "Sent " + dir);
      return;
    }
  }
  Serial.println("Invalid command received from client");
  server.send(400, "text/plain", "Invalid");
}

// Start camera server on port 81
void startCameraServer() {
  static httpd_handle_t stream_httpd = NULL;

  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 81;

  httpd_uri_t stream_uri = {
    .uri       = "/stream",
    .method    = HTTP_GET,
    .handler   = [](httpd_req_t *req) {
      camera_fb_t * fb = NULL;
      esp_err_t res = ESP_OK;
      size_t _jpg_buf_len;
      uint8_t * _jpg_buf;

      res = httpd_resp_set_type(req, "multipart/x-mixed-replace; boundary=frame");
      if (res != ESP_OK) return res;

      while (true) {
        fb = esp_camera_fb_get();
        if (!fb) {
          Serial.println("Camera capture failed");
          continue;
        }

        _jpg_buf_len = fb->len;
        _jpg_buf = fb->buf;

        char part_buf[64];
        size_t hlen = snprintf(part_buf, 64,
          "--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n", _jpg_buf_len);
        res = httpd_resp_send_chunk(req, part_buf, hlen);
        res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
        res = httpd_resp_send_chunk(req, "\r\n", 2);

        esp_camera_fb_return(fb);
        if (res != ESP_OK) break;
      }
      return res;
    },
    .user_ctx  = NULL
  };

  if (httpd_start(&stream_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(stream_httpd, &stream_uri);
    Serial.println("Stream server started on port 81");
  } else {
    Serial.println("Failed to start stream server!");
  }
}

// Initialize the camera
void startCamera() {
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
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 10;
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}

// Setup
void setup() {
  delay(3000); // Power stabilization
  Serial.begin(9600); // Tốc độ baud khớp với Arduino Uno
  Serial.println("Booting...");

  startCamera();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("Stream URL: http://");
  Serial.print(WiFi.localIP());
  Serial.println(":81/stream");

  server.on("/move", handleMove);
  server.begin();
  Serial.println("HTTP server started.");

  startCameraServer();
}

// Main loop
void loop() {
  server.handleClient();
}