# SpokeRotate by Riley Tranquillow

It's a program where some circles rotate while being linked together and junk.

## Features

- Works with virtually infinite number of circles \(I've tried ten thousand and it works albeit at one frame per second\)
- Toggling outlines on/off \(should've increased the frame rate but it doesnt help much\).
- Commands scrolling towards the bottom
- Info about the circles
- Randomized parameters affecting the circles' attributes
- Randomizer seed tied to the CPU's clock speed, so it will change from CPU to CPU
- Alternate view without circles and at a %20 scale
- Resizable window
- A window icon

Requires CMake >3.11 and [raylib 5.0](https://github.com/raysan5/raylib/) to be installed already.

## Installation

***Please try the program with X11, I have not tried it with Wayland, at all.***
``` bash
cmake -B build && cmake --build build
```
Once it hopefully compiles without error, the binary should be `./build/SpokeRotate`

## Screenshots

Note: Some screenshots might be old and not show the complete feature set.

![Circles, Many Circles](https://github.com/Tranquillow/SpokeRotate/blob/master/screenshots/Screenshot%20at%202023-12-24%2009-17-29.png)
![Few Circles](https://github.com/Tranquillow/SpokeRotate/blob/master/screenshots/Screenshot%20at%202023-12-24%2012-45-14.png)
![Alternate View](https://github.com/Tranquillow/SpokeRotate/blob/master/screenshots/Screenshot%20at%202023-12-24%2012-46-07.png)
