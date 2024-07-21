# Music Player for STM32

## Overview
Design and implement a music player with hardware control and UART communication.
## Features
- **Startup**:
The program starts in pause mode.
The seven-segment display is blank, and no music plays from the buzzer.
- **Buttons**:

- Button 1 (Play/Pause):
  - Press once to start music playback, display the track number, and play music from the buzzer.
  - Press again to pause music playback.
- Button 2 (Change Track):
  - Hold the button and rotate the volume control to change the track number.
  - Release the button to confirm and play the selected track.
- Button 3 (Adjust Volume):
  - Hold the button to display the current volume level on the seven-segment display.
  - Rotate the volume control to adjust the volume.
  - Release the button to confirm the new volume level.
- **Tracks**:

  - At least 4 tracks must be available.
  - After finishing a track, the next track in the list automatically plays.
  - After the last track, playback restarts from the first track.
    
- **UART Commands**:

  - Change Track:
    - Command: MUSIC_SET(Number)
    - Displays an error message if the track number is invalid.
    - Plays the new track and displays a confirmation message if the track number is valid.
    - Change Volume:
    - Command: Change_Volume(Volume)
    - Displays an error message if the volume value is invalid.
    - Changes the volume and displays a confirmation message if the volume value is valid.
- **Messages**:
  - Error Messages:
  - [ERROR][Music not found][Time]
  - [ERROR][Volume not valid][Time]
  - Info Messages:
  - [INFO][Music changed to Number][Time]
  - [INFO][Volume changed to Volume][Time]
## Required Hardware
- Three external buttons
- Volume control (Potentiometer)
- Seven-segment display
- Buzzer
- UART
## Usage
  Download the code and implement it on the required hardware. Follow the instructions for using the external buttons and UART commands to play, pause, change tracks, and adjust the volume.
