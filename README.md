# 🚗 ESP32-CAM Smart Car with Obstacle Avoidance and Live Streaming

This project features a smart car controlled by an ESP32-CAM board. The car supports live video streaming, remote direction control via a web interface (using Gradio), and autonomous obstacle avoidance using an ultrasonic sensor. The Arduino Uno handles motor control, LED feedback, and sensor integration, communicating with the ESP32-CAM via Serial.

---

## 📷 Features

- Live video stream over Wi-Fi (ESP32-CAM at `/stream`)
- Remote directional control via web interface (forward, backward, left, right, stop) with temporary movement (0.5 seconds per command)
- Automatic obstacle detection and avoidance using ultrasonic sensor (HC-SR04)
- Visual feedback using green, red, and blue LEDs
- Simple serial communication between ESP32-CAM and Arduino Uno (motor control)

---

## 🛠 Hardware Requirements

| Component          | Quantity |
|--------------------|----------|
| ESP32-CAM (AI Thinker) | 1        |
| Arduino Uno (or Nano) | 1        |
| L298N Motor Driver     | 1        |
| Ultrasonic Sensor (HC-SR04) | 1        |
| TT Motors (with wheels) | 2 (or 4) |
| LEDs (Red + Green + Blue) | 1 each   |
| Jumper wires + Battery pack | -        |

---

## ⚙️ Wiring Overview

### ESP32-CAM
- Connect to computer via FTDI or USB-UART for programming.
- Streams video on `http://<ESP_IP>:81/stream`.
- Sends single-letter commands to Uno via Serial:
  - `F` = Forward (move forward)
  - `B` = Backward (move backward)
  - `L` = Turn Left
  - `R` = Turn Right
  - `S` = Stop
- **Serial Connection**: 
  - TX (GPIO 1) to Arduino RX (pin 0).
  - GND to GND (shared with Arduino Uno).
  - **Important**: Add a **voltage divider** (1kΩ from TX to RX, 2kΩ from RX to GND) or a **level shifter** to convert 3.3V (ESP32-CAM) to 5V (Uno).

### Arduino Uno to L298N (motor control)
| Arduino Pin | L298N Pin |
|-------------|-----------|
| D8          | IN1       |
| D9          | IN2       |
| D10         | IN3       |
| D11         | IN4       |
| GND         | GND (shared with ESP32-CAM) |

- Motors:
  - Left motor connected to OUT1/OUT2.
  - Right motor connected to OUT3/OUT4.
- Enable jumpers (ENA, ENB) on L298N for full speed.

### HC-SR04 Ultrasonic Sensor
| Arduino Pin | Sensor Pin |
|-------------|-------------|
| A0          | Trig        |
| A1          | Echo        |
| VCC         | 5V          |
| GND         | GND         |

### LEDs
| Arduino Pin | Color  |
|-------------|--------|
| D3          | Red    |
| D4          | Green  |
| D5          | Blue   |
| (Anode)     | GND (via 220Ω resistor) |

---

## 💻 Software

### ESP32-CAM Code (Arduino)
- File: `esp32_cam_control.ino`
- Handles camera initialization.
- Sets up a web server for video streaming (`/stream`) and movement control (`/move?dir=F|B|L|R|S`).
- Sends command characters over `Serial` to Uno at 9600 baud.

### Arduino Uno Code
- File: `arduino_uno_control.ino`
- Receives commands from ESP32-CAM via Serial.
- Controls motor driver via IN1–IN4.
- Measures distance via HC-SR04 and overrides commands if an obstacle is detected (< 20cm).
- When obstacle detected: moves backward (0.5s), turns right 90°, moves forward (0.5s), then stops.
- Temporary movement: Each command (F, B, L, R) makes the car move for 0.5 seconds before stopping.
- LEDs:
  - Red LED: On when stopped.
  - Green and Blue LEDs: On when moving.

### Web Interface (Gradio)
- File: `web_control.py`
- Provides a web interface to control the car and view the live stream.
- Buttons: Forward, Backward, Left, Right, Stop, Dancing Mode.
- Dancing Mode: Performs a sequence of movements (spin 360°, backward, forward, stop).

---

## 📡 How to Use

