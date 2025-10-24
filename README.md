# Smart Kitchen Control and Monitoring via Blynk

This project implements a smart kitchen control and monitoring system using an ESP8266 microcontroller and the Blynk platform. It allows for remote monitoring of temperature, humidity, and air quality, as well as control of various kitchen appliances.

## Features

*   **Remote Monitoring:** Real-time monitoring of temperature, humidity (DHT11), and air quality (MQ135) via the Blynk app.
*   **Human Presence Detection:** PIR sensor to detect movement in the kitchen.
*   **Appliance Control:** Remotely control a fan, light, fridge, and oven via the Blynk app.
*   **Safety Alerts:** Buzzer alarm and forced fan activation for high air quality (poor air) or high temperature thresholds.
*   **Local Display:** OLED screen to show current temperature, humidity, and air quality readings.
*   **WiFi Failover:** Automatically connects to available WiFi networks from a predefined list.

## Hardware Requirements

*   ESP8266 Development Board (e.g., NodeMCU, Wemos D1 Mini)
*   DHT11 Temperature and Humidity Sensor
*   MQ135 Air Quality Sensor
*   PIR Motion Sensor
*   Buzzer
*   4-Channel Relay Module
*   0.96 inch I2C OLED Display (SSD1306)
*   Jumper Wires

## Software Requirements

*   [Arduino IDE](https://www.arduino.cc/en/software)
*   [ESP8266 Core for Arduino](https://github.com/esp8266/Arduino)
*   **Arduino Libraries:**
    *   `Blynk`
    *   `ESP8266WiFi`
    *   `MQ135`
    *   `Adafruit Unified Sensor`
    *   `DHT sensor library`
    *   `Adafruit GFX Library`
    *   `Adafruit SSD1306`

## Pinout

| Component           | ESP8266 Pin |
| :------------------ | :---------- |
| DHT11 Sensor        | D4          |
| MQ135 Sensor        | A0          |
| PIR Motion Sensor   | D3          |
| Buzzer              | D0          |
| Fan Relay           | D5          |
| Light Relay         | D6          |
| Fridge Relay        | D7          |
| Oven Relay          | D8          |
| OLED Display (SDA)  | D2 (GPIO4)  |
| OLED Display (SCL)  | D1 (GPIO5)  |

## Setup and Configuration

1.  **Arduino IDE Setup:**
    *   Install the ESP8266 board package in your Arduino IDE.
    *   Go to **Sketch > Include Library > Manage Libraries...** and install the required libraries listed under [Software Requirements](#software-requirements).

2.  **Blynk App Configuration:**
    *   **Create a Blynk Template:** In the Blynk web dashboard, create a new Template. Note down the `Template ID` and `Template Name`.
    *   **Create Datastreams:** Create the following Virtual Pin Datastreams:
        *   `V1`: Temperature (Double, 0-100)
        *   `V2`: Humidity (Double, 0-100)
        *   `V3`: Air Quality (Double, 0-1000)
        *   `V4`: Buzzer Status (Integer, 0-1)
        *   `V5`: PIR Status (Integer, 0-1)
        *   `V6`: Fan Control (Integer, 0-1)
        *   `V7`: Light Control (Integer, 0-1)
        *   `V8`: Fridge Control (Integer, 0-1)
        *   `V9`: Oven Control (Integer, 0-1)
    *   **Create an Event:** Create an Event named `kitchen_alert` and configure notifications for it. This will be triggered when air quality or temperature thresholds are exceeded.
    *   **Create a Device:** Create a new device using your newly created Template. This will generate an `Auth Token`.
    *   **Configure Mobile App:** In the Blynk mobile app, add a new device using the generated `Auth Token`. Add widgets (e.g., Gauges for sensors, Buttons for relays) and assign them to the corresponding Virtual Pins.

3.  **Code Configuration:**
    *   Open the `Smart_Kicthen_Monitor_via_Blynk.ino` file in the Arduino IDE.
    *   Update the following lines with your Blynk Template ID, Auth Token, and WiFi credentials:
        ```cpp
        #define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID"
        #define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

        const char* ssid1 = "YOUR_WIFI_SSID_1";
        const char* pass1 = "YOUR_WIFI_PASSWORD_1";
        const char* ssid2 = "YOUR_WIFI_SSID_2";
        const char* pass2 = "YOUR_WIFI_PASSWORD_2";
        const char* ssid3 = "YOUR_WIFI_SSID_3";
        const char* pass3 = "YOUR_WIFI_PASSWORD_3";
        // Add more WiFi networks if needed
        ```

4.  **Upload the Firmware:**
    *   Select your ESP8266 board from the **Tools > Board** menu.
    *   Select the correct COM port from the **Tools > Port** menu.
    *   Click the "Upload" button to flash the firmware to your ESP8266.

## How it Works

The ESP8266 connects to your specified WiFi network and then to the Blynk server. It continuously reads data from the DHT11, MQ135, and PIR sensors. This data is sent to the Blynk app for remote monitoring and displayed locally on the OLED screen. Based on predefined thresholds for air quality and temperature, the system can trigger a buzzer alarm and activate the fan for safety. The Blynk app allows users to remotely control the fan, light, fridge, and oven by sending commands to the ESP8266, which then actuates the corresponding relays.

## Contributing

Feel free to contribute to this project by submitting issues or pull requests.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
