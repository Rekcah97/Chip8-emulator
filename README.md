# CHIP-8 Emulator

A CHIP-8 emulator written in C++17, using SDL2 for display and input.

## Build

Requires a C++17 compiler and SDL2 (`libsdl2-dev` on Debian/Ubuntu, `brew install sdl2` on macOS).

```
g++ -std=c++17 -Wall -Wextra -o build/chip8 main.cpp src/chip8.cpp src/platform.cpp $(sdl2-config --cflags --libs)
```

## Usage

```
./chip8 path/to/rom.ch8
```

Press `Esc` or close the window to quit.

## Controls

```
CHIP-8 keypad     Keyboard
1 2 3 C           1 2 3 4
4 5 6 D           Q W E R
7 8 9 E           A S D F
A 0 B F           Z X C V
```

## Details

- 64x32 display, drawn at 10x scale (640x320 window)
- 10 instructions per frame, with timers ticking once per frame (vsync, about 60 Hz)
- All 34 standard opcodes are dispatched; unknown opcodes are ignored

## Resources

- [Building a CHIP-8 Emulator [C++]](https://austinmorlan.com/posts/chip8_emulator/), Austin Morlan
- [CHIP-8 design report](https://www.cs.columbia.edu/~sedwards/classes/2016/4840-spring/designs/Chip8.pdf), Columbia University CS 4840
- [Cowgod's CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [chip8-test-suite](https://github.com/Timendus/chip8-test-suite), Timendus
