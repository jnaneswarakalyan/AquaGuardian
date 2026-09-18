AquaGuardian -- Smart Water Drinking Reminder System
AquaGuardian is an embedded water-drinking reminder and daily hydration
tracking system developed using the NXP/Philips LPC2148 ARM7
microcontroller.
The system uses the LPC2148 RTC to maintain time and schedule drinking
reminders. It provides LCD, LED and buzzer feedback, allows the user to
log water intake through a push button, tracks the daily target, records
missed reminders, and provides a keypad-based configuration menu.
> **Implementation baseline:** This README documents the current working
> LPC2148 firmware and its actual pin assignments. It is intentionally
> aligned with the working `AquaGuardian-1.c` implementation rather than
> with an alternative pin map or a different firmware architecture.
---
1. Project Overview
Problem
People may forget to drink water regularly during work, study, travel,
or other daily activities. A simple timer is not sufficient because the
system should also allow the user to record intake and monitor progress
toward a daily target.
Solution
AquaGuardian combines:
RTC-based time keeping
Automatic drinking reminders
LCD status display
4×4 keypad configuration
Manual water-intake logging
Daily hydration target
Missed-reminder counting
Yellow, green and red status LEDs
Audible buzzer
External-interrupt-based configuration entry
Automatic daily reset
Main objective
To implement a reliable embedded system that reminds the user to drink
water at configurable intervals and maintains a daily record of consumed
glasses, remaining glasses and missed reminders.
---
2. Main Features
Core features
LPC2148 ARM7 controller
RTC-based one-second time reference
Configurable reminder interval
Configurable daily water target
Manual water logging using a push button
4×4 keypad for configuration and numeric entry
16×2 LCD user interface
Yellow LED for active reminder
Green LED for successful water logging / goal completion
Red LED for missed reminder
Buzzer for audible notification
Daily automatic reset
Input validation
Keypad bounds checking
Button debounce
Demo mode for faster testing
---
3. System Block Diagram
``` text
                         +----------------------+
                         |       LPC2148        |
                         |     ARM7 MCU         |
                         +----------+-----------+
                                    |
          +-------------------------+-------------------------+
          |                         |                         |
          v                         v                         v
      +-------+                 +-------+                 +---------+
      |  RTC  |                 |  LCD  |                 |  Keypad |
      +-------+                 | 16x2  |                 |   4x4   |
          |                     +-------+                 +---------+
          |
          v
   Reminder / Hydration
       Application
          |
    +-----+-----+------------------+
    |           |                  |
    v           v                  v
 Yellow LED  Green LED          Red LED
    |
    v
  Buzzer

 Additional inputs:
    P0.20 -> Manual water switch
    P0.1  -> EINT0 configuration switch
```
---
4. Hardware Components
Component                   Purpose
---
LPC2148                     Main ARM7 microcontroller
16×2 LCD                    Displays time, hydration status and messages
4×4 matrix keypad           Configuration and numeric input
Water push button           Records a glass of water
Configuration push button   Enters configuration mode through EINT0
Yellow LED                  Indicates an active drinking reminder
Green LED                   Indicates successful logging / goal completion
Red LED                     Indicates a missed reminder
Buzzer                      Audible reminder and status indication
Main crystal                LPC2148 processor clock
RTC crystal                 RTC clock source
Power supply                Supplies the controller and peripherals
---
5. Exact LPC2148 Pin Mapping
This is the actual mapping used by the current firmware.
Port 0
LPC2148 signal       Pin Connected device
---
P0.0                P0.0 Buzzer
P0.1                P0.1 EINT0 / Configuration switch
P0.8                P0.8 LCD D0
P0.9                P0.9 LCD D1
P0.10              P0.10 LCD D2
P0.11              P0.11 LCD D3
P0.12              P0.12 LCD D4
P0.13              P0.13 LCD D5
P0.14              P0.14 LCD D6
P0.15              P0.15 LCD D7
P0.16              P0.16 LCD RS
P0.17              P0.17 LCD RW
P0.18              P0.18 LCD EN
P0.20              P0.20 Manual water switch
P0.21              P0.21 Yellow LED
P0.22              P0.22 Green LED
P0.23              P0.23 Red LED
Port 1
LPC2148 signal       Pin Connected device
---
P1.16              P1.16 Keypad Row A
P1.17              P1.17 Keypad Row B
P1.18              P1.18 Keypad Row C
P1.19              P1.19 Keypad Row D
P1.20              P1.20 Keypad Column 1
P1.21              P1.21 Keypad Column 2
P1.22              P1.22 Keypad Column 3
P1.23              P1.23 Keypad Column 4
> **Important:** Do not use a different pin map from an older README or
> an alternate implementation. The firmware must match this table.
---
6. LCD Interface
The firmware uses an 8-bit LCD interface.
``` text
LCD D0 -> P0.8
LCD D1 -> P0.9
LCD D2 -> P0.10
LCD D3 -> P0.11
LCD D4 -> P0.12
LCD D5 -> P0.13
LCD D6 -> P0.14
LCD D7 -> P0.15

LCD RS -> P0.16
LCD RW -> P0.17
LCD EN -> P0.18
```
For the physical/simulation circuit, the LCD power and contrast
connections must also be wired correctly.
Typical LCD power connections:
``` text
VSS -> GND
VDD -> appropriate supply
VEE -> contrast adjustment
```
---
7. Keypad Interface
The firmware uses a 4×4 matrix keypad.
Logical keypad layout
``` text
+---+---+---+---+
| 1 | 2 | 3 | a |
+---+---+---+---+
| 4 | 5 | 6 | b |
+---+---+---+---+
| 7 | 8 | 9 | c |
+---+---+---+---+
| C | 0 | e | d |
+---+---+---+---+
```
Special keys:
``` text
C -> Backspace
e -> Enter / Confirm
d -> Skip / Cancel
```
Keypad wiring
``` text
Rows:
ROW A -> P1.16
ROW B -> P1.17
ROW C -> P1.18
ROW D -> P1.19

Columns:
COL 1 -> P1.20
COL 2 -> P1.21
COL 3 -> P1.22
COL 4 -> P1.23
```
The keypad scanning implementation includes bounds checking before using
the row/column values to index the key lookup table.
---
8. User Inputs
Manual water switch
``` text
P0.20
```
The switch is active LOW.
When pressed, the firmware:
Debounces the input.
Increments `WaterTaken` if the target has not already been reached.
Updates `RemainingGlass`.
Resets the reminder countdown.
Updates the LCD dashboard.
Configuration switch
``` text
P0.1 -> EINT0
```
The EINT0 interrupt requests configuration mode. The actual
configuration work is performed by the main application rather than
doing the full menu operation inside the ISR.
---
9. LED and Buzzer Indications
Yellow LED
Used during an active drinking reminder.
``` text
P0.21 -> Yellow LED
```
Green LED
Used after successful water logging and during goal-completion
indication.
``` text
P0.22 -> Green LED
```
Red LED
Used when a reminder is missed.
``` text
P0.23 -> Red LED
```
Buzzer
``` text
P0.0 -> Buzzer
```
The buzzer is activated during reminder/status events.
---
10. Application Data
The current implementation maintains the following main application
values:
``` text
TotalGlass
WaterTaken
RemainingGlass
MissCount
goal_done
```
Default daily target:
``` text
8 glasses
```
The configured target is constrained to:
``` text
1 to 20 glasses
```
These values are runtime variables in the current implementation. The
current firmware does not implement non-volatile EEPROM/Flash
storage for retaining the target across a complete power cycle.
---
11. Reminder Operation
Production mode
The normal reminder interval is:
``` text
1 hour = 3600 seconds
```
Demo mode
For development and demonstrations:
``` c
#define DEMO_MODE 1
```
sets the reminder interval to:
``` text
15 seconds
```
The production setting is:
``` c
#define DEMO_MODE 0
```
> Use `DEMO_MODE=1` during demonstrations so the reminder can be tested
> without waiting for one hour. Return it to `0` for the intended normal
> operating configuration.
---
12. Reminder Flow
``` text
             RTC running
                  |
                  v
        Countdown reaches zero
                  |
                  v
             Reminder()
                  |
                  v
      +-------------------------+
      | LCD: DRINK WATER NOW!   |
      |       PRESS SWITCH      |
      +-------------------------+
                  |
          Yellow LED + buzzer
                  |
          Wait during alert
             window
             /     \
           YES      NO
            |        |
            v        v
      Water logged  MissCount++
            |        |
       Green LED   Red LED
            |        |
            +---+----+
                |
                v
        Restart countdown
```
Current firmware behavior
The current implementation uses a 5-second reminder alert window.
If the water switch is pressed during this period, the water is logged.
If it is not pressed, the reminder is recorded as missed and the miss
counter is incremented.
---
13. Water Logging
When a user presses the water switch:
``` text
WaterTaken = WaterTaken + 1
RemainingGlass = TotalGlass - WaterTaken
```
The firmware also prevents the consumed count from exceeding the
configured target.
The reminder countdown is restarted after a successful water log.
---
14. Goal Completion
When:
``` text
WaterTaken >= TotalGlass
```
the daily target is considered complete.
The LCD displays a goal-completion message, and the green LED and buzzer
are used for the completion indication.
The system then displays a rest-for-today message and stops normal
reminder processing for the completed day.
---
15. Configuration Mode
The configuration switch is connected to EINT0.
When the switch is activated, the firmware sets the configuration
request flag and the main application enters configuration mode.
Initial configuration screen
``` text
CONFIG MODE
PRESS 'e' TO SET
```
Configuration menu
``` text
1:TIME 2:INT
3:TARGET
```
Option 1 -- Time
The user can enter:
``` text
HH
MM
SS
```
Validation:
``` text
Hour   : 0–23
Minute : 0–59
Second : 0–59
```
Option 2 -- Reminder interval
The user can configure:
``` text
Hours
Minutes
Seconds
```
The values are converted to a total number of seconds for the countdown.
Option 3 -- Daily target
The target can be configured within:
``` text
1–20 glasses
```
Changing the target resets the relevant daily hydration counters
according to the current firmware behavior.
---
16. RTC Operation
The LPC2148 RTC provides the application's time reference.
The firmware configures the RTC prescaler values for the selected
peripheral clock configuration and monitors the seconds value.
When the seconds value changes, the application treats that as a
one-second application tick and updates the reminder countdown.
The RTC also provides date information used for daily reset detection.
---
17. Automatic Daily Reset
The application checks for a change in the RTC day value.
When a new day is detected, the daily runtime statistics are reset:
``` text
WaterTaken     = 0
RemainingGlass = TotalGlass
MissCount      = 0
goal_done      = 0
```
The reminder countdown is restarted and status outputs are cleared.
The configured target itself remains in the runtime configuration.
---
18. Software Architecture
The project has been organized into separate source and header modules.
``` text
AquaGuardian/
│
├── README.md
├── LICENSE
├── .gitignore
│
├── src/
│   ├── main.c
│   ├── app.c
│   ├── lcd.c
│   ├── keypad.c
│   ├── rtc.c
│   ├── gpio.c
│   ├── interrupt.c
│   ├── ui.c
│   ├── hydration.c
│   ├── reminder.c
│   ├── config.c
│   └── delay.c
│
├── inc/
│   ├── aquaguardian.h
│   ├── app.h
│   ├── lcd.h
│   ├── keypad.h
│   ├── rtc.h
│   ├── gpio.h
│   ├── interrupt.h
│   ├── ui.h
│   ├── hydration.h
│   ├── reminder.h
│   ├── config.h
│   └── delay.h
│
├── docs/
│   ├── AquaGuardian_Project_Report.pdf
│   ├── logical_flow.png
│   └── circuit_diagram.png
│
├── tests/
│   └── test_cases.md
│
└── legacy/
    └── AquaGuardian-1.c
```
Module responsibilities
Module             Responsibility
---
`main.c`           Application entry point and main loop
`app.c`            Application-level coordination
`lcd.c`            LCD low-level driver
`keypad.c`         Matrix keypad scanning
`rtc.c`            RTC initialization and time handling
`gpio.c`           GPIO setup and hardware control
`interrupt.c`      EINT0 configuration and ISR
`ui.c`             LCD dashboard and user-interface rendering
`hydration.c`      Water/target/miss state handling
`reminder.c`       Reminder behavior
`config.c`         Configuration menu and input processing
`delay.c`          Delay routines
`aquaguardian.h`   Common project definitions
---
19. Why the Project Is Modular
The original application was developed as a single C source file. It has
been reorganized into functional modules so that:
hardware drivers are separated from application logic;
each module has a clear responsibility;
the project is easier to maintain;
individual functions are easier to test;
the GitHub repository is easier to understand;
future hardware or UI changes can be made with less impact on
unrelated modules.
The original source is retained under `legacy/` as a reference.
---
20. Keil µVision Build
The project is intended to be built using Keil µVision for the
LPC2148 target.
A µVision project consists of targets, groups and source files, and the
target options control compiler, linker, debugger and output
configuration. Keil documents the normal workflow as
compiling/assembling the source files and then linking them into the
executable target.
Important project setting
Because the project headers are stored in:
``` text
inc/
```
the Keil target must have the `inc` directory in its C/C++ include path.
If the `.uvproj`/`.uvprojx` file is in the project root:
``` text
.\inc
```
If the Keil project file is inside a `Keil/` subdirectory:
``` text
..\inc
```
After changing project settings, rebuild all target files.
Expected successful build
A successful build should finish with:
``` text
0 Error(s), 0 Warning(s)
```
and generate the target executable/HEX output according to the Keil
target configuration.
---
21. Hardware Programming
The generated HEX file can be used with the programmer/debugger
appropriate for the LPC2148 hardware board.
Before programming:
Select the correct LPC2148 device in the Keil target.
Verify the target memory configuration.
Verify the startup file matches the LPC2148.
Verify the output HEX option.
Program the controller.
Reset the board.
Observe the LCD and status indicators.
The physical hardware implementation is the primary working reference
for this project.
---
22. Proteus Simulation
The Proteus simulation should reproduce the exact firmware pin mapping.
Minimum required connections
``` text
LPC2148
│
├── LCD
│   ├── D0–D7 -> P0.8–P0.15
│   ├── RS    -> P0.16
│   ├── RW    -> P0.17
│   └── EN    -> P0.18
│
├── Keypad
│   ├── Rows -> P1.16–P1.19
│   └── Cols -> P1.20–P1.23
│
├── Buzzer -> P0.0
├── EINT0 switch -> P0.1
├── Water switch -> P0.20
├── Yellow LED -> P0.21
├── Green LED -> P0.22
└── Red LED -> P0.23
```
Proteus troubleshooting checklist
If the firmware works on physical hardware but not in Proteus, verify
the simulation in this order:
LPC2148 device model is correct.
Main crystal is connected correctly.
RTC crystal is connected correctly if required by the simulation
model.
Reset is correctly wired.
LPC2148 power pins are correctly supplied.
LCD D0--D7 are actually wired to P0.8--P0.15.
LCD RS/RW/EN are wired to P0.16/P0.17/P0.18.
Keypad all four rows and four columns are connected.
Manual switch is connected to P0.20.
Configuration switch is connected to P0.1.
LEDs are connected to P0.21/P0.22/P0.23 with suitable current
limiting.
Buzzer is connected to P0.0.
The correct HEX file is loaded into the LPC2148 Proteus component.
`DEMO_MODE` is set to `1` for fast reminder testing.
Do not change the C pin definitions just to accommodate a different
Proteus wiring arrangement. Make the Proteus schematic match the working
firmware.
---
23. Test and Validation
Functional test cases
---
ID                Test              Procedure         Expected result
---
TC01              Power-up          Apply power/reset LCD and application
initialize
TC02              RTC               Observe time      Time advances
correctly
TC03              Manual water log  Press P0.20       Water count
switch            increases
TC04              Remaining target  Log water         Remaining glasses
decrease
TC05              Reminder          Use demo mode and Reminder message
wait              appears
TC06              Reminder          Press water       Water is logged and
acknowledgement   switch during     green indication
reminder          occurs
TC07              Missed reminder   Do not press      Miss counter
switch            increases and red
indication occurs
TC08              Goal completion   Reach configured  Goal-completed
target            indication appears
TC09              Configuration     Press EINT0       Configuration mode
switch            opens
TC10              Time              Select TIME and   RTC time is updated
configuration     enter valid  
values
TC11              Interval          Select INT        Reminder interval
configuration                       changes
TC12              Target            Select TARGET     Daily target
configuration                       changes
TC13              Invalid time      Enter             Invalid value is
out-of-range      rejected
value
TC14              Keypad backspace  Enter digits and  Last digit is
press C           removed
TC15              Keypad cancel     Press d during    Current operation
applicable input  is
skipped/cancelled
TC16              Daily reset       Advance RTC to a  Daily counters
new day           reset
TC17              Target upper      Enter target      Target is
limit             above allowed     constrained
range
TC18              Target lower      Enter target      Target is
limit             below allowed     constrained
range
---
24. Demonstration Procedure
For a project demonstration, use:
``` c
#define DEMO_MODE 1
```
Then demonstrate the following sequence:
Step 1 -- Power ON
Show:
LCD initialization
current time
hydration dashboard
Step 2 -- Manual water logging
Press the water switch.
Show:
consumed count increasing
remaining count decreasing
dashboard update
Step 3 -- Reminder
Wait approximately 15 seconds.
Show:
`DRINK WATER NOW!`
yellow LED
buzzer
Step 4 -- Acknowledge reminder
Press the water switch.
Show:
`GOOD JOB!`
water logged
green LED
Step 5 -- Miss a reminder
Allow the reminder window to expire.
Show:
missed-reminder message
miss counter
red LED/buzzer indication
Step 6 -- Configuration
Press the EINT0 configuration switch.
Show:
``` text
CONFIG MODE
PRESS 'e' TO SET
```
Then demonstrate:
``` text
1:TIME 2:INT
3:TARGET
```
Step 7 -- Goal completion
Configure a small target for demonstration, or log enough glasses to
reach the configured target.
Show:
``` text
GOAL COMPLETED!
100% DONE!
```
---
25. Current Implementation Notes
The following points are intentionally documented so the README does not
claim functionality that the current firmware does not implement.
Reminder acknowledgement
The current firmware uses a 5-second alert window. It does not keep
the reminder active indefinitely until acknowledgement.
Date configuration
The current configuration menu changes the time (HH/MM/SS). It does
not provide a user menu for editing the complete RTC date.
Hydration percentage
The current LCD dashboard tracks consumed glasses, remaining glasses and
missed reminders and uses a custom glass/progress character. A dedicated
numeric hydration-percentage field is not part of the current dashboard
implementation.
Non-volatile goal storage
The configured target is maintained in runtime variables. The current
implementation does not provide explicit EEPROM/Flash persistence for
the target across a power cycle.
Demo mode
`DEMO_MODE=1` is intended for testing and demonstration. The normal
production configuration is `DEMO_MODE=0`.
These notes keep the documentation consistent with the actual source
code.
---
26. Project Limitations and Future Enhancements
Possible future improvements include:
EEPROM/Flash persistence for the daily target.
Persistent storage of hydration history.
Full RTC date/time configuration.
Numeric hydration percentage display.
Timestamp logging for individual drinking events.
A reminder state that remains active until acknowledgement, if that
behavior is required by the final specification.
Improved non-blocking reminder handling.
Low-power sleep operation between events.
More detailed daily/weekly hydration statistics.
A dedicated Proteus simulation configuration matching the final
hardware schematic.
Automated unit tests for calculation and validation modules.
These are enhancements; they are not claimed as current functionality.
---
27. Development Modes
Normal operation
``` c
#define DEMO_MODE 0
```
Reminder interval:
``` text
3600 seconds
1 hour
```
Demonstration/testing
``` c
#define DEMO_MODE 1
```
Reminder interval:
``` text
15 seconds
```
Only use demo mode when rapid testing is required.
---
28. Repository Structure
``` text
AquaGuardian/
│
├── README.md
├── LICENSE
├── .gitignore
│
├── src/
│   ├── main.c
│   ├── app.c
│   ├── lcd.c
│   ├── keypad.c
│   ├── rtc.c
│   ├── gpio.c
│   ├── interrupt.c
│   ├── ui.c
│   ├── hydration.c
│   ├── reminder.c
│   ├── config.c
│   └── delay.c
│
├── inc/
│   ├── aquaguardian.h
│   ├── app.h
│   ├── lcd.h
│   ├── keypad.h
│   ├── rtc.h
│   ├── gpio.h
│   ├── interrupt.h
│   ├── ui.h
│   ├── hydration.h
│   ├── reminder.h
│   ├── config.h
│   └── delay.h
│
├── docs/
│   ├── AquaGuardian_Project_Report.pdf
│   ├── logical_flow.png
│   └── circuit_diagram.png
│
├── tests/
│   └── test_cases.md
│
└── legacy/
    └── AquaGuardian-1.c
```
---
29. Technology Stack
Category                  Technology
---
Microcontroller           LPC2148 ARM7
Programming language      Embedded C
IDE / Build environment   Keil µVision
Simulation                Proteus
Display                   16×2 LCD
Input                     4×4 matrix keypad + push buttons
Time base                 LPC2148 RTC
Indicators                LEDs + buzzer
Version control           Git / GitHub
---
30. Project Status
Current status
Firmware: Working on physical LPC2148 hardware
Keil build: Successful
Compilation: 0 errors / 0 warnings in the reported build
Hardware operation: Working as expected
Modular source structure: Implemented
Documentation: This README documents the actual current firmware
Proteus: Schematic must be wired to exactly match the pin
mapping above before it can be considered a faithful simulation
---
31. GitHub Description
Suggested GitHub repository description:
> **AquaGuardian -- Smart Water Drinking Reminder System using LPC2148
> ARM7, RTC, LCD, keypad, LEDs and buzzer with configurable hydration
> targets and reminder scheduling.**
Suggested repository topics:
``` text
lpc2148
arm7
embedded-c
keil
proteus
embedded-systems
microcontroller
rtc
lcd
keypad
iot-project
hydration
water-reminder
```
---
32. Viva Summary
What is AquaGuardian?
AquaGuardian is an LPC2148-based embedded system that reminds the user
to drink water and tracks daily water intake.
Why is RTC used?
The RTC provides a real-time reference for maintaining time and
scheduling reminder intervals.
Why use a keypad?
The keypad allows the user to configure time, reminder interval and
daily water target.
Why use EINT0?
EINT0 provides an external-interrupt mechanism to request configuration
mode.
What happens when the water switch is pressed?
The consumed-water count is incremented, the remaining target is
updated, and the reminder countdown is restarted.
What happens if the user misses a reminder?
The miss counter is incremented and the red LED/buzzer provide the
missed-reminder indication.
What happens when the target is reached?
The system displays goal completion and provides a green indication,
then stops normal reminder processing for that day.
How do you test the reminder quickly?
Set:
``` c
#define DEMO_MODE 1
```
which changes the reminder interval to 15 seconds.
---
33. Conclusion
AquaGuardian demonstrates the integration of multiple embedded-system
concepts in a single LPC2148 application:
GPIO interfacing
LCD interfacing
Matrix keypad scanning
RTC operation
External interrupts
Button debouncing
Timer/countdown logic
User-interface design
State tracking
Input validation
Modular Embedded C programming
Hardware testing
Proteus-based simulation
The current README intentionally describes the actual working LPC2148
implementation and its real pin mapping, providing a reliable
reference for hardware assembly, Proteus wiring, Keil development,
testing and GitHub presentation.
---
License
This project is intended for educational and embedded-systems project
development.
See `LICENSE` for the repository license.
