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
+---------------------------------------+
                |           Power Supply (3.3V)         |
                +-------------------+-------------------+
                                    |
+----------------------+                v                +----------------------+
|     Input Layer      |    +-----------------------+    |    Actuator Layer    |
|                      |    |   LPC2148 / LPC2138   |    |                      |
| [P0.1]  EINT0 Button |--->|                       |--->| [P0.0]  Piezo Buzzer |
| [P0.20] Drink Button |--->|  - VIC & ISR (EINT0)  |--->| [P0.21] Yellow LED   |
| [P1.16..19] KPM Rows |<---|  - Hardware RTC       |--->| [P0.22] Green LED    |
| [P1.20..23] KPM Cols |--->|  - Application Engine |--->| [P0.23] Red LED      |
+----------------------+    +-----------+-----------+    +----------------------+
|
v
+-----------------------+
|     Display Layer     |
| [P0.8..15] LCD Data   |
| [P0.16..18] LCD Ctrl  |
| HD44780 16x2 Display  |
+-----------------------+


---

## 🔄 System Flow & Operating Logic

                    [ Power On / System Reset ]
                                 |
                                 v
                   [ Peripheral Initialization ]
                   - GPIO Direction (IODIR0/1)
                   - Vectored IRQ Setup (EINT0)
                   - RTC Calibration (PREINT/FRAC)
                   - LCD Init & Custom CGRAM Build
                                 |
                                 v
+------------------->> [ MAIN POLLING LOOP ] <==<--------------------+
|                                    |                                           |
|       +----------------------------+----------------------------+              |
|       | Check Date Change          | Manual Drink Pressed?      | 1-Sec Tick?  |
|       v                            v                            v              |
| [ DOM != PreviousDay ]     [ P0.20 Active LOW ]        [ SEC != PreviousSec ]  |
|       |                            |                            |              |
|   YES: Reset daily             YES: WaterTaken++            YES: Decrement     |
|        metrics (Glasses,            Reset countdown              countdown     |
|        Misses, Goal)                Update Dashboard                 |         |
|                                                              Countdown == 0?   |
|                                                                 /         \    |
|                                                            YES /           \ NO|
|                                                               v             v  |
|                                                      [ Reminder Alert ]  [Refresh|
|                                                      - Buzzer/LED ON     Dashboard|
|                                                      - 5s Poll Drink     Line 2]  |
|                                                             |                  |
|                                                    +--------+--------+         |
|                                                    |                 |         |
|                                               [ Drank ]         [ Missed ]     |
|                                                    |                 |         |
|                                              WaterTaken++       MissCount++    |
|                                              Green LED 2s       Red LED/Buzzer |
|                                                                       3s       |
|                                                    +--------+--------+         |
|                                                             |                  |
+-------------------------------------------------------------+------------------+


### Configuration Interrupt Flow (`EINT0`)
1. Pressing the button on `P0.1` triggers an asynchronous external interrupt.
2. `eint0_isr()` marks `config_mode = 1`, clears `EXTINT`, and acknowledges the VIC.
3. The main loop detects `config_mode == 1`, halts alerts, and calls `EnterConfigMode()`.
4. The user interacts via LCD and keypad:
   * Key `'1'`: Edit RTC Time (`HH`, `MM`, `SS`) with numeric bounds validation.
   * Key `'2'`: Edit Reminder Interval (`HH`, `MM`, `SS`).
   * Key `'3'`: Edit Daily Target Glasses (clamped between 1 and 20).
   * Key `'e'`: Confirm / Accept entry.
   * Key `'C'`: Backspace digit.
   * Key `'d'`: Skip current field.
5. On menu completion, the main loop clears the screen and resynchronizes baseline timers.

---

## 🖥️ Module & Function Descriptions

