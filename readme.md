## Day at BSU university

![](screenshots/bsu-1.png)


This is our student project made in spring 2021. 
It's game, that simulates a day at our university. 
The player has to explore its interiors, pass a meticulous guard, make it through a
dark labyrinth and master difficult minigames.

The game is written in C++ and uses the QT framework for windowing and graphics. 
A  self-written ECS powers the game core.
Maps are created using [Tiled](https://www.mapeditor.org/). 
Our animation parser picks up any [Aseprite](https://www.aseprite.org/) animations.


![](screenshots/bsu-2.png)

#### Building

The project can be built with CMake. It requires the Qt libraries 
(including mutlimedia) to be installed.
