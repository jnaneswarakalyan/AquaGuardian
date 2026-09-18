# AquaGuardian – Smart Water Drinking Reminder System

An automated hydration reminder and intake logging embedded system implemented on the **NXP LPC2148 (ARM7TDMI)** microcontroller using modular **Embedded C**. The system integrates a real-time clock (RTC), an interactive $4\times4$ matrix keypad, an HD44780 16x2 character LCD with custom CGRAM icons, multi-level visual/audible alert actuators, and external interrupt-driven runtime system configuration.

---

## 📌 Problem Statement
Dehydration causes fatigue, reduced cognitive performance, and chronic health issues, particularly for desk workers and individuals who fail to maintain consistent water intake schedules. Traditional smartphone alarms are easily dismissed, forgotten, or muted. AquaGuardian is a dedicated physical desktop embedded device that schedules reminders, tracks consumption progress against a daily goal, monitors compliance, and resets automatically each day without requiring constant smartphone or cloud tethering.

---

## 🎯 Project Objectives
1. Implement accurate hardware-assisted real-world timekeeping (24-hour clock) using on-chip RTC registers.
2. Automate countdown timers with customizable reminder intervals (default: 15 seconds for testing; scalable to hours).
3. Provide visual and audible alerting for hydration intervals with acknowledgment tracking.
4. Support spontaneous manual water consumption logging at any time outside active alarms.
5. Track delinquent hydration compliance by recording missed alert intervals.
6. Provide a user menu via an external interrupt (EINT0) and matrix keypad to configure time, interval, and daily target without device restarts.
7. Reset metrics (glasses consumed, missed counts, goal flag) automatically upon crossing day boundaries.

---

## ⚙️ Technical Highlights
* **Target Architecture:** NXP LPC2148 / LPC2138 (32-bit ARM7TDMI-S Core).
* **Language & Toolchain:** Embedded C, Keil µVision IDE (ARM Compiler toolchain).
* **Simulation:** Proteus VSM (Virtual System Modeling).
* **Hardware RTC Integration:** Hardware-driven calibration and counter synchronization via LPC21xx `PREINT`, `PREFRAC`, `CCR`, and time registers (`SEC`, `MIN`, `HOUR`, `DOM`, `DOW`).
* **Vectored Interrupt Controller (VIC):** Vectored IRQ configuration for External Interrupt 0 (`EINT0`) mapped to priority slots for prompt UI context switching.
* **Peripheral Bus Driving:** Parallel 8-bit bus control for HD44780 character LCD with custom 5x8 pixel CGRAM icon definition for visual fill percentages (0%, 25%, 50%, 75%, 100%).
* **Matrix Keypad Interface:** Row-scanning and column-sensing routines with edge-settling delays and an interactive numeric entry sub-routine featuring backspace and skip logic.
* **Modular Driver Architecture:** Segregated hardware abstraction layer (HAL) comprising delay drivers, LCD routines, keypad scanner, and system macro definitions.

---

## 🧩 Hardware & Software Specifications

### Microcontroller (LPC2148)
* **Core:** 16/32-bit ARM7TDMI-S @ 12–60 MHz
* **Flash / SRAM:** 512 KB on-chip Flash / 32 KB + 8 KB on-chip SRAM
* **Peripherals Used:** GPIO (Port 0 & Port 1), Hardware RTC, Vectored Interrupt Controller (VIC), External Interrupt 0 (`EINT0`)

### Peripheral Components
* **Display:** 16x2 Alphanumeric LCD (HD44780-compatible, driven in 8-bit parallel mode)
* **Input Interfaces:** $4\times4$ Matrix Keypad, 2x Momentary Tactile Push Buttons (Drink Log Button, EINT0 Trigger Button)
* **Actuators:** Active Piezo Buzzer, 3x Status LEDs (Yellow, Green, Red)
* **Passives:** Resistors ($330\,\Omega$ for LEDs, $10\text{ k}\Omega$ pull-up resistors for active-low inputs/keypad columns)

### Development Tools
* **IDE & Compiler:** Keil µVision V4 / V5 (ARM Legacy Toolset)
* **Simulation Environment:** Labcenter Electronics Proteus Design Suite (v7.7+)
* **Programmer (Hardware Deployment):** Flash Magic (NXP In-System Programming over UART0)

---

## 🗂️ Pin Configuration Table

All pin mappings are strictly aligned with `defines.h` and the peripheral driver headers:

| LPC2148 Pin | Signal / Net Name | Direction | Connected Device | Active Logic | Description |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **P0.0** | `BUZZER_PIN` | Output | Active Buzzer | HIGH | Alert tone generation |
| **P0.1** | `EINT0` | Input | Tactile Push Button | LOW (Falling Edge) | Config Menu Interrupt Trigger (needs $10\text{ k}\Omega$ pull-up) |
| **P0.8–P0.15** | `LCD_DATA` (D0–D7) | Output | LCD Data Pins 7–14 | HIGH/LOW | 8-bit Parallel Display Bus |
| **P0.16** | `LCD_RS` | Output | LCD Pin 4 (RS) | Command: 0 / Data: 1 | Register Select |
| **P0.17** | `LCD_RW` | Output | LCD Pin 5 (RW) | Write: 0 | Read/Write Select |
| **P0.18** | `LCD_EN` | Output | LCD Pin 6 (E) | High-to-Low Pulse | Latch Enable Pulse |
| **P0.20** | `SW_PIN` | Input | Tactile Push Button | LOW | Manual Drink Intake Switch (needs $10\text{ k}\Omega$ pull-up) |
| **P0.21** | `LED_Y_PIN` | Output | Yellow LED | HIGH | Reminder Active Visual Indicator |
| **P0.22** | `LED_G_PIN` | Output | Green LED | HIGH | Daily Goal Completed Indicator |
| **P0.23** | `LED_R_PIN` | Output | Red LED | HIGH | Missed Reminder Indicator |
| **P1.16–P1.19** | `ROW0`–`ROW3` | Output | Keypad Pins A, B, C, D | LOW (Scan) | Keypad Row Drivers |
| **P1.20–P1.23** | `COL0`–`COL3` | Input | Keypad Pins 1, 2, 3, 4 | LOW (Sense) | Keypad Column Sensors (needs $10\text{ k}\Omega$ pull-ups) |

---

## 🏗️ Project Architecture
