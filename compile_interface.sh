#!/bin/bash

g++ ./src/gui_test.cpp ./src/cpugui.cpp -o ./build/cpugui -lsfml-graphics -lsfml-window -lsfml-system
./build/cpugui

