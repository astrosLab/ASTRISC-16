#!/usr/bin/env bash

#g++ -g -fsanitize=address -o ./build/emulator ./src/emulator.cpp ./src/cpugui.cpp -lsfml-graphics -lsfml-window -lsfml-system
g++ -o ./build/emulator ./src/emulator.cpp ./src/cpugui.cpp -lsfml-graphics -lsfml-window -lsfml-system
./build/emulator

