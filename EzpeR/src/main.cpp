// esp-32 client for Pi-backed e-ink EPUB reader
#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <GxEPD2_BW.h>
#include <ArduinoJson.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "../lib/credentials.hpp"
#include "../lib/consts.hpp"
#include "esp32-hal.h"


enum NavEvent { NAV_NONE, NAV_UP, NAV_DOWN, NAV_LEFT, NAV_RIGHT, NAV_SELECT, NAV_BACK, NAV_MODE_CHANGE, NAV_SLIDER_JUMP };

enum AppMode { MODE_READING, MODE_BOOK_BROWSER, MODE_SETTINGS };

enum EncoderMode { ENCMODE_SCROLL, ENCMODE_BRIGHTNESS, ENCMODE_BOOK_BROWSER, ENCMODE_SETTINGS_NAV };

struct BookEntry {
  String id;
  String title;
  String author;
};

 struct ReaderState {
  String currentBookId = "";
  int currentPage = 0;
  int encoderMode = 0; // index into EncoderMode
  AppMode mode = MODE_READING;
  BookEntry bookList[BOOKLIST_MAX_ENTRIES];
  int bookListCount = 0;
  int bookListSelectedIdx = 0;
};
// ---------------- SCREEN ----------------
GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display(GxEPD2_290_T94_V2(PIN_EPD_CS, PIN_EPD_DC, PIN_EPD_RST, PIN_EPD_BUSY));

// ---------------- STATE ----------------
ReaderState state;
uint8_t pageBuffer[BITMAP_BYTES];
extern String piBaseUrl;



// Set ISR Flags
volatile bool F_Enc1Push = false, F_Enc2Push = false;
volatile int Enc1_D = 0, Enc2_D = 0;
volatile uint32_t lastMs_enc1Push = 0, lastMs_enc2Push = 0;
volatile uint8_t enc1LastAB = 0, enc2LastAB = 0;

// -------------------------- ISR ROUTINE ---------------------------------
// ISRs (debounce then set flag and lastMs)

volatile bool F_JoyPush = false;
volatile uint32_t lastMs_JoyPush = 0;
void IRAM_ATTR isrJoyPush() {
  uint32_t t = millis();
  if (t - lastMs_JoyPush > DEBOUNCE_MS) { F_JoyPush = true; lastMs_JoyPush = t; }
}

void IRAM_ATTR isrEnc1Push()  { uint32_t t = millis(); if (t - lastMs_enc1Push > DEBOUNCE_MS)  { F_Enc1Push = true;  lastMs_enc1Push = t; } }
void IRAM_ATTR isrEnc2Push()  { uint32_t t = millis(); if (t - lastMs_enc2Push > DEBOUNCE_MS)  { F_Enc2Push = true;  lastMs_enc2Push = t; } }

// Decode rotation based on (prevAB<<2 | currAB)
void IRAM_ATTR isrEnc1Change() {
  uint8_t a = digitalRead(PIN_ENC1_A), b = digitalRead(PIN_ENC1_B);
  uint8_t curr = (a << 1) | b;
  uint8_t combined = (enc1LastAB << 2) | curr;
  if (combined == 0b0001 || combined == 0b0111 || combined == 0b1110 || combined == 0b1000) Enc1_D++;
  else if (combined == 0b0010 || combined == 0b1011 || combined == 0b1101 || combined == 0b0100) Enc1_D--;
  enc1LastAB = curr;
}
void IRAM_ATTR isrEnc2Change() {
  uint8_t a = digitalRead(PIN_ENC2_A), b = digitalRead(PIN_ENC2_B);
  uint8_t curr = (a << 1) | b;
  uint8_t combined = (enc2LastAB << 2) | curr;
  if (combined == 0b0001 || combined == 0b0111 || combined == 0b1110 || combined == 0b1000) Enc2_D++;
  else if (combined == 0b0010 || combined == 0b1011 || combined == 0b1101 || combined == 0b0100) Enc2_D--;
  enc2LastAB = curr;
}

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(WIFI_RETRY_DELAY_MS);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("WiFi connected, IP: ");
  Serial.println(WiFi.localIP());
}

