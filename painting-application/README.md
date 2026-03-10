# Painting Application
---
This application is my first SDL2 Project. It is a simple painting application that has the following features:
1. Paint on mouse left click
2. Paint on mouse drag
3. Change the size of the brush
4. Change Color using Keyboard keys
5. Clear the whole screen 
6. Erase brush stroke
---
### User Guide
- r = red
- w = white (default)
- g = green
- b = blue
- e = black/erase
- c = clear screen
- arrow up = increase brush size
- arrow down = decrease brush size
---
### Used Modules/Libraries
1. <SDL2/SDL.h>
2. <stdbool.h>
---
### Cloning
1. Clone the Repository
```bash
git clone https://github.com/Wizard77-exe/cppGO
```
2. Head to the file folder
```bash
cd painting-application
```
3. Compile the c file and run the .exe file
```bash
gcc paint.c -o paint -lmingw32 -lSDL2main -lSDL2
./paint
```