# EE Data Collection Source Code
## This program uses an encoder to measure the rotation of a swinging arm

## Data Collection Procedure (Device Screen Captures):
### Step 1: Pull Back Arm
- The arm needs to be pulled back until its pressing the button

![Pull Back](https://github.com/Udit8348/EE_Source_Code/blob/master/Screen_Captures/pull_back.png)
### Step 2: Prompt to Release Arm
- 75: encoder measurement in degrees
- 1: the arm is currently pressing the arm
- Data collection starts the instant the arm is released

![Release](https://github.com/Udit8348/EE_Source_Code/blob/master/Screen_Captures/release.png)
### Step 3: Data Collection is Complete, Prompt to Save Data
- Once the oscilations have settled at 0 degrees the data collection stops
- Prompts to save colllected data by pressing the right button

![Complete](https://github.com/Udit8348/EE_Source_Code/blob/master/Screen_Captures/test_complete.png)
### Step 4: Notify that the Program was Saved
- Data was successfully saved to comma separed value file (.csv)

![Saved](https://github.com/Udit8348/EE_Source_Code/blob/master/Screen_Captures/test_saved.png)
### Preview Program Output
- See the enocder values, sampled every 50ms (20 Hz)
- See the elapsed time, sampled every 50ms (20 Hz)
- Output path (file name) of the saved data

![debug image](https://github.com/Udit8348/EE_Source_Code/blob/master/Screen_Captures/debug.png)