// Resolves Pi via mDNS, retries until found. Sets global piBaseUrl on success.
bool discoverPi() {
  if (!MDNS.begin("esp32reader")) {
    Serial.println("mDNS init failed");
    return false;
  }
  Serial.println("Querying mDNS for Pi server...");
  IPAddress piIp = MDNS.queryHost(PI_MDNS_HOSTNAME);
  int attempts = 0;
  while (piIp == IPAddress(0, 0, 0, 0) && attempts < 10) {
    delay(MDNS_QUERY_RETRY_MS);
    piIp = MDNS.queryHost(PI_MDNS_HOSTNAME);
    attempts++;
  }
  if (piIp == IPAddress(0, 0, 0, 0)) {
    Serial.println("Pi not found via mDNS");
    return false;
  }
  piBaseUrl = "http://" + piIp.toString();
  Serial.print("Pi found at: ");
  Serial.println(piBaseUrl);
  return true;
}

// ---------------- JOYSTICK POLLING ----------------
// Tracks which direction is currently "held" so it only fires once per movement
// and requires return-to-center before firing again.
enum JoyDir { JOY_NONE, JOY_UP, JOY_DOWN, JOY_LEFT, JOY_RIGHT };
JoyDir joyHeld = JOY_NONE;
uint32_t lastJoySampleMs = 0;


// ============================================================
// Setup helpers
// ============================================================
// ---------------- setupPins() — replace button setup with joystick ----------------
void setupPins() {
  // REMOVE all PIN_BTN_* pinMode and attachInterrupt calls

  // Joystick push button (replaces center button)
  pinMode(PIN_JOY_PUSH, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_JOY_PUSH), isrJoyPush, FALLING);

  // Encoders unchanged
  pinMode(PIN_ENC1_A, INPUT_PULLUP);   pinMode(PIN_ENC1_B, INPUT_PULLUP);
  pinMode(PIN_ENC1_PUSH, INPUT_PULLUP);
  pinMode(PIN_ENC2_A, INPUT_PULLUP);   pinMode(PIN_ENC2_B, INPUT_PULLUP);
  pinMode(PIN_ENC2_PUSH, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIN_ENC1_PUSH), isrEnc1Push, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC2_PUSH), isrEnc2Push, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC1_A), isrEnc1Change, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC1_B), isrEnc1Change, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC2_A), isrEnc2Change, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC2_B), isrEnc2Change, CHANGE);

  // Joystick ADC pins are input-only, no pinMode needed for ADC1 on ESP32
}

void setupDisplay() {
  display.init(SERIAL_BAUD_RATE);
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
}

// ============================================================
// HTTP — fetch & post helpers
// ============================================================
// URL-encodes a string for safe use in HTTP paths (spaces, hyphens, special chars)
String urlEncode(const String &s) {
  String encoded = "";
  for (int i = 0; i < s.length(); i++) {
    char c = s[i];
    if (isAlphaNumeric(c) || c == '-' || c == '_' || c == '.' || c == '~') {
      encoded += c;
    } else {
      char buf[4];
      snprintf(buf, sizeof(buf), "%%%02X", (unsigned char)c);
      encoded += buf;
    }
  }
  return encoded;
}

// Fetches raw bitmap bytes into pageBuffer. Returns true on success (exact BITMAP_BYTES received).
bool fetchPageBitmap(const String &path) {
  if (piBaseUrl == "") return false;
  HTTPClient http;
  http.begin(piBaseUrl + path);
  http.setTimeout(HTTP_TIMEOUT_MS);
  int httpCode = http.GET();
  if (httpCode != HTTP_CODE_OK) {
    Serial.printf("GET %s failed, code: %d\n", path.c_str(), httpCode);
    http.end();
    return false;
  }
  WiFiClient *stream = http.getStreamPtr();
  int bytes_read = 0;
  uint32_t startMs = millis();
  while (bytes_read < BITMAP_BYTES && (millis() - startMs) < HTTP_TIMEOUT_MS) {
    if (stream->available()) {
      int n = stream->readBytes(pageBuffer + bytes_read, BITMAP_BYTES - bytes_read);
      bytes_read += n;
    }
  }
  http.end();
  if (bytes_read != BITMAP_BYTES) {
    Serial.printf("Bitmap incomplete: got %d bytes out of %d expected bytes\n", bytes_read, BITMAP_BYTES);
    return false;
  }
  return true;
}
 
// Sends a nav event to the Pi (book page or settings menu) and refreshes pageBuffer with the response.
bool postNavEvent(const String &path, const String &jsonBody) {
  if (piBaseUrl == "") return false;
  HTTPClient http;
  http.begin(piBaseUrl + path);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(HTTP_TIMEOUT_MS);
  int httpCode = http.POST(jsonBody);
  bool ok = (httpCode == HTTP_CODE_OK);
  if (!ok) Serial.printf("POST %s failed, code: %d\n", path.c_str(), httpCode);
  http.end();
  return ok;
}
 
