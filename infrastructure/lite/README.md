# ⚡ Slate v2.1 Lite Edition

**For low-power laptops or quick prototyping.**

The Lite Edition removes the heavy Database and Metrics engine.
Instead, it runs **Node-RED**, a lightweight visual tool.

## 🚀 How to Start

1. Open VS Code Command Palette (`Cmd+Shift+P` / `F1`).
2. Run Task: **`🚀 Start Lite Cloud`**.

## 📊 How to Visualize

1. Open [http://localhost:1880](http://localhost:1880).
2. Drag an **"mqtt in"** node from the left.
   - Server: `mosquitto`
   - Topic: `slate/freezer/v2/telemetry`
3. Drag a **"debug"** node and connect them.
4. Click **Deploy** (Top Right).
5. Look at the **Debug Sidebar** to see live data streaming in.

## ⚠️ Limitations

- **No History:** If you refresh the page, the data is gone.
- **No Database:** You cannot query "What was the temp last night?"
