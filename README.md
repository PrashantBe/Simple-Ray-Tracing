This is a simple ray tracing simulation written in C using the SDL2 graphics library. Inspired by HirschDaniel , this project demonstrates the foundational principles behind how real-time ray tracing works in games — in a super lightweight, visual way.



 > WHAT IT DOES : 

- Simulates a light source emitting 1000 rays in all directions

- Rays dynamically reflect based on a moving circular object

- Visual shadow effect where rays stop upon hitting the object

- Interactive mouse tracking to move the light source (circle)

> TECH STACK :

- Language: C

- Graphics: SDL2

**This was a personal learning project to understand how ray tracing works under the hood — the same technique used in modern gaming graphics (like in NVIDIA RTX).**

> TO RUN : 

Make sure you have SDL2 installed. 

  LINUX
  - sudo apt install libsdl2-dev
  
  COMPILE AND RUN

  - gcc -o raytracing rayTracing.c -lm  `sdl2-config --cflags --libs `
  - ./raytracing