// Fetches GET /books, parses JSON array of {id, title, author} into state.bookList.
bool fetchBookList() {
  if (piBaseUrl == "") return false;
  HTTPClient http;
  http.begin(piBaseUrl + "/books");
  http.setTimeout(HTTP_TIMEOUT_MS);
  int httpCode = http.GET();
  if (httpCode != HTTP_CODE_OK) {
    Serial.printf("GET /books failed, code: %d\n", httpCode);
    http.end();
    return false;
  }
  String payload = http.getString();
  http.end();
 
  StaticJsonDocument<MAX_BOOKLIST_JSON_SIZE> doc;
  DeserializationError err = deserializeJson(doc, payload);
  if (err) {
    Serial.printf("Book list JSON parse failed: %s\n", err.c_str());
    return false;
  }
  JsonArray arr = doc.as<JsonArray>();
  state.bookListCount = 0;
  for (JsonObject entry : arr) {
    if (state.bookListCount >= BOOKLIST_MAX_ENTRIES) break;
    state.bookList[state.bookListCount].id = entry["id"].as<String>();
    state.bookList[state.bookListCount].title = entry["title"].as<String>();
    state.bookList[state.bookListCount].author = entry["author"].as<String>();
    state.bookListCount++;
  }
  return state.bookListCount > 0;
}
 
// Fetches GET /last_read -> {book_id, page}. Returns false on 404/empty (no prior session, fresh install, etc).
bool fetchLastReadBook() {
  if (piBaseUrl == "") return false;
  HTTPClient http;
  http.begin(piBaseUrl + "/last_read");
  http.setTimeout(HTTP_TIMEOUT_MS);
  int httpCode = http.GET();
  if (httpCode == 404) {
    Serial.println("No last-read book on Pi (fresh install or cleared)");
    http.end();
    return false;
  }
  if (httpCode != HTTP_CODE_OK) {
    Serial.printf("GET /last_read failed, code: %d\n", httpCode);
    http.end();
    return false;
  }
  String payload = http.getString();
  http.end();
 
  StaticJsonDocument<256> doc;
  DeserializationError err = deserializeJson(doc, payload);
  if (err || !doc.containsKey("book_id") || !doc.containsKey("page")) {
    Serial.println("Last-read response missing book_id/page");
    return false;
  }
  state.currentBookId = doc["book_id"].as<String>();
  state.currentPage = doc["page"].as<int>();
  return true;
}
 
 
 
// Full refresh blit of pageBuffer using double-blank-flush technique (clears SSD1680 ghosting residue).
void blitPageBuffer() {
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
  } while (display.nextPage());
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.drawBitmap(0, 0, pageBuffer, 296, 128, GxEPD_BLACK);
  } while (display.nextPage());
}
 
// Draws the book list locally (not Pi-rendered — simple text list, no need for the Pillow pipeline).
void renderBookBrowser() {
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
  } while (display.nextPage());
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    int y = 16;
    for (int i = 0; i < state.bookListCount && i < 7; i++) { // ~7 rows fit at this font size on 128px height
      display.setCursor(4, y);
      if (i == state.bookListSelectedIdx) display.print("> "); else display.print("  ");
      display.print(state.bookList[i].title);
      y += 16;
    }
  } while (display.nextPage());
}
 
 
 
