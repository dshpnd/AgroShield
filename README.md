# AgroShield
Smart farm surveillance node with intruder alert & deterrence

## Overview
AgroShield is a modular ESP32-CAM-based surveillance node designed for remote farms and gardens. It streams camera video, detects motion and tripwire events, measures distance, and sends alerts via GSM (SIM800L). The project supports solar powering for off-grid deployments.

## Features
- ESP32-CAM video streaming (AI module compatible)
- PIR motion detection
- Laser + LDR tripwire
- Ultrasonic distance sensing (HC-SR04 or compatible)
- Sound sensor (microphone-based trigger)
- GSM/SIM800L alert notifications (SMS / data)
- Solar power harvesting + battery management guidance

## Repository layout
- `CameraWebServer.ino` — main Arduino sketch and configuration
- `app_httpd.cpp` — HTTP server / streaming implementation
- `camera_index.h` — web UI assets (HTML/CSS/JS embedded)
- `board_config.h` — camera model selection and board configs
- `camera_pins.h` — pin mappings per board
- `images/` — place module photos and wiring images here

## Hardware (recommended)
- AI-Thinker ESP32-CAM (with PSRAM recommended for stable streaming)
- SIM800L GSM module (with 4V regulated power and appropriate antenna)
- PIR sensor (HC-SR501 or similar)
- Laser diode module + LDR (or photodiode) for tripwire
- Ultrasonic sensor (HC-SR04 or HY-SRF05)
- Sound sensor module
- LiPo battery (3.7V) + TP4056 charger or solar charge controller (e.g., TP4056 + boost/regulator)
- Solar panel sized per battery + load (example: 6V 5–10W for small setups)

## Wiring & pinout (example for AI-Thinker ESP32-CAM)
- Camera: use the board's built-in camera connector (no external wiring).
- PIR sensor: VCC = 5V (or 3.3V if module supports), GND = GND, OUT -> GPIO 13 (configurable in `board_config.h`)
- Laser diode: VCC = 5V, GND = GND, signal (if present) -> unused or direct power
- LDR: one leg to 3.3V, other leg to a 10k pull-down to GND with junction tapped to ADC pin (e.g., GPIO 34 / ADC1_6)
- Ultrasonic (HC-SR04): TRIG -> GPIO 12, ECHO -> GPIO 14 (use a voltage divider on ECHO to keep it ≤3.3V)
- Sound sensor: OUT -> GPIO 27 (or any available ADC/digital pin)
- SIM800L: VCC -> 4.0V regulator (capable of ≥2A surge), GND -> GND, TX -> ESP32 RX (GPIO 3 / use `Serial1`), RX -> ESP32 TX (GPIO 1 / use `Serial1`) (level shifting recommended)

Note: Exact pins are configurable in `camera_pins.h` and `board_config.h`. Verify your board variant and change pin macros before flashing.

## Images

![AgroShield block diagram]<img width="546" height="509" alt="blockdiagram" src="https://github.com/user-attachments/assets/f8d2e0ef-60c6-41ee-9202-ee89934c923d" />

![AgroShield node front view]<img width="476" height="679" alt="node1" src="https://github.com/user-attachments/assets/6901f69d-2505-4f14-bf42-1b93aacb1728" />


When adding photos, optimize them for web (resize to ≤1600px, compress) and include alt text.

## Software / Setup
1. Install Arduino IDE (or PlatformIO) and add ESP32 board support:
   - In Arduino IDE: File > Preferences > Additional Boards Manager URLs add https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   - Boards Manager: install "esp32 by Espressif Systems"
2. Open `CameraWebServer.ino`.
3. Configure board: Tools > Board: "AI Thinker ESP32-CAM" (or the board matching your module).
4. Set partition scheme to one with ≥3MB APP (e.g., "Huge APP (3MB No OTA)").
5. Edit Wi‑Fi credentials and optional SIM800L settings in `CameraWebServer.ino`:
   - WIFI_SSID, WIFI_PASSWORD
   - SIM800_SERIAL_BAUD, SIM800_PWR_PIN (if used)
6. Verify pin mappings in `camera_pins.h` for your board variant.
7. Connect a USB-to-Serial adapter for flashing (GPIO0 to GND to enter flash mode on some boards).
8. Upload and open Serial Monitor at 115200 baud. The device will print its IP address when connected.

## SIM800L notes & power
- SIM800L requires a stable 4.0V supply capable of up to 2A peak current. Use a LiPo with a proper regulator or a dedicated SIM power supply.
- Add decoupling capacitors and avoid powering SIM800L from the same noisy 5V source without filtering.
- If using SMS alerts: ensure your SIM card has SMS enabled and disable PIN or set PIN in code.

## Web UI
- Visit http://<device-ip> in a browser on the same network.
