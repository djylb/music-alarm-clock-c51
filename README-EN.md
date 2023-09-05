# Music-Alarm-Clock
A music alarm clock implemented using a 51 single-chip microcontroller.

## Compilation Instructions

Please modify the code according to the specific 51 microcontroller model you're using.

The code can be compiled using Keil.

## Demo Screenshots:
![image](https://user-images.githubusercontent.com/34548095/227974853-45a47138-2993-46ae-a068-3b32bcb86e06.png)
![image](https://user-images.githubusercontent.com/34548095/227981843-f70dc13d-2453-4348-bebf-3de8b77d7fcd.png)

## Usage Instructions for Hardware or Simulation

### 1. Buttons

K1: Mode switch. Cycles between time, stopwatch, and alarms 0, 1, 2.

K2: Increments values in settings mode or starts the stopwatch.

K3: Decrements values in settings mode or pauses the stopwatch.

K4: Enters settings, switches between settings (moves to the next setting option), and resets the stopwatch.

Note: Any button press will stop both the alarm and the hourly chime.

### 2. Time, Date, and Lunar Calendar Display

This screen includes date, day, time, and lunar calendar information.

Press K4 to enter the settings mode and use K2 and K3 to adjust the selected values. Press K4 again to move to the next setting. You can switch modes during configuration; any changes you've made will be saved.

### 3. Stopwatch Display

The stopwatch supports precision to one-hundredth of a second and has been manually calibrated for extremely low error (specific to CH549 development board).

On this screen, you can use K2, K3, and K4 to start, pause, and reset the stopwatch, respectively. Background timing is also supported.

### 4. Alarm Display

The alarm clock features three alarms: A0, A1, A2. Press K1 to cycle between these alarms. The selected alarm will be highlighted. You can configure the alarm time and recurring days using K4, K2, and K3.

### 5. Hourly Chime

Pre-set to chime between 8:00 and 21:00, using a 12-hour format. It will chime 12 times at 12:00 and once at 13:00.

### 6. Serial Port Settings (Controlled by ASCII strings)

Note: A successful operation will return "OK", otherwise, nothing will be displayed.

Set time command: T2022-01-01 00:00:00

Get current time: GET

Alarm settings: A+Alarm Number+HH:mm:ss+ +SunMonTueWedThuFriSat (Cycle On 1 Off 0)

Eg. A0 06:00:00 0111110      Sets alarm A0 for 6 a.m. on weekdays.

Stopwatch operation: Reset: S0 Start: S1 Pause: S2 

Button simulation: Simulate buttons K1-K4 eg. B0 B1 B2 B3 

## Microcontroller Resource Usage:

Timer 0: Handles music and hourly chime

Timer 1: UART0 Baud rate generation

Timer 2: Manages time, stopwatch, button states, alarm states, and hourly chime states.

## Wiring Instructions:

P3.0, P3.1: Connect to an external wireless serial communication module

P2.6, P2.7: Connect to OLED's SCL and SDA

P1.0: Connect to a transistor controlling the buzzer

P1.4-P1.7: Connect to buttons K1-K4