void handleNavEvent(NavEvent ev, int sliderDirection = 0) {
String bookId = urlEncode(state.currentBookId);
  if (state.mode == MODE_SETTINGS) {
    String body;
    switch (ev) {
      case NAV_UP:     body = "{\"event\":\"up\"}"; break;
      case NAV_DOWN:   body = "{\"event\":\"down\"}"; break;
      case NAV_SELECT: body = "{\"event\":\"select\"}"; break;
      case NAV_BACK:
        state.mode = MODE_READING;
        if (fetchPageBitmap("/books/" + bookId + "/page/" + String(state.currentPage))) blitPageBuffer();
        return;
      default: return;
    }
    if (postNavEvent("/menu/settings/nav", body)) blitPageBuffer();
    return;
  }
 
  if (state.mode == MODE_BOOK_BROWSER) {
    switch (ev) {
      case NAV_UP:
        if (state.bookListSelectedIdx > 0) { state.bookListSelectedIdx--; renderBookBrowser(); }
        break;
      case NAV_DOWN:
        if (state.bookListSelectedIdx < state.bookListCount - 1) { state.bookListSelectedIdx++; renderBookBrowser(); }
        break;
      case NAV_SELECT: {
        BookEntry &chosen = state.bookList[state.bookListSelectedIdx];
        if (fetchPageBitmap("/books/" + urlEncode(chosen.id) + "/page/0")) {
          state.currentBookId = chosen.id;
          state.currentPage = 0;
          state.mode = MODE_READING;
          blitPageBuffer();
        }
        break;
      }
      case NAV_BACK:
        state.mode = MODE_READING;
        if (fetchPageBitmap("/books/" + urlEncode(state.currentBookId) + "/page/" + String(state.currentPage))) blitPageBuffer();
        break;
      default: break;
    }
    return;
  }
 
  // MODE_READING
  switch (ev) {
    case NAV_RIGHT:
      if (fetchPageBitmap("/books/" + urlEncode(state.currentBookId) + "/page/" + String(state.currentPage) + "/next")) {
        state.currentPage++;
        blitPageBuffer();
      }
      break;
    case NAV_LEFT:
    if (state.currentPage <= 0) break; 
      if (fetchPageBitmap("/books/" + urlEncode(state.currentBookId) + "/page/" + String(state.currentPage) + "/prev")) {
        state.currentPage--;
        blitPageBuffer();
      }
      break;
    case NAV_SELECT: // center button opens settings directly from reading mode
      state.mode = MODE_SETTINGS;
      if (fetchPageBitmap("/menu/settings")) blitPageBuffer();
      break;
    case NAV_MODE_CHANGE:
      state.encoderMode = (state.encoderMode + 1) % ENCODER_MODE_COUNT;
      Serial.printf("Encoder mode -> %d\n", state.encoderMode);
      break;
    case NAV_SLIDER_JUMP:
      // Slider acts as scrollbar: jumps +/-2 pages, not infinite scroll
      if (sliderDirection > 0) {
        for (int i = 0; i < 2; i++) fetchPageBitmap("/books/" + urlEncode(state.currentBookId) + "/page/" + String(state.currentPage) + "/next");
        state.currentPage += 2;
      } else {
        for (int i = 0; i < 2; i++) fetchPageBitmap("/books/" + urlEncode(state.currentBookId) + "/page/" + String(state.currentPage) + "/prev");
        state.currentPage -= 2;
      }
      blitPageBuffer();
      break;
    default:
      break;
  }
}

void pollJoystick() {
  uint32_t now = millis();
  if (now - lastJoySampleMs < SLIDER_SAMPLE_MS) return; // reuse sample rate
  lastJoySampleMs = now;

  int x = analogRead(PIN_JOY_X); // ~2048 at center on ESP32 12-bit ADC
  int y = analogRead(PIN_JOY_Y);
  int dx = x - 2048;
  int dy = y - 2048;

  // Check if joystick has returned to center (releases the held direction)
  if (joyHeld != JOY_NONE) {
    if (abs(dx) < JOY_DEADZONE && abs(dy) < JOY_DEADZONE) {
      joyHeld = JOY_NONE;
    }
    return; // don't fire again until released
  }

  // Determine dominant axis and direction, fire once
  if (abs(dx) > abs(dy)) {
    if (dx > JOY_THRESHOLD)       { joyHeld = JOY_RIGHT; handleNavEvent(NAV_RIGHT); }
    else if (dx < -JOY_THRESHOLD) { joyHeld = JOY_LEFT;  handleNavEvent(NAV_LEFT);  }
  } else {
    // Y axis: depends on your physical wiring — flip if inverted
    if (dy > JOY_THRESHOLD)       { joyHeld = JOY_DOWN;  handleNavEvent(NAV_DOWN);  }
    else if (dy < -JOY_THRESHOLD) { joyHeld = JOY_UP;    handleNavEvent(NAV_UP);    }
  }
}


 
// Interprets encoder delta according to current encoderMode. Mode list is example-only per spec, not finalized.
void handleEncoderDelta(int encoderId, int delta) {
  if (delta == 0) return;
 
  switch (state.encoderMode) {
    case ENCMODE_SCROLL:
      // In reading mode: treat like left/right page turns. In book browser: like up/down.
      if (state.mode == MODE_READING) {
        handleNavEvent(delta > 0 ? NAV_RIGHT : NAV_LEFT);
      } else if (state.mode == MODE_BOOK_BROWSER) {
        handleNavEvent(delta > 0 ? NAV_DOWN : NAV_UP);
      }
      break;
 
    case ENCMODE_BRIGHTNESS: {
      // Pushes a dither-threshold delta to the Pi's brightness setting (see spec addendum).
      int step = (delta > 0) ? 1 : -1;
      String body = "{\"value\":" + String(step) + "}";
      postNavEvent("/settings/dither_threshold_delta", body);
      break;
    }
 
    case ENCMODE_BOOK_BROWSER:
      if (state.mode != MODE_BOOK_BROWSER) {
        state.mode = MODE_BOOK_BROWSER;
        fetchBookList();
        state.bookListSelectedIdx = 0;
        renderBookBrowser();
      } else {
        handleNavEvent(delta > 0 ? NAV_DOWN : NAV_UP);
      }
      break;
 
    case ENCMODE_SETTINGS_NAV:
      if (state.mode != MODE_SETTINGS) {
        state.mode = MODE_SETTINGS;
        if (fetchPageBitmap("/menu/settings")) blitPageBuffer();
      } else {
        handleNavEvent(delta > 0 ? NAV_DOWN : NAV_UP);
      }
      break;
  }
 
  Serial.printf("Encoder %d delta: %d (mode %d)\n", encoderId, delta, state.encoderMode);
}
 
