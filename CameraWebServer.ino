// Smart Farm Surveillance Node - CameraWebServer.ino
// Modified for smooth streaming + random LED deterrent blinking

#include "esp_camera.h"
#include <WiFi.h>
#include "board_config.h"

// ===========================
// Enter your WiFi credentials
// ===========================
const char *ssid = "**********";
const char *password = "**********";

// ===========================
// LED Blink Task (runs on Core 0)
// ===========================
#if defined(LED_GPIO_NUM)
TaskHandle_t ledTaskHandle = NULL;

void ledBlinkTask(void *pvParameters) {
  // Set LED pin as output (bypass LEDC for simple on/off blink)
  pinMode(LED_GPIO_NUM, OUTPUT);
  digitalWrite(LED_GPIO_NUM, LOW);

  while (true) {
    // Random ON duration: 50ms to 400ms
    int onTime  = random(50, 400);
    // Random OFF duration: 80ms to 800ms
    int offTime = random(80, 800);

    digitalWrite(LED_GPIO_NUM, HIGH);  // LED ON
    vTaskDelay(pdMS_TO_TICKS(onTime));

    digitalWrite(LED_GPIO_NUM, LOW);   // LED OFF
    vTaskDelay(pdMS_TO_TICKS(offTime));
  }
}
#endif

void startCameraServer();

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  // ===========================
  // Camera Configuration
  // Tuned for smooth streaming (lower resolution, higher fps, lower quality = less lag)
  // ===========================
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0       = Y2_GPIO_NUM;
  config.pin_d1       = Y3_GPIO_NUM;
  config.pin_d2       = Y4_GPIO_NUM;
  config.pin_d3       = Y5_GPIO_NUM;
  config.pin_d4       = Y6_GPIO_NUM;
  config.pin_d5       = Y7_GPIO_NUM;
  config.pin_d6       = Y8_GPIO_NUM;
  config.pin_d7       = Y9_GPIO_NUM;
  config.pin_xclk     = XCLK_GPIO_NUM;
  config.pin_pclk     = PCLK_GPIO_NUM;
  config.pin_vsync    = VSYNC_GPIO_NUM;
  config.pin_href     = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn     = PWDN_GPIO_NUM;
  config.pin_reset    = RESET_GPIO_NUM;

  // Increase XCLK to 24MHz for higher sensor frame rate
  config.xclk_freq_hz = 24000000;

  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    // PSRAM available: use 2 frame buffers + GRAB_LATEST to always serve
    // the newest frame — this is the key fix for lag
    config.frame_size  = FRAMESIZE_VGA;    // 640x480 — good balance of quality vs. speed
    config.jpeg_quality = 15;              // 0=best, 63=worst; 15 is a smooth-streaming sweet spot
    config.fb_count    = 2;
    config.grab_mode   = CAMERA_GRAB_LATEST; // discard stale frames — prevents queue buildup
    config.fb_location = CAMERA_FB_IN_PSRAM;
  } else {
    // No PSRAM: must keep resolution small
    config.frame_size  = FRAMESIZE_QVGA;  // 320x240
    config.jpeg_quality = 20;
    config.fb_count    = 1;
    config.grab_mode   = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_DRAM;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed: 0x%x\n", err);
    return;
  }

  // Fine-tune sensor for better low-light / outdoor performance
  sensor_t *s = esp_camera_sensor_get();
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);
    s->set_brightness(s, 1);
    s->set_saturation(s, -2);
  }
  // Set initial streaming frame size (overrides init value for the web UI)
  s->set_framesize(s, FRAMESIZE_VGA);

  // Boost gain ceiling for night-time farm use
  s->set_gainceiling(s, GAINCEILING_4X);
  // Enable auto white balance gain for outdoor conditions
  s->set_awb_gain(s, 1);

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

  // ===========================
  // Start random LED blink task immediately (Core 0, priority 1)
  // ===========================
#if defined(LED_GPIO_NUM)
  xTaskCreatePinnedToCore(
    ledBlinkTask,    // task function
    "LED_Blink",     // task name
    1024,            // stack size (bytes)
    NULL,            // parameters
    1,               // priority
    &ledTaskHandle,  // task handle
    0                // run on Core 0 (camera/web server runs on Core 1)
  );
  Serial.println("LED blink task started");
#endif

  // ===========================
  // WiFi
  // ===========================
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  Serial.print("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  startCameraServer();

  Serial.print("Camera Ready! Open: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/stream  for raw stream");
  Serial.print("          http://");
  Serial.print(WiFi.localIP());
  Serial.println("/  for full control panel");
}

void loop() {
  // Everything runs in tasks / HTTP server — nothing needed here
  delay(10000);
}
