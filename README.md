# Cpu graph on MAX7219 8x8 led matrix display

![Sample](ezgif-5-f1e95c971952.gif?raw=true)

# Quick about the project
This project is inspired by [THIS PROJECT](https://www.raspberrypi.org/forums/viewtopic.php?t=18964) which reads CPU usage and turns on several LEDs. My implementation rather uses 8x8 matrix display where I wanted to show CPU usage as a history graph.
From mentioned project I used '**get_cpu_usage**' which reads Linux' **/proc/stat** file and wrote my logic to draw CPU history graph.

# How it's working
* It's written in C++.
* It uses popular WiringPi lib to control GPIOs.
* It uses Max7219 library I created based on [THIS CODE](http://www.plingboot.com/2013/01/driving-the-max7219-with-the-raspberry-pi/) to easy communicate with 8x8 display using any 3 digital pins. [HERE](http://www.netzmafia.de/skripten/hardware/RasPi/Projekt-MAX7219/index.html) you can find additinal information about commands that MAX7219 can receive so you can customize it as you like. MAX7219 is a chip that drives 8x8 display.

# Instalation
1) Install WiringPi: http://wiringpi.com/download-and-install/
2) Clone this project
3) Run '**make**' to build executable
4) Execute with '**sudo ./cpugraphpi 1000**' (1000 means 1000ms delay after each reading)

## Wiring the display with Raspberry Pi
The code is set to use these pins but you are free to change them:
```
+5V/VCC:     Rpi header pin 2
GND:         Rpi header pin 6

DATA/DIN:    0 - GPIO 17 (WiringPi pin num 0) Rpi header pin 11
CLOCK/CLK:   3 - GPIO 22 (WiringPi pin num 3) Rpi header pin 15
LOAD/CS:     4 - GPIO 23 (WiringPi pin num 4) Rpi header pin 16

```

![Rpi pinout](https://docs.particle.io/assets/images/pi-pinout-diagram-01.png)
