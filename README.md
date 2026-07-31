# AgroShield

ESP32-CAM based smart farm surveillance node with multi-sensor fusion.

## Features
- ESP32-CAM video streaming (AI-Thinker module)
- PIR motion detection
- Laser-LDR tripwire
- Ultrasonic distance sensing
- Sound sensor
- GSM/SIM800 alert notifications
- Solar power harvesting
- LoRa communication

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
MIT