### Driver Layer
* **`delay_us()`, `delay_ms()`, `delay_s()` (`delay.c`):** Calibrated assembly-style busy-wait loops tailored for a standard processor clock.
* **`InitLCD()`, `CmdLCD()`, `CharLCD()`, `StrLCD()`, `U32LCD()` (`lcd.c`):** 8-bit parallel LCD interface managing standard HD44780 command sets, bus strobe timing ($t_{\text{pulse}}$ via `LCD_EN`), string printing, and integer-to-ASCII LCD rendering.
* **`BuildCGRAM()` (`lcd.c`):** Programs custom 5-character pixel arrays into LCD CGRAM (character addresses `0x00` through `0x04`) displaying dynamic glass fill levels.
* **`Init_KPM()`, `keyscan()` (`kpm.c`):** Configures Row pins as outputs and Column pins as inputs. Sequentially drives rows LOW to detect grounding on column inputs, translating coordinate hits via a $4\times4$ lookup table (`KPMLUT`).
* **`ReadNum()` (`kpm.c`):** High-level interactive keypad entry engine allowing real-time character echoing on the LCD, backspacing, and default value preservation upon skip.

### Application Logic Layer (`main.c`)
* **`eint0_isr()`:** Lightweight Vectored Interrupt Service Routine that flags configuration mode requests.
* **`CheckWaterSwitch()`:** Debounces and registers asynchronous intake events, adjusting remaining glass metrics and resetting the interval timer to avoid false reminders.
* **`Reminder()`:** Executes a 5-second active alerting sequence. If acknowledged, logs consumption and flashes the Green LED; if timed out, increments `MissCount` and triggers delinquent alerting (Red LED and buzzer for 3 seconds).
* **`RenderDashboardLine2()`:** Computes hydration progress and prints glasses taken (`W:`), remaining glasses (`R:`), misses (`M:`), and the custom glass fill glyph.
* **`EnterConfigMode()`:** Interactive state sub-system enabling live reconfiguration of RTC time, alert intervals, and hydration goals.

---

## ⚡ How to Build & Simulate

### Step 1: Compilation in Keil µVision
1. Launch **Keil µVision**.
2. Go to **Project** $\rightarrow$ **New µVision Project...** and name it `AquaGuardian`.
3. Select Device: **NXP** $\rightarrow$ **LPC2148** (or **LPC2138**).
4. When prompted *“Copy Startup.s to Project Folder and Add to Project?”*, select **Yes**.
5. Create two logical source folders in the project manager: `Source` and `Headers`.
6. Add the source files to `Source`: `main.c`, `lcd.c`, `kpm.c`, `delay.c`.
7. Go to **Project** $\rightarrow$ **Options for Target...** (or press `Alt + F7`):
   * **Target Tab:** Set Oscillator (MHz) to `12.0`.
   * **Output Tab:** Check **Create HEX File**.
   * **C/C++ Tab:** Under **Include Paths**, browse and add the `include/` directory.
8. Click **Project** $\rightarrow$ **Rebuild All Target Files** (`F7`). Ensure 0 Errors, 0 Warnings. The compiled binary will be generated under `hex/AquaGuardian.hex`.

### Step 2: Simulation Setup in Proteus
1. Open **Proteus ISIS**.
2. Open `proteus/AquaGuardian.pdsprj` or assemble the schematic manually:
   * Place component **`LPC2138`** or **`LPC2148`** (Note: do not use `LPC2124` due to lack of full RTC model support).
   * Place **`LM016L`** (16x2 LCD) and wire Data (`D0`–`D7`) to `P0.8`–`P0.15`, Control (`RS`, `RW`, `E`) to `P0.16`, `P0.17`, `P0.18`.
   * Place **`KEYPAD-SMALLCALC`**. Connect rows to `P1.16`–`P1.19`. Connect columns to `P1.20`–`P1.23`.
   * Place an active **`RESPACK-8`** ($10\text{ k}\Omega$) pulling `P1.20`–`P1.23` up to `POWER` (+3.3V).
   * Wire `BUZZER` to `P0.0`. Wire LEDs with $330\,\Omega$ series resistors to `P0.21` (Yellow), `P0.22` (Green), `P0.23` (Red).
   * Wire two momentary **`BUTTON`** switches to `P0.1` and `P0.20` with individual $10\text{ k}\Omega$ pull-up resistors to `POWER`.
