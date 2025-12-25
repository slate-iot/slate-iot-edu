# ❄️ Slate v2.1: The Freezer Edition

**Industrial IoT Education Platform**

Welcome to the Slate v2.1 Digital Ecosystem.
This repository converts your computer into a professional IoT Cloud Server and provides the firmware to build a connected Cold Chain Monitoring system.

---

## 🛠️ Phase 0: The Toolbelt (Install These First)

You cannot build a house without a hammer. Install these tools before doing anything else.

### 1. Visual Studio Code (The Editor)

- [Download VS Code](https://code.visualstudio.com/)

### 2. Docker Desktop (The Engine)

- [Download Docker Desktop](https://www.docker.com/products/docker-desktop/)
- **⚠️ CRITICAL:** After installing, open the Docker app and wait until the whale icon stops animating. It must be running in the background.

---

## 🚀 Phase 1: The Zero-Friction Setup

We have automated the configuration for you.

1.  **Open this Folder in VS Code:**
    - File > Open Folder... > Select `slate-iot-v2.1-freezer`.
2.  **Install Recommended Extensions:**
    - A popup will appear: _"This workspace recommends extensions..."_ -> Click **Install**.
    - This installs **PlatformIO** (for code) and **Wokwi** (for simulation).

---

## ☁️ Phase 2: Start Your Local Cloud

Choose the mode that fits your computer.

### Option A: The Professional Stack (Recommended)

- **Best for:** Data Science, Historical Analysis, Modern Laptops.
- **Includes:** Mosquitto (Broker) + InfluxDB (Database) + Grafana (Dashboard).
- **How to Start:**
  1. Press `Cmd+Shift+P` (Mac) or `Ctrl+Shift+P` (Windows).
  2. Select **`Tasks: Run Task`** -> **`🚀 Start Local Cloud`**.
  3. Open **[http://localhost:3000](http://localhost:3000)** (User: `admin` / Pass: `slate`).

### Option B: The Lite Stack (Low RAM)

- **Best for:** Old Laptops, Quick Prototyping.
- **Includes:** Mosquitto (Broker) + Node-RED (Visual Tool).
- **Limitations:** Data disappears when you refresh the page (No Database).
- **How to Start:**
  1. Press `Cmd+Shift+P` (Mac) or `Ctrl+Shift+P` (Windows).
  2. Select **`Tasks: Run Task`** -> **`🚀 Start Lite Cloud (Low RAM)`**.
  3. Open **[http://localhost:1880](http://localhost:1880)**.

---

## 🧪 Phase 3: The Digital Twin (Simulation)

Waiting for your PCB to arrive? Simulate the hardware right now.

### Step A: Activate the Simulator (First Time Only)

1.  In VS Code, press `F1` (or `Cmd+Shift+P`).
2.  Type and select **`Wokwi: Request a new license`**.
3.  Follow the prompts in your browser to authorize.

### Step B: Run the Bridge (Required for Cloud)

1.  Download the **[Wokwi IoT Gateway](https://github.com/wokwi/wokwigw/releases)**.
2.  Unzip and run the `wokwigw` program. Keep the terminal window open!

### Step C: Launch

1.  **Build the Code:** Open `labs/02_Cold_Chain_Sensing/src/main.cpp` and click the Checkmark (✓) icon at the bottom.
2.  **Start Sim:** Press `F1` -> Select **`Wokwi: Start Simulation`**.
3.  Go back to your Dashboard (localhost:3000 or 1880) and watch the data stream!

---

## 📚 Documentation & Theory

- **[Wiring Guide](./docs/hardware/wiring.md):** How to connect the BME280.
- **[Lab Manual](./docs/labs/lab-02.md):** Step-by-step instructions.
- **[Troubleshooting](./docs/troubleshooting.md):** Common fixes for "Connection Refused".

---

## 🔌 Phase 4: The Real Hardware

Ready to flash the physical Slate v2.1 Board?

1.  Plug your Slate Board into USB.
2.  Open `labs/02_Cold_Chain_Sensing/src/main.cpp`.
3.  Click the **Alien Face Icon** (PlatformIO) on the left sidebar.
4.  Click **"Upload and Monitor"** (Right Arrow icon).

---

_Copyright © 2025 Slate IoT Education._
