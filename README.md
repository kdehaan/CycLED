**Objective**

This is a project designed and built in under 24 hours at the November 2017 HackEd hackathon.
The goal of the project was to create something that could be attached to a bicycle wheel that would track the revolutions per second and display a simple lightshow according to the frequency of revolution. When biking faster, the cycle of colours on the LED strip progresses more quickly, seeming to keep just ahead of the bicycle's speed at all times as the colors creep forward. When biking slowly, the color cycling slows in turn, yet still maintains a slightly faster rotation than the bicycle itself.

**Pictures and Video**

Testing the wheel for the first time:

![Testing Indoors](https://github.com/kdehaan/poor-man-s-persistence-of-vision/raw/master/indoor.jpg "testing indoors")

Going for a ride:

[![Bike POV](https://youtu.be/F7jCjdlAlsw)](https://youtu.be/F7jCjdlAlsw)

**Details**

The software is set up such that it should technically be able to keep up with revolutions of up to 2400 rpm (40 rps) or roughly 288 kph. There is a physical button that allows for easy toggling of the leds, and the system starts in and returns to a stationary mode with all-white leds when not moving.
The system is powered by a rechargeable LiPo 3.7v battery that runs both the adafruit arduino trinket and the neopixel strip. Revolutions are measured by means of a hall effect sensor attached to the arduino and two magnets attached to the frame of the bicycle.

**Parts**

Adafruit Trinket Pro (5v)

30 LED Adafruit Neopixel

3.7v 1200mAh LiPi rechargeable battery

salvaged ethernet cable (wiring)

Hall effect sensor

2 Rare earth magnets (attached together)

falling-voltage button

2 10kOhm resistors

Ethernet male/female pair (for quick removal)

2 Plastic bags (weatherproofing)