// ============================================================
// Slider polling (not interrupt-driven — ADC needs sampling, not edge events)
// ============================================================
 
int lastSliderReading = -1;
uint32_t lastSliderSampleMs = 0;
 
void pollSlider() {
  uint32_t now = millis();
  if (now - lastSliderSampleMs < SLIDER_SAMPLE_MS) return;
  lastSliderSampleMs = now;
 
  int reading = analogRead(PIN_SLIDER_ADC);
  if (lastSliderReading < 0) { lastSliderReading = reading; return; }
 
  int diff = reading - lastSliderReading;
  if (abs(diff) > SLIDER_DEADBAND) {
    handleNavEvent(NAV_SLIDER_JUMP, diff > 0 ? 1 : -1);
    lastSliderReading = reading;
  }
}
 
// ============================================================
// Main flag-polling loop (ISRs only set flags; all logic happens here)
// ============================================================
 
// ---------------- pollInputFlags() — replace button flag checks ----------------
void pollInputFlags() {
  // REMOVE: F_Up, F_Down, F_Left, F_Right, F_Center checks

  // Joystick push → NAV_MODE_CHANGE (was center button)
  if (F_JoyPush) { F_JoyPush = false; handleNavEvent(NAV_MODE_CHANGE); }

  // Encoder pushes → NAV_SELECT (unchanged)
  if (F_Enc1Push) { F_Enc1Push = false; handleNavEvent(NAV_SELECT); }
  if (F_Enc2Push) { F_Enc2Push = false; handleNavEvent(NAV_SELECT); }

  // Encoder rotation (unchanged)
  noInterrupts();
  int d1 = Enc1_D; Enc1_D = 0;
  int d2 = Enc2_D; Enc2_D = 0;
  interrupts();
  if (d1 != 0) handleEncoderDelta(1, d1);
  if (d2 != 0) handleEncoderDelta(2, d2);

  pollSlider();
  pollJoystick(); // add this
}
 
// ============================================================
// Arduino entry points
// ============================================================
 
void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.print("hello");
  delay(5000);
  Serial.print("bye");
  setupPins();
  setupDisplay();
  connectWiFi();  
  Serial.print(discoverPi());
  delay(5000);
 
  // Try auto-resume first (GET /last_read). Falls back to book browser if no prior session.
  if (fetchLastReadBook() && fetchPageBitmap("/books/" + urlEncode(state.currentBookId) + "/page/" + String(state.currentPage))) {
    state.mode = MODE_READING;
    blitPageBuffer();
  } else if (fetchBookList()) {
    state.mode = MODE_BOOK_BROWSER;
    state.bookListSelectedIdx = 0;
    renderBookBrowser();
  } else {
    Serial.println("Neither last-read nor book list reachable — Pi may be offline");
  }
}
 
void loop() {
  pollInputFlags();
  delay(5); // small idle yield, ISRs still fire regardless
}
 

