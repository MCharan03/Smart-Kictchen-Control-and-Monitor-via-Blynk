#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Kitchen"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SPI.h>
#include <Wire.h>
#include "MQ135.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- UPGRADES: RELAY LOGIC AND MQ135 FIX ---
// Define Active-LOW Relay Constants for clarity
#define RELAY_ON    LOW
#define RELAY_OFF   HIGH

// FIX: Initialize MQ135 globally
MQ135 gasSensor = MQ135(A0); 

// Blynk authentication
char auth[] = "YOUR_BLYNK_AUTH_TOKEN";

// List of WiFi networks (Primary, Secondary, Tertiary)
const char* ssid1 = "YOUR_WIFI_SSID_1";
const char* pass1 = "YOUR_WIFI_PASSWORD_1";
const char* ssid2 = "YOUR_WIFI_SSID_2";
const char* pass2 = "YOUR_WIFI_PASSWORD_2";
const char* ssid3 = "YOUR_WIFI_SSID_3";
const char* pass3 = "YOUR_WIFI_PASSWORD_3";

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DHT sensor settings
#define DHTTYPE DHT11
#define DHTPIN D4
DHT dht(DHTPIN, DHTTYPE);

// --- NEW FEATURE: Temperature Alert Threshold ---
#define MAX_TEMP_THRESHOLD 40.0 // Alert if temp (in C) goes above this

// Relay and sensor pin definitions
#define relay_fan D5
#define relay_light D6
#define relay_fridge D7
#define relay_oven D8  
#define buzzer_alarm D0
#define pir_human D3

// Virtual pin states
int fan_state = 0;
int light_state = 0;
int fridge_state = 0;
int oven_state = 0;

// Global Sensor Data Storage
double current_air_quality = 0.0;
double current_temperature = 0.0;
double current_humidity = 0.0;

// --- NEW FEATURE: Notification Flag ---
// Tracks if we've already sent the notification to prevent spam
bool notificationSent = false; 

// Timers
BlynkTimer timer; // UPGRADED: Use BlynkTimer for periodic tasks
unsigned long previousDisplayTime = 0;
unsigned long switchDisplayTime = 3000; // 3 seconds
bool displayMode = true;

// Function to connect to WiFi (tries up to 3 networks)
void connectWiFi() {
  Serial.println("Trying WiFi connections...");
  const char* ssids[] = {ssid1, ssid2, ssid3};
  const char* passes[] = {pass1, pass2, pass3};

  for (int i = 0; i < 3; i++) {
    Serial.print("Connecting to: "); Serial.println(ssids[i]);
    WiFi.begin(ssids[i], passes[i]);
    int retry = 0;
    while (WiFi.status() != WL_CONNECTED && retry < 10) {
      delay(500);
      Serial.print(".");
      retry++;
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nConnected!");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
      return;
    }
    Serial.println("\nFailed, trying next WiFi...");
  }
  Serial.println("All WiFi networks failed! Restarting...");
  ESP.restart();
}

// UPGRADED: Function to read sensors, run safety logic, and report to Blynk
void sendSensorData() {
  // Read all sensor values
  double t = dht.readTemperature();
  double h = dht.readHumidity();
  double aq = gasSensor.getPPM();
  int pir_status = digitalRead(pir_human);

  // UPGRADED: DHT Validation
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return; // Exit function to avoid sending bad data
  }
  
  // Store valid readings globally for the OLED
  current_temperature = t;
  current_humidity = h;
  current_air_quality = aq;

  // --- UPGRADED Safety Logic (Checks Air Quality OR High Temp) ---
  if (current_air_quality > 90 || current_temperature > MAX_TEMP_THRESHOLD) {
    // If air is bad OR temp is too high, turn on alerts:
    digitalWrite(buzzer_alarm, HIGH);
    digitalWrite(relay_fan, RELAY_ON); // Force fan ON

    // Only send notification IF we haven't sent one already
    if (!notificationSent) {
      // In Blynk Console, create an Event with the name "kitchen_alert" and configure notifications for it.
      Blynk.logEvent("kitchen_alert");
      notificationSent = true; // Set the flag so we don't send again
    }
  } else {
    // If everything is normal:
    digitalWrite(buzzer_alarm, LOW);
    // Revert fan to user-controlled state (ORIGINAL LOGIC)
    digitalWrite(relay_fan, fan_state ? LOW : HIGH);
    
    // CRITICAL: Reset the flag so we are ready for the *next* alarm
    notificationSent = false; 
  }

  // Send all data to Blynk
  Blynk.virtualWrite(V1, current_temperature);
  Blynk.virtualWrite(V2, current_humidity);
  Blynk.virtualWrite(V3, current_air_quality);
  Blynk.virtualWrite(V4, digitalRead(buzzer_alarm));
  Blynk.virtualWrite(V5, pir_status);
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Prevent relay flickering: Set relay states to OFF
  digitalWrite(relay_fan, RELAY_OFF);
  digitalWrite(relay_light, RELAY_OFF);
  digitalWrite(relay_fridge, RELAY_OFF);
  digitalWrite(relay_oven, RELAY_OFF);

  // Define pin modes
  pinMode(pir_human, INPUT);
  pinMode(buzzer_alarm, OUTPUT);
  pinMode(relay_fan, OUTPUT);
  pinMode(relay_light, OUTPUT);
  pinMode(relay_fridge, OUTPUT);
  pinMode(relay_oven, OUTPUT);

  // Connect to WiFi
  connectWiFi();

  // Connect to Blynk
  Blynk.config(auth);
  Blynk.connect();
  
  // UPGRADED: Set Blynk Timer to call sendSensorData() every 1 second
  timer.setInterval(1000L, sendSensorData);

  // Restore last known states from Blynk
  Blynk.syncVirtual(V6, V7, V8, V9);
}

// ORIGINAL RELAY LOGIC (Active-LOW)
BLYNK_WRITE(V6) { fan_state = param.asInt(); digitalWrite(relay_fan, fan_state ? LOW : HIGH); }
BLYNK_WRITE(V7) { light_state = param.asInt(); digitalWrite(relay_light, light_state ? LOW : HIGH); }
BLYNK_WRITE(V8) { fridge_state = param.asInt(); digitalWrite(relay_fridge, fridge_state ? LOW : HIGH); }
BLYNK_WRITE(V9) { oven_state = param.asInt(); digitalWrite(relay_oven, oven_state ? LOW : HIGH); }

// --- OLED Display Functions ---
// (These now use the global variables for efficiency)

void updateTempHumidityOLED() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("Temp: ");
  display.setTextSize(2);
  display.print(current_temperature);
  display.setTextSize(1);
  display.print(" C");

  display.setCursor(0, 25);
  display.setTextSize(1);
  display.print("Humidity: ");
  display.setTextSize(2);
  display.print(current_humidity);
  display.print(" %");
  display.display();
}

void updateAirQualityOLED() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Air Quality Index");
  display.setCursor(0, 30);
  display.setTextSize(2);
  display.print(current_air_quality);
  display.setTextSize(1);
  display.println(" PPM");
  display.display();
}

void loop() {
  Blynk.run(); // Run Blynk tasks
  timer.run(); // Run tasks scheduled by BlynkTimer (i.e., sendSensorData)

  // This timer handles switching the OLED display every 3 seconds
  if (millis() - previousDisplayTime >= switchDisplayTime) {
    previousDisplayTime = millis();

    if (displayMode) {
      updateTempHumidityOLED();
    } else {
      updateAirQualityOLED();
    }
    displayMode = !displayMode; // Flip the display mode
  }
}
