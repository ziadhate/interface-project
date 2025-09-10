Stopwatch with Dual Mode (Increment & Countdown)
Using ATmega32 and Six Multiplexed Seven-Segment Displays
📌 Project Overview

This project implements a digital stopwatch with two operational modes:

Increment Mode (Default) → counts up from zero (HH:MM:SS).

Countdown Mode → counts down from a user-defined time with an alarm when it reaches zero.

The system is built using an ATmega32 microcontroller, six multiplexed seven-segment displays (common anode), and multiple push buttons for user interaction.

🎯 Features

⏱️ Increment Mode: Stopwatch starts from 00:00:00 and counts up.

⏳ Countdown Mode: User sets hours, minutes, seconds, then the timer counts down.

🔁 Mode Toggle: Switch easily between Increment and Countdown.

⏸️ Pause / Resume: Hold and resume counting at any time.

🔄 Reset: Reset stopwatch to 00:00:00.

🔔 Alarm/Buzzer: Rings when countdown reaches zero.

💡 LED Indicators:

Red LED → Increment Mode

Yellow LED → Countdown Mode

⚙️ Hardware Components

Microcontroller: ATmega32 (16 MHz clock).

Display: 6 × Seven-segment displays (common anode).

Driver: 7447 BCD-to-seven-segment decoder.

Inputs: 10 push buttons for reset, pause, resume, mode toggle, and time adjustment.

Outputs: Buzzer + LEDs (Red & Yellow).

Transistors: NPN BJTs for multiplexing the six seven-segment displays.

🛠️ System Design

Timer1 (CTC mode) is used for stopwatch counting.

External Interrupts:

INT0 → Reset (falling edge).

INT1 → Pause (rising edge).

INT2 → Resume (falling edge).

Multiplexing: First 6 pins of PORTA control enabling/disabling of the six displays.

Persistence of Vision is used to display all digits smoothly.
