/**
 * =================================================================================
 * PROJECT: Slate IoT v2.1 ("The Clean Slate")
 * ROLE:    Factory Diagnostic & Batch Sampler Firmware
 * =================================================================================
 * 🎓 EDUCATIONAL GOALS:
 * 1. Post-Power-On Self Test (POST): Verify I2C and SPI buses on boot.
 * 2. Power Gating: Use a P-MOSFET (GPIO 25) to kill peripheral power.
 * 3. Deep Sleep: Achieve <20uA consumption by locking GPIO states.
 * 4. LoRa: Broadcast a "Heartbeat" batch to a gateway.
 * =================================================================================
 */

#include <Adafruit_BME280.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_Sensor.h>
#include <LoRa.h>
#include <SPI.h>
#include <Wire.h>

#include "driver/rtc_io.h"  // ESP32 Low-Power Real-Time Clock GPIO Driver

// --- Pin Definitions ---
#define PIN_POWER_GATE 25   // MOSFET Gate: LOW = Power ON, HIGH = Power OFF
#define PIN_RGB_LED 13      // WS2812B Data Line
#define PIN_BATTERY_ADC 35  // Battery Voltage Divider (2M/2M)
#define PIN_LORA_NSS 5      // LoRa Chip Select
#define PIN_LORA_RESET 14   // LoRa Reset
#define PIN_LORA_DIO0 26    // LoRa IRQ

// --- Operational Settings ---
#define FW_VERSION "v3.1-STABLE"
#define MAX_SAMPLES 5    // How many measurements before sleeping
#define LOOP_DELAY 2000  // Gap between measurements (ms)

// Global Objects
Adafruit_NeoPixel strip(1, PIN_RGB_LED, NEO_GRB + NEO_KHZ800);
Adafruit_BME280 bme;
int sample_count = 0;

/**
 * @brief Enters Ultra-Low Power Mode.
 * This function cuts power to the "Island" (sensors/LEDs) and puts
 * the ESP32 into deep sleep.
 */
void goDark() {
    Serial.println("\n\033[1;33m[SLEEP] Max samples reached. Shutting down...\033[0m");

    // 1. Put LoRa into Sleep mode (essential, or it draws ~1mA)
    LoRa.sleep();

    // 2. Drive the MOSFET Gate HIGH to disconnect the 3.3V peripheral rail
    digitalWrite(PIN_POWER_GATE, HIGH);

    // 3. IMPORTANT: Lock the GPIO 25 state.
    // Normally, GPIOs float during sleep. This "holds" the HIGH signal
    // so the MOSFET stays closed while the CPU is off.
    rtc_gpio_hold_en((gpio_num_t)PIN_POWER_GATE);

    // 4. Turn off the LED
    strip.setPixelColor(0, 0, 0, 0);
    strip.show();

    // 5. Setup Wake-up: Pressing BOOT (GPIO 0) brings us back to setup()
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, 0);

    Serial.println("[SLEEP] CPU Halted. See you after the next Wakeup.");
    Serial.flush();
    esp_deep_sleep_start();
}

void setup() {
    Serial.begin(115200);

    // --- RECOVERY FROM SLEEP ---
    // If we just woke up, the Power Gate is still "Locked" HIGH.
    // We must release the lock so we can drive it LOW to turn on sensors.
    if (rtc_gpio_is_valid((gpio_num_t)PIN_POWER_GATE)) {
        rtc_gpio_hold_dis((gpio_num_t)PIN_POWER_GATE);
    }

    // --- SENSOR WAKEUP ---
    pinMode(PIN_POWER_GATE, OUTPUT);
    digitalWrite(PIN_POWER_GATE, LOW);  // Turn the MOSFET ON (Connects power)

    // Give the BME280 and other chips 300ms to stabilize after power-up
    delay(300);

    strip.begin();
    strip.setBrightness(40);  // Keep brightness low to keep current draw low

    Serial.println("\033[2J\033[H\033[1;36m[POST] Starting Hardware Self-Test...\033[0m");

    // Check I2C Sensor
    if (!bme.begin(0x77)) {
        Serial.println("\033[1;31m[FAIL] BME280 not found! Check I2C soldering.\033[0m");
        strip.setPixelColor(0, 255, 0, 0);  // Solid Red
        strip.show();
        while (1);  // Halt
    }

    // Check SPI Radio
    LoRa.setPins(PIN_LORA_NSS, PIN_LORA_RESET, PIN_LORA_DIO0);
    if (!LoRa.begin(868E6)) {  // Set to 915E6 if in US
        Serial.println("\033[1;31m[FAIL] LoRa Radio failed! Check SPI soldering.\033[0m");
        strip.setPixelColor(0, 255, 0, 0);  // Solid Red
        strip.show();
        while (1);  // Halt
    }

    Serial.println("\033[1;32m[PASS] All systems nominal. Starting loop.\033[0m");
}

void loop() {
    // Check if it's time to sleep
    if (sample_count >= MAX_SAMPLES) {
        goDark();
    }

    // Read Data
    float t = bme.readTemperature();
    // Battery calculation: raw ADC * (3.3V / 4095 levels) * 2 (for 2M/2M divider)
    float v = (analogRead(PIN_BATTERY_ADC) / 4095.0) * 3.3 * 2.0;

    Serial.printf("[BATCH %d/%d] Temp: %.1f°C | Battery: %.2fV\n", sample_count + 1, MAX_SAMPLES, t,
                  v);

    // Indicator: Short Blue blip
    strip.setPixelColor(0, 0, 0, 150);
    strip.show();

    // Broadcast via LoRa
    LoRa.beginPacket();
    LoRa.printf("ID:SLATE-01,T:%.2f,V:%.2f", t, v);
    LoRa.endPacket();

    delay(50);  // LED on time
    strip.setPixelColor(0, 0, 0, 0);
    strip.show();

    sample_count++;
    delay(LOOP_DELAY);
}