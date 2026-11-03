# CO3009 – Two-Road Traffic Light System (XNUCLEO-F103RB)

Course project for Microprocessors – Microcontrollers (CO3009), Semester 1, academic year 2026-2027, Ho Chi Minh City University of Technology – VNU-HCM.

## Team Members

| Full name | Student ID |
|-----------|------------|
| [...]     | [...]      |
| [...]     | [...]      |
| [...]     | [...]      |

Group: [XX]

## Description

A traffic light system for a two-road intersection, running on the XNUCLEO-F103RB board (STM32F103RB):

- **Automatic mode**: the Red – Green – Amber lights of the two roads cycle alternately according to the configured durations.
- **Manual mode**: the user changes the light state with push buttons.
- **Tuning mode**: the Red / Green / Amber durations are adjusted with push buttons, with the values shown on the LCD.
- **Environment display**: temperature and humidity are read from the DHT20 sensor and shown on the LCD.
- **O(1) scheduler**: `SCH_Add_Task`, `SCH_Delete_Task`, `SCH_Update` (O(1)) and `SCH_Dispatch_Tasks`; one-shot tasks are supported.

## Project Structure

```
Core/Inc/   global.h  scheduler.h  software_timer.h  button.h  traffic_light.h
            dht20.h  lcd.h  fsm_automatic.h  fsm_manual.h  fsm_tuning.h
Core/Src/   corresponding .c files
```

## Naming Conventions

- File names: lowercase, words joined with underscores (e.g. `fsm_automatic.c`).
- Function names: `<module>_<action>()` (e.g. `button_read()`, `traffic_set_state()`).
- Commits: one meaningful change per commit, with a clear message (e.g. `scheduler: add one-shot task support`).

## Build and Flash

1. Open STM32CubeIDE and choose **File → Import → Existing Projects into Workspace**, then select the repository folder.
2. Check the `.ioc` file: the board must be NUCLEO-F103RB and **System Core → SYS → Debug** must be set to **Serial Wire**.
3. Build the project with **Project → Build Project**.
4. Connect the board over USB and flash it with **Run → Debug / Run**.

## Usage

[Describe what each push button does in each mode — fill in after Step 6 is complete.]

## Submitted Version

Tag: `v1.0-final` — Commit: [hash]
