# 🚦 Traffic Light Controller (Arduino)

A 4-way traffic light system with safe transitions and serial control.

---

## 📖 Table of Contents
- [Overview](#-overview)
- [Hardware Setup](#-hardware-setup)
  - [Components](#-components-needed)
  - [Wiring Diagram](#-wiring-diagram)
- [Software Setup](#-software-setup)
  - [Uploading Code](#-upload-the-code)
  - [Serial Commands](#-serial-commands)
  - [Python Control](#-python-control-optional)
- [How It Works](#-how-it-works)
- [Troubleshooting](#-troubleshooting)
- [License](#-license)

---

## 🌟 Overview
This project simulates a **4-way traffic light intersection** with:
- 4 lanes (each with Red, Yellow, Green LEDs).
- Opposite lanes synced (Lane 1 ↔ Lane 3, Lane 2 ↔ Lane 4).
- Safe transitions (Yellow before Red).
- Serial command control (from Python or Arduino IDE).
- Emergency stop (all Red).

---

## 🔧 Hardware Setup

### 📦 Components Needed
| Component       | Quantity |
|-----------------|----------|
| Arduino Board   | 1        |
| Red LEDs        | 4        |
| Yellow LEDs     | 4        |
| Green LEDs      | 4        |
| 220Ω Resistors  | 12       |
| Breadboard      | 1        |
| Jumper Wires    | ~20      |

### 🔌 Wiring Diagram
| **Lane** | **Red Pin** | **Yellow Pin** | **Green Pin** |
|----------|------------|---------------|--------------|
| Lane 1   | 2          | 3             | 4            |
| Lane 2   | 5          | 6             | 7            |
| Lane 3   | 11         | 12            | 13           |
| Lane 4   | 8          | 9             | 10           |

**Connection Guide**:
1. Connect all LED cathodes (-) to GND via resistors.
2. Connect anodes (+) to specified pins.

---

## 💻 Software Setup

### ⬆️ Upload the Code
1. Open `TrafficLightController.ino` in Arduino IDE.
2. Select your board (**Tools > Board > Arduino Uno**).
3. Select the correct port (**Tools > Port**).
4. Click **Upload**.

### ⌨️ Serial Commands
Send these commands via **Arduino Serial Monitor (9600 baud)**:

| Command | Action                          |
|---------|---------------------------------|
| `1`     | Lane 1 + opposite (Lane 3) → 🟢 |
| `2`     | Lane 2 + opposite (Lane 4) → 🟢 |
| `3`     | Lane 3 + opposite (Lane 1) → 🟢 |
| `4`     | Lane 4 + opposite (Lane 2) → 🟢 |
| `-1`    | Emergency stop (all 🔴)         |

### 🐍 Python Control (Optional)
```python
import serial
ser = serial.Serial('COM3', 9600)  # Change COM3 to your port
ser.write(b'1')  # Turn Lane 1 green
ser.write(b'-1') # Emergency stop
```
###🔄 How It Works
- Startup: All lanes are 🔴.
- First Command: Selected lane + opposite turn 🟢.
- Next Command:
    - If same lane: No change.
    - If new lane: Current 🟢 → 🟡 (2 sec) → 🔴 → New lane 🟢.

### 🛠 Troubleshooting

| Issue                    | Solution                        |
|--------------------------|---------------------------------|
| LEDs not lighting        | Check resistor connections      |
| Serial not responding    | Verify baud rate (9600)         |
| Wrong lane activation    | Double-check pin mappings       |


### Key Features:
- **Markdown Tabs**: Organized with clear sections.
- **Emoji Visuals** (🟢 🟡 🔴) for intuitive reading.
- **Tables** for wiring/commands.
- **Python snippet** for serial control.
- **Troubleshooting guide**.
