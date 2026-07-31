# AgroShield
### Smart Farm Surveillance Node with Intruder Alert & Deterrence System

## Features
- ESP32-CAM video streaming (AI module)
- PIR motion detection
- Laser-LDR tripwire
- Ultrasonic distance sensing
- Sound sensor
- GSM/SIM800L alert notifications
- Solar power harvesting

## Files
- `CameraWebServer.ino` — main sketch
- `app_httpd.cpp` — HTTP server / streaming logic
- `camera_index.h` — web UI assets
- `board_config.h` — camera model selection
- `camera_pins.h` — pin mappings per board

## Hardware
AI-Thinker ESP32-CAM module (PSRAM required).

## Setup
1. Install ESP32 board support in Arduino IDE.
2. Select board: AI Thinker ESP32-CAM.
3. Select partition scheme with ≥3MB APP space.
4. Set Wi-Fi credentials in `CameraWebServer.ino`.
5. Flash and open the serial monitor for the device IP.

## License

