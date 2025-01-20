#!/bin/bash

g++ ./src/emulator.cpp ./src/cpugui.cpp -o ./build/emulator -lsfml-graphics -lsfml-window -lsfml-system
./build/emulator

