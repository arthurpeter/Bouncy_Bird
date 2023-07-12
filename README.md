# Bouncy_Bird - The Game

## A fully functional EXAMPLE project written in C++ showing how to create a simple game for Windows

This project shows how to create a custom game in C++ with the following features:

* Create functional window with the windows.h library
* Draw shapes, numbers and letters on the window
* Take in input from the keyboard
* Game physics
* How to make a Main Menu and an End Screen
* Simple Highscore system using a text file
* Simple Sound input

## How to Play

To run the project you need to write the following commands into the terminal:

* g++ main.cpp -o main -lgdi32 -lwinmm
* .\main.exe

When you enter the game, the Main Menu will show up where you can navigate the options using either the
LEFT and RIGHT arrows on your keyboard or the A and D keys and select the game mode by pressing the enter
key. For the Single Player option, you can make your bird "jump" by pressing the space bar or the UP arrow. 
For the multiplayer option, the first player (the yellow bird) is controlled either by the space bar or 
the UP arrow and the seccond player (the blue bird) is controlled by the W key. When a player dies, 
the game goes to the End Screen where you control the options as you would in the Main Menu.
