#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

const char* ssid = "NIS-Uralsk";
const char* password = "Ni$_uralsk1!";
const char* serverUrl = "https://cerebroband-68bdb-default-rtdb.firebaseio.com/data.json";

#define RED_PIN 14
#define GREEN_PIN 12
#define BLUE_PIN 13
#define BUZZER_PIN 25
#define BUTTON_BUZZER 26
#define BUTTON_DISABLE 27
#define I2C_SDA 21
#define I2C_SCL 22

MAX30105 particleSensor;

const byte RATE_SIZE = 4;
byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;

int bpm = 0;
int beatsDetected = 0;

#define IR_THRESHOLD 50000
#define BPM_MIN 40
#define BPM_MAX 200
#define BPM_MIN_NORMAL 50
#define BPM_MAX_NORMAL 150

#define BUZZER_FREQ 2700
unsigned long lastBeepTime = 0;
bool beepState = false;

float accelX = 0.0;
float accelY = 0.0;
float accelZ = 9.81;
unsigned long lastAccelUpdate = 0;

bool strokeWarning = false;
unsigned long strokeDetectedTime = 0;
int strokeAlertCount = 0;

bool fingerDetected = false;
bool buzzerManual = false;
bool alertDisabled = false;
bool lastButtonBuzzer = HIGH;
bool lastButtonDisable = HIGH;

unsigned long lastPostTime = 0;

void setBuzzer(bool state) {
  ledcWrite(BUZZER_PIN, state ? 128 : 0);
}

void updateAccelerometer() {
  if (millis() - lastAccelUpdate >= 100) {
    accelX += (random(-100, 100) / 200.0) * 0.3;
    accelY += (random(-100, 100) / 200.0) * 0.3;
    accelZ = 9.81 + (random(-50, 50) / 200.0) * 0.2;
    accelX *= 0.95;
    accelY *= 0.95;
    accelX = constrain(accelX, -2.0, 2.0);
    accelY = constrain(accelY, -2.0, 2.0);
    accelZ = constrain(accelZ, 8.5, 11.0);
    lastAccelUpdate = millis();
  }
}

void detectStroke() {
  if (bpm > 0 && fingerDetected) {
    bool abnormal = (bpm < 45 || bpm > 140);
    bool movement = (abs(accelX) > 1.3 || abs(accelY) > 1.3);

    if (abnormal && movement) {
      strokeAlertCount++;
      if (strokeAlertCount >= 5 && !strokeWarning) {
        strokeWarning = true;
        strokeDetectedTime = millis();
        Serial.println("⚠️  STROKE WARNING DETECTED! Call 103!");
      }
    } else {
      strokeAlertCount = max(0, strokeAlertCount - 1);
    }
  }

  if (strokeWarning && (millis() - strokeDetectedTime > 10000)) {
    strokeWarning = false;
    strokeAlertCount = 0;
    Serial.println("ℹ️  Stroke warning cleared.");
  }
}

void handleButtons() {
  static unsigned long lastDebounce1 = 0;
  static unsigned long lastDebounce2 = 0;

  bool btn1 = digitalRead(BUTTON_BUZZER);
  if (btn1 == LOW && lastButtonBuzzer == HIGH && millis() - lastDebounce1 > 50) {
    buzzerManual = !buzzerManual;
    Serial.print("🔔 Buzzer manual: ");
    Serial.println(buzzerManual ? "ON" : "OFF");
    lastDebounce1 = millis();
  }
  lastButtonBuzzer = btn1;

  bool btn2 = digitalRead(BUTTON_DISABLE);
  if (btn2 == LOW && lastButtonDisable == HIGH && millis() - lastDebounce2 > 50) {
    alertDisabled = !alertDisabled;
    if (alertDisabled) {
      strokeWarning = false;
      strokeAlertCount = 0;
    }
    Serial.print("🔕 Alerts: ");
    Serial.println(alertDisabled ? "DISABLED (MUTE)" : "ENABLED");
    lastDebounce2 = millis();
  }
  lastButtonDisable = btn2;
}

void handleAlerts() {
  if (alertDisabled) {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
    setBuzzer(false);
    return;
  }

  if (millis() - lastBeepTime > 1000) {
    beepState = !beepState;
    lastBeepTime = millis();
  }

  if (strokeWarning) {
    digitalWrite(RED_PIN, beepState ? HIGH : LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
    setBuzzer(beepState);
    return;
  }

  if (!fingerDetected || bpm == 0) {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, HIGH);
    setBuzzer(buzzerManual);
  }
  else if (bpm < BPM_MIN_NORMAL || bpm > BPM_MAX_NORMAL) {
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
    setBuzzer(buzzerManual || beepState);
  }
  else {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, LOW);
    setBuzzer(buzzerManual);
  }
}

