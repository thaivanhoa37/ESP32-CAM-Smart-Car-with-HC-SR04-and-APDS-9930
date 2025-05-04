# ESP32-CAM Smart Car with HC-SR04 and APDS-9930

This project is a smart robot car using the ESP32-CAM module. It combines:
- **Camera streaming** for visual monitoring
- **Ultrasonic sensor (HC-SR04)** for obstacle detection
- **APDS-9930** for proximity and ambient light sensing
- **L298N motor driver** for DC motor control

The system allows the robot to navigate autonomously while avoiding obstacles and adapting behavior based on environmental light.

---

## 🔧 Components Used

- ESP32-CAM module  
- HC-SR04 Ultrasonic Sensor  
- APDS-9930 Proximity and Ambient Light Sensor  
- L298N Motor Driver Module  
- 2 DC Motors (with wheels and chassis)  
- Li-ion Battery (7.4V recommended) or 5V regulated power  
- Jumper wires and breadboard (optional)

---

## 🔌 Wiring Overview

| Component       | ESP32-CAM Pin |
|----------------|---------------|
| HC-SR04 Trigger| GPIO 12       |
| HC-SR04 Echo   | GPIO 4        |
| APDS-9930 SDA  | GPIO 16       |
| APDS-9930 SCL  | GPIO 0        |
| L298N IN1      | GPIO 13       |
| L298N IN2      | GPIO 15       |
| L298N IN3      | GPIO 14       |
| L298N IN4      | GPIO 2        |

> ⚠️ Use 5V logic level for ESP32-CAM carefully. Double-check wiring before powering up.

---

## 🚗 Features

- Real-time video streaming via ESP32-CAM
- Obstacle detection using HC-SR04 ultrasonic sensor
- Light and proximity sensing using APDS-9930
- DC motor control using L298N driver
- Automatic navigation logic based on sensor input

---

## 🛠️ Project Steps

### 1. Assemble the Car Chassis
- Mount the motors and wheels
- Attach ESP32-CAM, L298N, and battery on the frame

### 2. Connect the Circuit
- Wire sensors and motor driver to ESP32-CAM as per the table above
- Connect motor power (7.4V–12V) to L298N’s `VCC`, `GND`, and `5V OUT` if needed

### 3. Upload the Code
- Connect ESP32-CAM to PC via FTDI USB-to-Serial (IO0 to GND to enter flash mode)
- Use Arduino IDE and select `AI Thinker ESP32-CAM` board
- Upload code with:
  - Camera stream
  - Motor control
  - Sensor integration (HC-SR04 + APDS-9930)

### 4. Test Individual Modules
- Test motor movement using simple digitalWrite test
- Check camera stream via local IP
- Read distance from ultrasonic sensor
- Read ambient light/proximity values from APDS-9930

### 5. Integrate and Tune Logic
- Avoid obstacles when distance < 20cm
- Adjust behavior if light < threshold
- Optional: Send values to Serial Monitor for debugging

---

## 📦 Libraries Required

Install via Arduino IDE > Library Manager:
- `Wire.h` (built-in)
- `Adafruit APDS9930` or equivalent
- `NewPing.h` (or custom ultrasonic code)
- `ESP32 WebServer` (included with ESP32 board package)

---

## 🌐 Future Improvements

- Add remote control via smartphone (WiFi/Web UI)
- AI image recognition (face/object detection)
- Battery level monitoring
- Data logging to Firebase or cloud IoT platform

---

## 📸 Preview

![Project Image]([https://example.com/smart-car-image.jpg](https://www.iotzone.vn/wp-content/uploads/2024/03/cach-su-dung-esp32-tim-hieu-so-do-cahn.jpg)) <!-- Replace with actual image URL -->

---

## 🧠 Author

Project by [thaivanhoa37]  
Date: May 2025  
Platform: ESP32-CAM with Arduino IDE  
Language: C++

---
