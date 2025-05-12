# 🚗 ESP32-CAM Smart Car with Obstacle Avoidance and Live Streaming

This project features a smart car controlled by an ESP32-CAM board. The car supports live video streaming, remote direction control via web, and autonomous obstacle avoidance using an ultrasonic sensor.

---

## 📷 Features

- Live video stream over Wi-Fi (ESP32-CAM at `/stream`)
- Remote directional control via web interface (forward, backward, left, right, stop)
- Automatic obstacle detection and avoidance using ultrasonic sensor (HC-SR04)
- Visual feedback using green/red LEDs
- Simple serial communication between ESP32-CAM and Arduino Uno (motor control)

---

## 🛠 Hardware Requirements

| Component          | Quantity |
|--------------------|----------|
| ESP32-CAM (AI Thinker) | 1        |
| Arduino Uno (or Nano) | 1        |
| L298N Motor Driver     | 1        |
| Ultrasonic Sensor (HC-SR04) | 1        |
| 4x TT Motors (with wheels) | 2 (or 4) |
| LEDs (Red + Green)     | 1 each   |
| Jumper wires + Battery pack | -        |

---

## ⚙️ Wiring Overview

### ESP32-CAM
- Connect to computer via FTDI or USB-UART
- Streams video on `http://<ESP_IP>:81/stream`
- Sends single-letter commands to Uno via Serial:
  - `F` = Forward
  - `B` = Backward
  - `L` = Turn Left
  - `R` = Turn Right
  - `S` = Stop

### Arduino Uno to L298N (motor control)
| Arduino Pin | L298N Pin |
|-------------|-----------|
| D8          | IN1       |
| D9          | IN2       |
| D10         | IN3       |
| D11         | IN4       |
| ❌ ENA      | Jumper to 5V |
| ❌ ENB      | Jumper to 5V |
| GND         | GND (shared with ESP32-CAM) |

### HC-SR04 Ultrasonic Sensor
| Arduino Pin | Sensor Pin |
|-------------|-------------|
| A0          | Trig        |
| A1          | Echo        |

### LEDs
| Arduino Pin | Color  |
|-------------|--------|
| D3          | Green  |
| D4          | Red    |

---

## 💻 Software

### ESP32-CAM Code (Arduino)
- Handles camera initialization
- Sets up web server for stream and `/move?dir=F|B|L|R|S`
- Sends command characters over `Serial` to Uno

### Arduino Uno Code
- Receives commands from ESP32-CAM via Serial
- Controls motor driver via IN1–IN4
- Measures distance via HC-SR04 and overrides commands if obstacle is detected
- Turns 90° right to avoid obstacle

---

## 📡 How to Use

1. Flash both ESP32-CAM and Arduino Uno with the provided sketches.
2. Power up both boards (use external battery pack for motors).
3. Connect to Wi-Fi network (update SSID/password in ESP32-CAM code).
4. Open a browser and go to `http://<ESP32-IP>:81/stream`.
5. Use `/move?dir=F`, `/move?dir=B`, etc., to control the car.
6. Car will automatically avoid obstacles if detected under 20cm.

---

## 📷 Example



![Project Image]([https://example.com/smart-car-image.jpg](https://www.iotzone.vn/wp-content/uploads/2024/03/cach-su-dung-esp32-tim-hieu-so-do-cahn.jpg)) <!-- Replace with actual image URL -->

---

## 🧠 Author

Project by [thaivanhoa37]  
Date: May 2025  
Platform: ESP32-CAM with Arduino IDE  
Language: C++

---
