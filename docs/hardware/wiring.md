---
sidebar_position: 1
id: wiring
title: 🔌 Wiring Guide
---

# Hardware Connection

### The Schematic

```mermaid
graph LR
    subgraph "Slate v2.1 (ESP32)"
        3V3[3.3V Output]
        GND_ESP[GND]
        D21[GPIO 21 (SDA)]
        D22[GPIO 22 (SCL)]
    end

    subgraph "BME280 Sensor"
        VCC[VCC / VIN]
        GND_SEN[GND]
        SDA[SDA]
        SCL[SCL]
    end

    3V3 ==>|Red Wire| VCC
    GND_ESP ==>|Black Wire| GND_SEN
    D21 -->|Blue Wire| SDA
    D22 -->|Green Wire| SCL

    style 3V3 fill:#ff9999,stroke:#333
    style GND_ESP fill:#999,stroke:#333
    style D21 fill:#99ccff,stroke:#333
    style D22 fill:#99ff99,stroke:#333
```
