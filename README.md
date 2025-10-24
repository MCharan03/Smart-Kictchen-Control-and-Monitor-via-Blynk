# Smart Kitchen Control and Monitoring via Blynk

This project implements a smart kitchen control and monitoring system using an ESP8266 microcontroller and the Blynk platform.

## Features

- Temperature and Humidity Monitoring (DHT11 sensor)
- Air Quality Monitoring (MQ135 sensor)
- PIR Sensor for human detection
- Control of Fan, Light, Fridge, and Oven via Blynk app
- Buzzer alarm for high air quality or temperature thresholds
- OLED display for local sensor readings

## Setup Instructions

1.  **Blynk Setup:**
    *   Create a new project in the Blynk app.
    *   Obtain your `BLYNK_TEMPLATE_ID` and `BLYNK_AUTH_TOKEN`.
    *   Configure events in Blynk for `kitchen_alert` notifications.

2.  **Arduino IDE Setup:**
    *   Install the ESP8266 board package.
    *   Install the following libraries:
        *   `ESP8266WiFi`
        *   `BlynkSimpleEsp8266`
        *   `MQ135`
        *   `Adafruit Unified Sensor`
        *   `DHT sensor library`
        *   `Adafruit GFX Library`
        *   `Adafruit SSD1306`

3.  **Code Configuration:**
    *   Open `Smart_Kicthen_Monitor_via_Blynk.ino` in the Arduino IDE.
    *   Replace `YOUR_BLYNK_TEMPLATE_ID` and `YOUR_BLYNK_AUTH_TOKEN` with your actual Blynk credentials.
    *   Replace `YOUR_WIFI_SSID_1`, `YOUR_WIFI_PASSWORD_1`, etc., with your WiFi network details.

4.  **Hardware Connections:**
    *   Connect the DHT11 sensor to D4.
    *   Connect the MQ135 sensor to A0.
    *   Connect the PIR sensor to D3.
    *   Connect the Buzzer to D0.
    *   Connect the Fan relay to D5.
    *   Connect the Light relay to D6.
    *   Connect the Fridge relay to D7.
    *   Connect the Oven relay to D8.
    *   Connect the OLED display via I2C (SDA, SCL).

## Usage

Upload the code to your ESP8266 board. Monitor and control your kitchen appliances using the Blynk mobile application.

## Contributing

Feel free to contribute to this project by submitting issues or pull requests.