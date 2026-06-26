#include <Arduino.h>
#define PI_MDNS_HOSTNAME "ebookserver" // resolves to ebookserver.local

// ---------------- DISPLAY PINS (finalized, do not change) ----------------
#define PIN_EPD_MOSI 23
#define PIN_EPD_CLK  18
#define PIN_EPD_CS   5
#define PIN_EPD_DC   2
#define PIN_EPD_RST  15
#define PIN_EPD_BUSY 4

// ---------------- INPUT PINS (defaults — change here if your wiring differs) ----------------
#define PIN_JOY_X      34   // ADC1 — joystick horizontal axis
#define PIN_JOY_Y      35   // ADC1 — joystick vertical axis
#define PIN_JOY_PUSH   26   // digital, replaces center button

#define JOY_THRESHOLD  1800 // ADC counts from center (center ≈ 2048) to trigger
#define JOY_DEADZONE   600  // must return within this of center before re-triggering

// Rotary encoder #1 (A/B/push)
#define PIN_ENC1_A     32
#define PIN_ENC1_B     33
#define PIN_ENC1_PUSH  25

// Rotary encoder #2 (A/B/push)
#define PIN_ENC2_A     19
#define PIN_ENC2_B     21
#define PIN_ENC2_PUSH  22

// Wiper slider potentiometer (ADC1 pin only — ADC2 conflicts with WiFi)
#define PIN_SLIDER_ADC 36  // 36
// Baud Rate
#define SERIAL_BAUD_RATE 115200

// ---------------- TIMING / MISC CONSTS ----------------
#define DEBOUNCE_MS 40
#define ENCODER_MODE_COUNT 4 // scroll / brightness / book-browser / settings-nav — placeholder, not finalized
#define SLIDER_DEADBAND 80   // ADC counts of hysteresis to avoid jitter-triggered page jumps
#define SLIDER_SAMPLE_MS 150
#define HTTP_TIMEOUT_MS 30000
#define WIFI_RETRY_DELAY_MS 500
#define MDNS_QUERY_RETRY_MS 2000
#define BITMAP_BYTES 4736 // 296x128 1bpp, 37 bytes/row * 128 rows
#define BOOKLIST_MAX_ENTRIES 20
#define MAX_BOOKLIST_JSON_SIZE 4096



String piBaseUrl = "";