3. Double-click the LPC MCU:
   * Set **Clock Frequency** to `12MHz`.
   * In **Program File**, browse and link `AquaGuardian.hex`.
4. Press the **Play** button at the bottom-left corner to begin simulation.

---

## 🧪 Test Cases & Verification Matrix

| # | Test Scenario | Procedure / Input | Expected System Behavior | Result |
| :- | :--- | :--- | :--- | :--- |
| **TC-01** | Power-On Display | Power applied / system reset | LCD shows current time on Line 1, `W:00 R:08 M:00 [EmptyGlass]` on Line 2. | Pass |
| **TC-02** | Spontaneous Drink | Press `P0.20` button during normal running | `WaterTaken` becomes `1`, `RemainingGlass` becomes `7`, glass icon updates to 25%, countdown resets. | Pass |
| **TC-03** | Hydration Alert | Allow timer to count down to 0 | Yellow LED turns ON, Buzzer sounds, Line 1 displays `DRINK WATER NOW!`. | Pass |
| **TC-04** | Acknowledged Alert | Press `P0.20` within 5 seconds of alert | Buzzer/Yellow LED stop, Green LED flashes for 2 seconds, LCD reads `GOOD JOB! WATER LOGGED`. | Pass |
| **TC-05** | Delinquent Alert | Leave alert unacknowledged for >5 seconds | Yellow LED shuts off, Red LED and Buzzer sound for 3 seconds, LCD reads `REMINDER MISSED! MISS COUNT: 01`. | Pass |
| **TC-06** | Goal Completion | Drink target reached (`WaterTaken >= TotalGlass`) | LCD displays `GOAL COMPLETED! 100% DONE!`, Green LED and Buzzer sound for 3 seconds, normal reminders cease. | Pass |
| **TC-07** | EINT0 Config Mode | Press `P0.1` button during normal running | Alert stops, LCD switches to `CONFIG MODE PRESS 'e' TO SET`. Entering 1, 2, or 3 edits parameters. | Pass |
| **TC-08** | Midnight Reset | RTC time reaches `00:00:00` (Day changes) | `DOM != PreviousDay` triggers: `WaterTaken` and `MissCount` reset to 0, reminders restart. | Pass |

---

## 🔍 Troubleshooting Guide
* **LCD displays black boxes or remains blank:**
  * In Proteus, verify that `VSS` and `VEE` are tied to ground and `VDD` is connected to power.
  * In physical hardware, adjust the $10\text{ k}\Omega$ potentiometer wiper feeding Pin 3 (`VEE`).
* **Proteus Simulation Freezes on Startup:**
  * Ensure the column lines `P1.20`–`P1.23` are connected to a $10\text{ k}\Omega$ pull-up resistor pack. Floating inputs cause `keyscan()` to stall inside an infinite polling loop.
  * Verify that the selected microcontroller is `LPC2138` or `LPC2148`. Older VSM models like `LPC2124` do not simulate the internal RTC hardware.
* **Buzzer or LEDs not responding:**
  * Verify common ground between the microcontroller board and peripheral driver circuits.
  * Ensure active buzzers are used; passive buzzers require an oscillating PWM signal rather than a static logic HIGH.

---

## 🔮 Future Improvements
* **Non-Blocking Finite State Machine:** Transition the blocking `Reminder()` alert delays (`delay_s`) into timer-driven state machine ticks.
* **Non-Volatile Storage (I2C EEPROM):** Integrate external EEPROM (e.g., AT24C08) via I2C to retain intake history across power outages.
* **Power Optimization:** Implement LPC2148 Power-Down modes, waking up the MCU exclusively via 32.768 kHz RTC second interrupts or GPIO wake-up interrupts.
* **Smart Hydration Volume Sensing:** Replace manual tactile buttons with load cells or ultrasonic water-level sensors placed beneath the water container.

---

## 👤 Author & Project Metadata
* **Project Name:** AquaGuardian
* **Domain:** Embedded Systems & Firmware Development
* **Target Controller:** NXP LPC2148 (ARM7TDMI-S)
* **License:** MIT Open Source License
