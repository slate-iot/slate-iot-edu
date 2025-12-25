/**
 * =============================================================================
 * SLATE v2.1 - DIGITAL TWIN SIMULATION
 * Description: Connects to Local Cloud via Wokwi IoT Gateway
 * =============================================================================
 */
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_BME280.h>
#include <ArduinoJson.h>

// 1. WOKWI SPECIFIC NETWORK CONFIG
// -----------------------------------------------------------------------------
const char* SSID = "Wokwi-GUEST"; // The virtual simulation network
const char* PASS = "";
// 'host.wokwi.internal' is the magic address that points to your laptop's localhost
const char* MQTT_BROKER = "host.wokwi.internal"; 
const int   MQTT_PORT = 1883;

// 2. OBJECTS
WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_BME280 bme; 

// 3. VARIABLES
unsigned long lastMsg = 0;
#define MSG_INTERVAL 3000

void setup_wifi() {
  delay(10);
  Serial.println("\n[SIM] Connecting to Virtual WiFi...");
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n[SIM] WiFi Connected!");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("[SIM] Connecting to MQTT Broker...");
    String clientId = "SlateTwin-Sim-" + String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println(" Connected!");
    } else {
      Serial.print(" Failed (rc=");
      Serial.print(client.state());
      Serial.println(") Retrying in 5s...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  if (!bme.begin(0x77)) {
    Serial.println("[SIM] Error: BME280 not found in diagram.json");
    while (1);
  }
  setup_wifi();
  client.setServer(MQTT_BROKER, MQTT_PORT);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > MSG_INTERVAL) {
    lastMsg = now;
    
    // Read Virtual Sensor
    float temp = bme.readTemperature();
    float hum = bme.readHumidity();

    // Pack Data
    StaticJsonDocument<200> doc;
    doc["deviceId"] = "slate-digital-twin";
    doc["temperature"] = temp;
    doc["humidity"] = hum;
    doc["battery"] = 100;

    char buffer[256];
    serializeJson(doc, buffer);

    // Send
    client.publish("slate/freezer/v2/telemetry", buffer);
    Serial.print("[SIM] Sent: ");
    Serial.println(buffer);
  }
}