1. **Flash the Boards**:
   - Flash the ESP32-CAM with `esp32_cam_control.ino` using Arduino IDE.
   - Flash the Arduino Uno with `arduino_uno_control.ino`. **Disconnect the TX wire** from ESP32-CAM to Uno RX (pin 0) while uploading.
   - Reconnect the TX wire after uploading.

2. **Power Up**:
   - Power the ESP32-CAM (5V/3.3V via USB or external source).
   - Power the Arduino Uno (5V via USB or external source).
   - Power the motors via a battery pack connected to the L298N (e.g., 7.4V LiPo or 4xAA batteries).
   - Ensure a **common GND** between ESP32-CAM, Uno, and L298N.

3. **Connect to Wi-Fi**:
   - Update the `ssid` and `password` in `esp32_cam_control.ino` to match your Wi-Fi network.
   - Upload the code to ESP32-CAM and open the Serial Monitor (9600 baud) to get the IP address (e.g., `192.168.137.xxx`).

4. **Run the Web Interface**:
   - Update the `ESP32_IP` in `web_control.py` with the IP address of your ESP32-CAM (e.g., `http://192.168.137.xxx`).
   - Run the script: `python web_control.py`.
   - Open the provided Gradio URL in a browser to access the control interface and live stream.

5. **Control the Car**:
   - Use the buttons on the web interface:
     - **Forward**: Moves forward for 0.5 seconds.
     - **Backward**: Moves backward for 0.5 seconds.
     - **Left**: Turns left for 0.5 seconds.
     - **Right**: Turns right for 0.5 seconds.
     - **Stop**: Stops immediately.
     - **Dancing Mode**: Performs a predefined sequence (spin 360°, backward, forward, stop).
   - The car will automatically avoid obstacles if detected under 20cm by moving backward, turning right 90°, then moving forward.

---

## 📷 Example

![Project Image]([[https://www.iotzone.vn/wp-content/uploads/2024/03/cach-su-dung-esp32-tim-hieu-so-do-cahn.jpg](https://github.com/thaivanhoa37/ESP32-CAM-Smart-Car-with-Obstacle-Avoidance-and-Live-Streaming/blob/main/image/car%20(5).jpg)](https://raw.githubusercontent.com/thaivanhoa37/ESP32-CAM-Smart-Car-with-Obstacle-Avoidance-and-Live-Streaming/refs/heads/main/image/car%20(5).jpg))

---

## 🧠 Author

Project by [thaivanhoa37]  
Date: May 2025  
Platform: ESP32-CAM with Arduino IDE  
Language: C++ (Arduino), Python (Gradio)

---

## 📂 Project Files

- `esp32_cam_control.ino`: ESP32-CAM code for video streaming and command handling.
- `arduino_uno_control.ino`: Arduino Uno code for motor control, obstacle avoidance, and LED feedback.
- `web_control.py`: Python script for the Gradio web interface.

---

## 🔧 Troubleshooting

1. **Serial Communication Issues**:
   - If Uno receives garbage data, ensure a **level shifter** or **voltage divider** is used between ESP32-CAM TX (3.3V) and Uno RX (5V).
   - Check Serial Monitor logs on both boards for correct command transmission.

2. **Motor Direction Wrong**:
   - If Forward/Backward is reversed, swap the HIGH/LOW logic in `moveForward()` and `moveBackward()` in `arduino_uno_control.ino`.
   - Alternatively, swap the motor wires on OUT1/OUT2 or OUT3/OUT4 of the L298N.

3. **Car Doesn't Stop After 0.5 Seconds**:
   - Verify the `moveDuration` value (500ms) in `arduino_uno_control.ino`.
   - Check for delays in the code that might interfere with timing.

4. **Obstacle Avoidance Not Working**:
   - Ensure the HC-SR04 is wired correctly (Trig: A0, Echo: A1).
   - Test the sensor by printing distance values to Serial Monitor.

5. **No Video Stream**:
   - Confirm the ESP32-CAM IP address and port (`http://<IP>:81/stream`).
   - Check Wi-Fi credentials in `esp32_cam_control.ino`.

---

## 📜 License

This project is open-source and available for educational purposes. Feel free to modify and share!

---
