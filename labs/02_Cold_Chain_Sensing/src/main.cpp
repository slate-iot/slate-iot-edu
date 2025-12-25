/**
 * ❄️ SLATE v2.1: LAB 02 - COLD CHAIN MONITOR
 * -----------------------------------------------------------------------------
 * Objective: Read BME280 sensor and send data to the Local Cloud.
 * * TODO FOR STUDENTS:
 * 1. Initialize the BME280 Sensor in setup().
 * 2. Read Temperature in loop().
 * 3. Format the JSON data packet.
 * -----------------------------------------------------------------------------
 */

#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <Wire.h>

// =============================================================================
// CONFIGURATION (Edit these)
// =============================================================================
const char* WIFI_SSID = "Wokwi-GUEST";  // Use "Wokwi-GUEST" for simulator
const char* WIFI_PASSWORD = "";         // No password for simulator

// The Docker Computer's IP (Use "localhost" or specific IP if on real hardware)
// For Wokwi Gateway, use your computer's local IP (e.g., 192.168.1.50)
const char* MQTT_SERVER = "192.168.1.100";
const int MQTT_PORT = 1883;

const char* MQTT_TOPIC = "slate/freezer/v2/telemetry";
const char* DEVICE_ID = "Freezer-01";

// =============================================================================
// OBJECTS
// =============================================================================
WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_BME280 bme;  // I2C Sensor

// =============================================================================
// HELPER FUNCTIONS
// =============================================================================
void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "SlateClient-";
        clientId += String(random(0xffff), HEX);

        // Attempt to connect
        if (client.connect(clientId.c_str())) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

// =============================================================================
// MAIN SETUP
// =============================================================================
void setup() {
    Serial.begin(115200);
    setup_wifi();
    client.setServer(MQTT_SERVER, MQTT_PORT);

    // -------------------------------------------------------------------------
    // STUDENT TODO: Initialize Sensor
    // Hint: if (!bme.begin(0x77)) { ... }
    // -------------------------------------------------------------------------
    Serial.println("Initializing Sensor...");
    if (!bme.begin(0x77)) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    Serial.println("Sensor Ready.");
}

// =============================================================================
// MAIN LOOP
// =============================================================================
void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    // Send data every 5 seconds
    static unsigned long lastMsg = 0;
    unsigned long now = millis();

    if (now - lastMsg > 5000) {
        lastMsg = now;

        // ---------------------------------------------------------------------
        // STUDENT TODO: Read Data & Publish
        // ---------------------------------------------------------------------

        // 1. Read Data
        float temp = bme.readTemperature();
        float hum = bme.readHumidity();

        // 2. Format JSON
        // {"deviceId": "Freezer-01", "temperature": -12.5, "humidity": 60.0}
        StaticJsonDocument<200> doc;
        doc["deviceId"] = DEVICE_ID;
        doc["temperature"] = temp;
        doc["humidity"] = hum;

        char buffer[256];
        serializeJson(doc, buffer);

        // 3. Publish
        Serial.print("Publishing: ");
        Serial.println(buffer);
        client.publish(MQTT_TOPIC, buffer);
    }
}