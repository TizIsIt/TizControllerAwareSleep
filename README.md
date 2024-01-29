# TizControllerAwareSleep

This program will hibernate the PC if it does not detect any gamepad/controller input within 5 minutes. It started when I want my PC to acts like mobile phone where it would go to sleep if there's no user input for 5mins. You can run this program if you're going to play a game to sleep.
Especially good to use alongside OBS because it would help to stop the recording/streaming whenever you fall asleep.


How it works
---

This is a C++ console program. It will have a 5 minute countdown where at the end, it will put the PC into hibernate. Any input from the game controller (Xinput) will reset the timer back to 5 minutes.

The console will show the remaining time every 2 seconds. It will also show if there's any game controller input.

It can be run in the background obviously.

How to get it
---

- Simply download the TizControllerAwareSleep.exe and run it
- Alternatively, you can download the .cpp file so you could compile and build it yourself in Visual Studio

NOTE
---
- This only works on Windows PC
- Only works with Xinput controller (Xbox controller). I suppose you could use Xinput wrapper, but I did not test that.