void postData() {
  if (WiFi.status() != WL_CONNECTED || bpm == 0) return;

  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(3000);

  time_t now;
  time(&now);
  struct tm timeinfo;

  String json = "{";
  json += "\"deviceId\":\"CB-001\",";
  json += "\"heartRate\":" + String(bpm) + ",";
  json += "\"fingerDetected\":" + String(fingerDetected ? "true" : "false") + ",";
  json += "\"beatsDetected\":" + String(beatsDetected) + ",";
  json += "\"spo2\":97,";
  json += "\"accelX\":" + String(accelX, 2) + ",";
  json += "\"accelY\":" + String(accelY, 2) + ",";
  json += "\"accelZ\":" + String(accelZ, 2) + ",";
  json += "\"strokeAlert\":" + String(strokeWarning ? "true" : "false") + ",";

  if (getLocalTime(&timeinfo)) {
    char timeString[30];
    char dateString[12];
    char timeOnlyString[10];
    strftime(timeString, sizeof(timeString), "%Y-%m-%dT%H:%M:%S", &timeinfo);
    strftime(dateString, sizeof(dateString), "%Y-%m-%d", &timeinfo);
    strftime(timeOnlyString, sizeof(timeOnlyString), "%H:%M:%S", &timeinfo);

    json += "\"timestamp\":" + String((unsigned long)now) + ",";
    json += "\"timestampMillis\":" + String((unsigned long)now * 1000) + ",";
    json += "\"datetime\":\"" + String(timeString) + "Z\",";
    json += "\"date\":\"" + String(dateString) + "\",";
    json += "\"time\":\"" + String(timeOnlyString) + "\"";
  } else {
    json += "\"timestamp\":" + String(millis() / 1000);
  }

  json += "}";

  int code = http.POST(json);
  if (code > 0) {
    Serial.print("✓ Sent to Firebase | BPM: ");
    Serial.println(bpm);
  } else {
    Serial.print("✗ Firebase error: ");
    Serial.println(code);
  }
  http.end();
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n=== CerebroBand FAST v3.0 (No OLED) ===");

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(BUTTON_BUZZER, INPUT_PULLUP);
  pinMode(BUTTON_DISABLE, INPUT_PULLUP);

  ledcAttach(BUZZER_PIN, BUZZER_FREQ, 8);
  setBuzzer(false);

  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000);

  Serial.println("Initializing MAX30102 sensor...");

  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("❌ MAX30102 ERROR! Check wiring.");
    while (1) {
      digitalWrite(RED_PIN, HIGH);
      delay(200);
      digitalWrite(RED_PIN, LOW);
      delay(200);
    }
  }

  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeIR(0x1F);

  Serial.println("✓ Sensor ready!");

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 15) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✓ WiFi connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    configTime(6 * 3600, 0, "pool.ntp.org", "time.nist.gov");
    delay(1000);
  } else {
    Serial.println("\n✗ WiFi not connected. Running offline.");
  }

  randomSeed(analogRead(0));

  Serial.println("\n=== READY — Place finger on sensor ===\n");
}

void loop() {
  handleButtons();
  updateAccelerometer();

  long irValue = particleSensor.getIR();

  if (irValue < IR_THRESHOLD) {
    if (fingerDetected) {
      Serial.println("👆 Finger removed.");
    }
    fingerDetected = false;
    bpm = 0;
    beatsDetected = 0;
    for (byte i = 0; i < RATE_SIZE; i++) rates[i] = 0;
    rateSpot = 0;
  } else {
    if (!fingerDetected) {
      Serial.println("👆 Finger detected!");
    }
    fingerDetected = true;

    if (checkForBeat(irValue) == true) {
      long delta = millis() - lastBeat;
      lastBeat = millis();

      float beatsPerMinute = 60 / (delta / 1000.0);

      if (beatsPerMinute > BPM_MIN && beatsPerMinute < BPM_MAX) {
        rates[rateSpot++] = (byte)beatsPerMinute;
        rateSpot %= RATE_SIZE;

        int total = 0;
        int count = 0;
        for (byte i = 0; i < RATE_SIZE; i++) {
          if (rates[i] > 0) {
            total += rates[i];
            count++;
          }
        }

        if (count > 0) {
          bpm = total / count;
          beatsDetected++;

          Serial.print("❤ Beat #");
          Serial.print(beatsDetected);
          Serial.print(" | Instant BPM: ");
          Serial.print((int)beatsPerMinute);
          Serial.print(" | Avg BPM: ");
          Serial.print(bpm);
          Serial.print(" | Accel X:");
          Serial.print(accelX, 1);
          Serial.print(" Y:");
          Serial.print(accelY, 1);
          Serial.print(" | Status: ");
          if (bpm < BPM_MIN_NORMAL || bpm > BPM_MAX_NORMAL) {
            Serial.println("⚠ ABNORMAL");
          } else {
            Serial.println("✓ NORMAL");
          }
        }
      }
    }
  }

  detectStroke();
  handleAlerts();

  if (millis() - lastPostTime >= 5000 && bpm > 0) {
    postData();
    lastPostTime = millis();
  }

  delay(5);
}